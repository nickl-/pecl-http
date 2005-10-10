/*
   +----------------------------------------------------------------------+
   | PECL :: http                                                         |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.0 of the PHP license, that  |
   | is bundled with this package in the file LICENSE, and is available   |
   | through the world-wide-web at http://www.php.net/license/3_0.txt.    |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Copyright (c) 2004-2005 Michael Wallner <mike@php.net>               |
   +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#	include "config.h"
#endif
#include "php.h"

#include "php_http_encoding_api.h"
#include "php_http.h"
#include "php_http_api.h"

#ifdef HTTP_HAVE_ZLIB
#include <zlib.h>

#define HTTP_GZMAXTRY 10
#define HTTP_GZBUFLEN(l) (l + (l / 1000) + 16 + 1)

ZEND_EXTERN_MODULE_GLOBALS(http);

static const char http_gzencode_header[] = {
	(const char) 0x1f, 
	(const char) 0x8b, 
	(const char) Z_DEFLATED, 
	0, 0, 0, 0, 0, 0, 
	(const char) 0x03
};

inline void http_init_gzencode_buffer(z_stream *Z, const char *data, size_t data_len, char **buf_ptr)
{
	Z->zalloc = Z_NULL;
	Z->zfree  = Z_NULL;
	Z->opaque = Z_NULL;
	
	Z->next_in   = (Bytef *) data;
	Z->avail_in  = data_len;
	Z->avail_out = HTTP_GZBUFLEN(data_len) - 1;
	
	*buf_ptr = emalloc(Z->avail_out + sizeof(http_gzencode_header));
	memcpy(*buf_ptr, http_gzencode_header, sizeof(http_gzencode_header));
	
	Z->next_out = *buf_ptr + sizeof(http_gzencode_header);
}

inline void http_init_deflate_buffer(z_stream *Z, const char *data, size_t data_len, char **buf_ptr)
{
	Z->zalloc = Z_NULL;
	Z->zfree  = Z_NULL;
	Z->opaque = Z_NULL;

	Z->data_type = Z_ASCII;
	Z->next_in   = (Bytef *) data;
	Z->avail_in  = data_len;
	Z->avail_out = HTTP_GZBUFLEN(data_len) - 1;
	Z->next_out  = emalloc(Z->avail_out);
	
	*buf_ptr = Z->next_out;
}

inline void http_init_inflate_buffer(z_stream *Z, const char *data, size_t data_len, char **buf_ptr, size_t *buf_len, int iteration)
{
	Z->zalloc = Z_NULL;
	Z->zfree  = Z_NULL;
	
	if (!iteration) {
		*buf_len = data_len * 2;
		*buf_ptr = emalloc(*buf_len + 1);
	} else {
		*buf_len <<= 2;
		*buf_ptr = erealloc(*buf_ptr, *buf_len + 1);
	}
	
	Z->next_in   = (Bytef *) data;
	Z->avail_in  = data_len;
	Z->avail_out = *buf_len;
	Z->next_out  = *buf_ptr;
}

inline size_t http_finish_buffer(size_t buf_len, char **buf_ptr)
{
	(*buf_ptr)[buf_len] = '\0';
	return buf_len;
}

inline size_t http_finish_gzencode_buffer(z_stream *Z, const char *data, size_t data_len, char **buf_ptr)
{
	unsigned long crc;
	char *trailer;
	
	crc = crc32(0L, Z_NULL, 0);
	crc = crc32(crc, (const Bytef *) data, data_len);
	
	trailer = *buf_ptr + sizeof(http_gzencode_header) + Z->total_out;
	
	/* write crc & stream.total_in in LSB order */
	trailer[0] = (char) crc & 0xFF;
	trailer[1] = (char) (crc >> 8) & 0xFF;
	trailer[2] = (char) (crc >> 16) & 0xFF;
	trailer[3] = (char) (crc >> 24) & 0xFF;
	trailer[4] = (char) (Z->total_in) & 0xFF;
	trailer[5] = (char) (Z->total_in >> 8) & 0xFF;
	trailer[6] = (char) (Z->total_in >> 16) & 0xFF;
	trailer[7] = (char) (Z->total_in >> 24) & 0xFF;
	
	return http_finish_buffer(Z->total_out + sizeof(http_gzencode_header) + 8, buf_ptr);
}


PHP_HTTP_API STATUS _http_encoding_gzencode(int level, const char *data, size_t data_len, char **encoded, size_t *encoded_len TSRMLS_DC)
{
	z_stream Z;
	STATUS status = Z_OK;
	
	http_init_gzencode_buffer(&Z, data, data_len, encoded);
	
	if (	(Z_OK == (status = deflateInit2(&Z, level, Z_DEFLATED, -MAX_WBITS, MAX_MEM_LEVEL, Z_DEFAULT_STRATEGY))) &&
			(Z_STREAM_END == (status = deflate(&Z, Z_FINISH))) &&
			(Z_OK == (status = deflateEnd(&Z)))) {
		*encoded_len = http_finish_gzencode_buffer(&Z, data, data_len, encoded);
		return SUCCESS;
	}
	
	efree(*encoded);
	http_error_ex(HE_WARNING, HTTP_E_ENCODING, "Could not gzencode data: %s", zError(status));
	return FAILURE;
}

