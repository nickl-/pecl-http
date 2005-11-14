/*
    +--------------------------------------------------------------------+
    | PECL :: http                                                       |
    +--------------------------------------------------------------------+
    | Redistribution and use in source and binary forms, with or without |
    | modification, are permitted provided that the conditions mentioned |
    | in the accompanying LICENSE file are met.                          |
    +--------------------------------------------------------------------+
    | Copyright (c) 2004-2005, Michael Wallner <mike@php.net>            |
    +--------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif
#include "php.h"

#include "php_http_std_defs.h"
#include "php_http_api.h"
#include "php_http_filter_api.h"

#include "phpstr/phpstr.h"

#include "php_streams.h"

PHP_MINIT_FUNCTION(http_filter)
{
	php_stream_filter_register_factory("http.*", &http_filter_factory TSRMLS_CC);
	return SUCCESS;
}

/*
	-
*/

typedef struct {
	phpstr	buffer;
	ulong	hexlen;
} http_filter_buffer;

#define HTTP_FILTER_PARAMS \
	php_stream *stream, \
	php_stream_filter *this, \
	php_stream_bucket_brigade *buckets_in, \
	php_stream_bucket_brigade *buckets_out, \
	size_t *bytes_consumed, int flags \
	TSRMLS_DC
#define HTTP_FILTER_OP(filter) \
	http_filter_op_ ##filter
#define HTTP_FILTER_OPS(filter) \
	php_stream_filter_ops HTTP_FILTER_OP(filter)
#define HTTP_FILTER_DTOR(filter) \
	http_filter_ ##filter## _dtor
#define HTTP_FILTER_DESTRUCTOR(filter) \
	void HTTP_FILTER_DTOR(filter)(php_stream_filter *this TSRMLS_DC)
#define HTTP_FILTER_FUNC(filter) \
	http_filter_ ##filter
#define HTTP_FILTER_FUNCTION(filter) \
	php_stream_filter_status_t HTTP_FILTER_FUNC(filter)(HTTP_FILTER_PARAMS)

#define NEW_BUCKET(data, length) \
	{ \
		char *__data; \
		php_stream_bucket *__buck; \
		\
		__data = pemalloc(length, this->is_persistent); \
		if (!__data) { \
			return PSFS_ERR_FATAL; \
		} \
		memcpy(__data, data, length); \
		\
		__buck = php_stream_bucket_new(stream, __data, length, 1, this->is_persistent TSRMLS_CC); \
		if (!__buck) { \
			pefree(__data, this->is_persistent); \
			return PSFS_ERR_FATAL; \
		} \
		\
		php_stream_bucket_append(buckets_out, __buck TSRMLS_CC); \
	}

static HTTP_FILTER_FUNCTION(chunked_decode)
{
	int out_avail = 0;
	php_stream_bucket *ptr, *nxt;
	http_filter_buffer *buffer = (http_filter_buffer *) (this->abstract);
	
	if (bytes_consumed) {
		*bytes_consumed = 0;
	}
	
	/* new data available? */
	if (buckets_in->head) {
		
		/* fetch available bucket data */
		for (ptr = buckets_in->head; ptr; ptr = nxt) {
			nxt = ptr->next;
			if (bytes_consumed) {
				*bytes_consumed += ptr->buflen;
			}
		
			if ((size_t) -1 == phpstr_append(PHPSTR(buffer), ptr->buf, ptr->buflen)) {
				return PSFS_ERR_FATAL;
			}
			
			php_stream_bucket_unlink(ptr TSRMLS_CC);
			php_stream_bucket_delref(ptr TSRMLS_CC);
		}
	}
	if (!phpstr_fix(PHPSTR(buffer))) {
		return PSFS_ERR_FATAL;
	}

	/* we have data in our buffer */
	while (PHPSTR_LEN(buffer)) {
	
		/* we already know the size of the chunk and are waiting for data */
		if (buffer->hexlen) {
		
			/* not enough data buffered */
			if (PHPSTR_LEN(buffer) < buffer->hexlen) {
			
				/* flush anyway? */
				if (flags == PSFS_FLAG_FLUSH_INC) {
				
					/* flush all data (should only be chunk data) */
					out_avail = 1;
					NEW_BUCKET(PHPSTR_VAL(buffer), PHPSTR_LEN(buffer));
					
					/* waiting for less data now */
					buffer->hexlen -= PHPSTR_LEN(buffer);
					/* no more buffered data */
					phpstr_reset(PHPSTR(buffer));
					/* break */
				} 
				
				/* we have too less data and don't need to flush */
				else {
					break;
				}
			} 
			
			/* we seem to have all data of the chunk */
			else {
				out_avail = 1;
				NEW_BUCKET(PHPSTR_VAL(buffer), buffer->hexlen);
				
				/* remove outgoing data from the buffer */
				phpstr_cut(PHPSTR(buffer), 0, buffer->hexlen);
				/* reset hexlen */
				buffer->hexlen = 0;
				/* continue */
			}
		} 
		
		/* we don't know the length of the chunk yet */
		else {
			size_t off = 0;
			
			/* ignore preceeding CRLFs (too loose?) */
			while (off < PHPSTR_LEN(buffer) && (
					PHPSTR_VAL(buffer)[off] == 0xa || 
					PHPSTR_VAL(buffer)[off] == 0xd)) {
				++off;
			}
			if (off) {
				phpstr_cut(PHPSTR(buffer), 0, off);
			}
			
			/* still data there? */
			if (PHPSTR_LEN(buffer)) {
				int eollen;
				const char *eolstr;
				
				/* we need eol, so we can be sure we have all hex digits */
				phpstr_fix(PHPSTR(buffer));
				if (eolstr = http_locate_eol(PHPSTR_VAL(buffer), &eollen)) {
					char *stop = NULL;
					
					/* read in chunk size */
					buffer->hexlen = strtoul(PHPSTR_VAL(buffer), &stop, 16);
					
					/*	if strtoul() stops at the beginning of the buffered data
						there's domething oddly wrong, i.e. bad input */
					if (stop == PHPSTR_VAL(buffer)) {
						return PSFS_ERR_FATAL;
					}
					
					/* cut out <chunk size hex><chunk extension><eol> */
					phpstr_cut(PHPSTR(buffer), 0, eolstr + eollen - PHPSTR_VAL(buffer));
					/* buffer->hexlen is 0 now or contains the size of the next chunk */
					/* continue */
				} else {
					/* we have not enough data buffered to read in chunk size */
					break;
				}
			}
			/* break */
		}
	}
	
	/* flush before close, but only if we are already waiting for more data */
	if (flags == PSFS_FLAG_FLUSH_CLOSE && buffer->hexlen && PHPSTR_LEN(buffer)) {
		out_avail = 1;
		NEW_BUCKET(PHPSTR_VAL(buffer), PHPSTR_LEN(buffer));
		phpstr_reset(PHPSTR(buffer));
		buffer->hexlen = 0;
	}
	
	return out_avail ? PSFS_PASS_ON : PSFS_FEED_ME;
}

static HTTP_FILTER_DESTRUCTOR(chunked_decode)
{
	http_filter_buffer *b = (http_filter_buffer *) (this->abstract);
	
	phpstr_dtor(PHPSTR(b));
	pefree(b, this->is_persistent);
}

static HTTP_FILTER_FUNCTION(chunked_encode)
{
	int out_avail = 0;
	php_stream_bucket *ptr, *nxt;
	
	if (bytes_consumed) {
		*bytes_consumed = 0;
	}
	
	/* new data available? */
	if (buckets_in->head) {
		phpstr buf;
		out_avail = 1;
		
		phpstr_init(&buf);
		
		/* fetch available bucket data */
		for (ptr = buckets_in->head; ptr; ptr = nxt) {
			nxt = ptr->next;
			if (bytes_consumed) {
				*bytes_consumed += ptr->buflen;
			}
			
			phpstr_appendf(&buf, "%x" HTTP_CRLF, ptr->buflen);
			phpstr_append(&buf, ptr->buf, ptr->buflen);
			phpstr_appends(&buf, HTTP_CRLF);
			
			/* pass through */
			NEW_BUCKET(PHPSTR_VAL(&buf), PHPSTR_LEN(&buf));
			/* reset */
			phpstr_reset(&buf);
			
			php_stream_bucket_unlink(ptr TSRMLS_CC);
			php_stream_bucket_delref(ptr TSRMLS_CC);
		}
		
		/* free buffer */
		phpstr_dtor(&buf);
	}
	
	/* terminate with "0" */
	if (flags == PSFS_FLAG_FLUSH_CLOSE) {
		out_avail = 1;
		NEW_BUCKET("0" HTTP_CRLF, lenof("0" HTTP_CRLF));
	}
	
	return out_avail ? PSFS_PASS_ON : PSFS_FEED_ME;
}

static HTTP_FILTER_OPS(chunked_decode) = {
	HTTP_FILTER_FUNC(chunked_decode),
	HTTP_FILTER_DTOR(chunked_decode),
	"http.chunked_decode"
};

static HTTP_FILTER_OPS(chunked_encode) = {
	HTTP_FILTER_FUNC(chunked_encode),
	NULL,
	"http.chunked_encode"
};

static php_stream_filter *http_filter_create(const char *name, zval *params, int p TSRMLS_DC)
{
	php_stream_filter *f = NULL;
	
	if (!strcasecmp(name, "http.chunked_decode")) {
		http_filter_buffer *b = NULL;
		
		if (b = pecalloc(1, sizeof(http_filter_buffer), p)) {
			phpstr_init_ex(PHPSTR(b), 4096, p ? PHPSTR_INIT_PERSISTENT : 0);
			if (!(f = php_stream_filter_alloc(&HTTP_FILTER_OP(chunked_decode), b, p))) {
				pefree(b, p);
			}
		}
	} else
	
	if (!strcasecmp(name, "http.chunked_encode")) {
		f = php_stream_filter_alloc(&HTTP_FILTER_OP(chunked_encode), NULL, p);
	}
	
	return f;
}

php_stream_filter_factory http_filter_factory = {
	http_filter_create
};

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
