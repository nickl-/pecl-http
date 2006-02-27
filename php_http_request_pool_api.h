/*
    +--------------------------------------------------------------------+
    | PECL :: http                                                       |
    +--------------------------------------------------------------------+
    | Redistribution and use in source and binary forms, with or without |
    | modification, are permitted provided that the conditions mentioned |
    | in the accompanying LICENSE file are met.                          |
    +--------------------------------------------------------------------+
    | Copyright (c) 2004-2006, Michael Wallner <mike@php.net>            |
    +--------------------------------------------------------------------+
*/

/* $Id$ */

#ifndef PHP_HTTP_REQUEST_POOL_API_H
#define PHP_HTTP_REQUEST_POOL_API_H
#ifdef HTTP_HAVE_CURL

typedef struct _http_request_pool_t {
	CURLM *ch;
	zend_llist finished;
	zend_llist handles;
	int unfinished;
} http_request_pool;

#define http_request_pool_responsehandler _http_request_pool_responsehandler
extern void _http_request_pool_responsehandler(zval **req, CURL *ch TSRMLS_DC);

#define http_request_pool_try \
	{ \
		zval *old_exception = EG(exception); \
		EG(exception) = NULL;
#define http_request_pool_catch() \
		if (EG(exception)) { \
			http_request_pool_wrap_exception(old_exception, EG(exception)); \
		} else { \
			EG(exception) = old_exception; \
		} \
	}
#define http_request_pool_final() \
	if (EG(exception)) { \
		zval *exception; \
		http_request_pool_wrap_exception(NULL, EG(exception)); \
		exception = EG(exception); \
		EG(exception) = NULL; \
		zend_throw_exception_object(exception TSRMLS_CC); \
	}

#define http_request_pool_wrap_exception(o, n) _http_request_pool_wrap_exception((o), (n) TSRMLS_CC)
extern void _http_request_pool_wrap_exception(zval *old_exception, zval *new_exception TSRMLS_DC);


#define http_request_pool_init(p) _http_request_pool_init((p) TSRMLS_CC)
PHP_HTTP_API http_request_pool *_http_request_pool_init(http_request_pool *pool TSRMLS_DC);

#define http_request_pool_attach(p, r) _http_request_pool_attach((p), (r) TSRMLS_CC)
PHP_HTTP_API STATUS _http_request_pool_attach(http_request_pool *pool, zval *request TSRMLS_DC);

#define http_request_pool_detach(p, r) _http_request_pool_detach((p), (r) TSRMLS_CC)
PHP_HTTP_API STATUS _http_request_pool_detach(http_request_pool *pool, zval *request TSRMLS_DC);

#define http_request_pool_detach_all(p) _http_request_pool_detach_all((p) TSRMLS_CC)
PHP_HTTP_API void _http_request_pool_detach_all(http_request_pool *pool TSRMLS_DC);

#define http_request_pool_send(p) _http_request_pool_send((p) TSRMLS_CC)
PHP_HTTP_API STATUS _http_request_pool_send(http_request_pool *pool TSRMLS_DC);

#define http_request_pool_select _http_request_pool_select
PHP_HTTP_API STATUS _http_request_pool_select(http_request_pool *pool);

#define http_request_pool_perform(p) _http_request_pool_perform((p) TSRMLS_CC)
PHP_HTTP_API int _http_request_pool_perform(http_request_pool *pool TSRMLS_DC);

#define http_request_pool_dtor(p) _http_request_pool_dtor((p) TSRMLS_CC)
PHP_HTTP_API void _http_request_pool_dtor(http_request_pool *pool TSRMLS_DC);

#endif
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */

