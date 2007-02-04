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

#ifndef HTTP_PERSISTENT_HANDLE_H
#define HTTP_PERSISTENT_HANDLE_H

#ifdef HTTP_HAVE_PERSISTENT_HANDLES
typedef void *(*http_persistent_handle_ctor)(void);
typedef void (*http_persistent_handle_dtor)(void *handle);

PHP_MINIT_FUNCTION(http_persistent_handle);
PHP_MSHUTDOWN_FUNCTION(http_persistent_handle);

#define http_persistent_handle_provide(n, c, d) _http_persistent_handle_provide_ex((n), strlen(n), (c), (d))
#define http_persistent_handle_provide_ex(n, l, c, d) _http_persistent_handle_provide_ex((n), (l), (c), (d))
PHP_HTTP_API STATUS _http_persistent_handle_provide_ex(const char *name_str, size_t name_len, http_persistent_handle_ctor ctor, http_persistent_handle_dtor dtor);

#define http_persistent_handle_cleanup(n, c) _http_persistent_handle_cleanup_ex((n), strlen(n), (c) TSRMLS_CC)
#define http_persistent_handle_cleanup_ex(n, l,c ) _http_persistent_handle_cleanup_ex((n), (l), (c) TSRMLS_CC)
PHP_HTTP_API void _http_persistent_handle_cleanup_ex(const char *name_str, size_t name_len, int current_ident_only TSRMLS_DC);

#define http_persistent_handle_statall() _http_persistent_handle_statall_ex(NULL)
#define http_persistent_handle_statall_ex(ht) _http_persistent_handle_statall_ex((ht))
PHP_HTTP_API HashTable *_http_persistent_handle_statall_ex(HashTable *ht);

#define http_persistent_handle_acquire(n, h) _http_persistent_handle_acquire_ex((n), strlen(n), (h) TSRMLS_CC)
#define http_persistent_handle_acquire_ex(n, l, h) _http_persistent_handle_acquire_ex((n), (l), (h) TSRMLS_CC)
PHP_HTTP_API STATUS _http_persistent_handle_acquire_ex(const char *name_str, size_t name_len, void **handle TSRMLS_DC);

#define http_persistent_handle_release(n, h) _http_persistent_handle_release_ex((n), strlen(n), (h) TSRMLS_CC)
#define http_persistent_handle_release_ex(n, l, h) _http_persistent_handle_release_ex((n), (l), (h) TSRMLS_CC)
PHP_HTTP_API STATUS _http_persistent_handle_release_ex(const char *name_str, size_t name_len, void **handle TSRMLS_DC);

#endif /* HTTP_HAVE_PERSISTENT_HANDLES */
#endif /* HTTP_PERSISTENT_HANDLE_H */