PHP_HTTP_API STATUS _http_encoding_deflate(int level, const char *data, size_t data_len, char **encoded, size_t *encoded_len TSRMLS_DC)
{
	z_stream Z;
	STATUS status = Z_OK;
	
	http_init_deflate_buffer(&Z, data, data_len, encoded);
	
	if (	(Z_OK == (status = deflateInit2(&Z, level, Z_DEFLATED, -MAX_WBITS, MAX_MEM_LEVEL, Z_DEFAULT_STRATEGY))) &&
			(Z_STREAM_END == (status = deflate(&Z, Z_FINISH))) &&
			(Z_OK == (status = deflateEnd(&Z)))) {
		*encoded_len = http_finish_buffer(Z.total_out, encoded);
		return SUCCESS;
	}
	
	efree(encoded);
	http_error_ex(HE_WARNING, HTTP_E_ENCODING, "Could not deflate data: %s", zError(status));
	return FAILURE;
}

PHP_HTTP_API STATUS _http_encoding_compress(int level, const char *data, size_t data_len, char **encoded, size_t *encoded_len TSRMLS_DC)
{
	STATUS status;
	
	*encoded = emalloc(*encoded_len = HTTP_GZBUFLEN(data_len));
	
	if (Z_OK == (status = compress2(*encoded, encoded_len, data, data_len, level))) {
		http_finish_buffer(*encoded_len, encoded);
		return SUCCESS;
	}
	
	efree(encoded);
	http_error_ex(HE_WARNING, HTTP_E_ENCODING, "Could not compress data: %s", zError(status));
	return FAILURE;
}

PHP_HTTP_API STATUS _http_encoding_gzdecode(const char *data, size_t data_len, char **decoded, size_t *decoded_len TSRMLS_DC)
{
	const char *encoded = data + sizeof(http_gzencode_header);
	size_t encoded_len;
	
	if (data_len <= sizeof(http_gzencode_header) + 8) {
		http_error(HE_WARNING, HTTP_E_ENCODING, "Could not gzdecode data: too short data length");
	} else {
		encoded_len = data_len - sizeof(http_gzencode_header) - 8;
		
		if (SUCCESS == http_encoding_inflate(encoded, encoded_len, decoded, decoded_len)) {
			unsigned long len = 0, cmp = 0, crc = crc32(0L, Z_NULL, 0);
			
			crc = crc32(crc, *decoded, *decoded_len);
			
			cmp  = (unsigned) (data[data_len-8]);
			cmp += (unsigned) (data[data_len-7] << 8);
			cmp += (unsigned) (data[data_len-6] << 16);
			cmp += (unsigned) (data[data_len-5] << 24);
			len  = (unsigned) (data[data_len-4]);
			len += (unsigned) (data[data_len-4] << 8);
			len += (unsigned) (data[data_len-4] << 16);
			len += (unsigned) (data[data_len-4] << 24);
			
			if (cmp != crc) {
				http_error(HE_NOTICE, HTTP_E_ENCODING, "Could not verify data integrity: CRC check failed");
			}
			if (len != *decoded_len) {
				http_error(HE_NOTICE, HTTP_E_ENCODING, "Could not verify data integrity: data length check failed");
			}
			
			return SUCCESS;
		}
	}
	return FAILURE;
}

PHP_HTTP_API STATUS _http_encoding_inflate(const char *data, size_t data_len, char **decoded, size_t *decoded_len TSRMLS_DC)
{
	int max = 0;
	STATUS status;
	z_stream Z;
	
	do {
		http_init_inflate_buffer(&Z, data, data_len, decoded, decoded_len, max++);
		if (Z_OK == (status = inflateInit2(&Z, -MAX_WBITS))) {
			if (Z_STREAM_END == (status = inflate(&Z, Z_FINISH))) {
				if (Z_OK == (status = inflateEnd(&Z))) {
					*decoded_len = http_finish_buffer(Z.total_out, decoded);
					return SUCCESS;
				}
			}
		}
	} while (max < HTTP_GZMAXTRY);
	
	http_error_ex(HE_WARNING, HTTP_E_ENCODING, "Could not inflate data: %s", zError(status));
	return FAILURE;
}

PHP_HTTP_API STATUS _http_encoding_uncompress(const char *data, size_t data_len, char **decoded, size_t *decoded_len TSRMLS_DC)
{
	int max = 0;
	STATUS status;
	size_t want = data_len * 2;
	
	*decoded = emalloc(want + 1);
	if (Z_BUF_ERROR == (status = uncompress(*decoded, &want, data, data_len))) do {
		/*	this is a lot faster with large data than gzuncompress(),
			but could be a problem with a low memory limit */
		want <<= 2;
		*decoded = erealloc(*decoded, want + 1);
		status = uncompress(*decoded, &want, data, data_len);
	} while (++max < HTTP_GZMAXTRY && status == Z_BUF_ERROR);
	
	if (Z_OK == status) {
		*decoded_len = http_finish_buffer(want, decoded);
		return SUCCESS;
	}
	
	efree(*decoded);
	http_error_ex(HE_WARNING, HTTP_E_ENCODING, "Could not uncompress data: %s", zError(status));
	return FAILURE;
}

#endif /* HTTP_HAVE_ZLIB */

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */

