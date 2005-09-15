dnl config.m4 for pecl/http
dnl $Id$

PHP_ARG_ENABLE([http], [whether to enable extended HTTP support],
[  --enable-http           Enable extended HTTP support])
PHP_ARG_WITH([http-curl-requests], [wheter to enable cURL HTTP requests],
[  --with-http-curl-requests[=CURLDIR]
                           With cURL HTTP request support])
PHP_ARG_WITH([http-mhash-etags], [whether to enable mhash ETag generator],
[  --with-http-mhash-etags[=MHASHDIR]
                           With mhash ETag generator support])
PHP_ARG_WITH([http-magic-mime], [whether to enable response content type guessing],
[  --with-http-magic-mime[=MAGICDIR]
                           With magic mime response content type guessing])

if test "$PHP_HTTP" != "no"; then

dnl -------
dnl NETDB.H
dnl -------
	AC_MSG_CHECKING(for netdb.h)
	if test -r /usr/include/netdb.h -o -r /usr/local/include/netdb.h; then
		AC_DEFINE(HAVE_NETDB_H, 1, [Have netdb.h])
		AC_MSG_RESULT(found in default path)
	else
		AC_MSG_RESULT(not found in default path)
	fi

dnl ----
dnl CURL
dnl ----
	if test "$PHP_HTTP_CURL_REQUESTS" != "no"; then
	
		AC_MSG_CHECKING([for curl/curl.h])
		CURL_DIR=
		for i in "$PHP_HTTP_CURL_REQUESTS" /usr/local /usr /opt; do
			if test -r "$i/include/curl/curl.h"; then
				CURL_DIR=$i
				break
			fi
		done
		if test -z "$CURL_DIR"; then
			AC_MSG_RESULT([not found])
			AC_MSG_ERROR([could not find curl/curl.h])
		else
			AC_MSG_RESULT([found in $CURL_DIR])
		fi
		
		AC_MSG_CHECKING([for curl-config])
		CURL_CONFIG=
		for i in "$CURL_DIR/bin/curl-config" "$CURL_DIR/curl-config" `which curl-config`; do
			if test -x "$i"; then
				CURL_CONFIG=$i
				break
			fi
		done
		if test -z "$CURL_CONFIG"; then
			AC_MSG_RESULT([not found])
			AC_MSG_ERROR([could not find curl-config])
		else
			AC_MSG_RESULT([found: $CURL_CONFIG])
		fi
		
		CURL_SSL=`$CURL_CONFIG --features | $EGREP SSL`
		if test "$CURL_SSL" == "SSL"; then
			AC_DEFINE([HTTP_HAVE_SSL], [1], [ ])
		fi
		AC_CHECK_HEADERS([openssl/crypto.h])
		
		CURL_LIBS=`$CURL_CONFIG --libs`
		
		PHP_ADD_INCLUDE($CURL_DIR/include)
		PHP_ADD_LIBRARY_WITH_PATH(curl, $CURL_DIR/$PHP_LIBDIR, HTTP_SHARED_LIBADD)
		PHP_EVAL_LIBLINE($CURL_LIBS, HTTP_SHARED_LIBADD)
		AC_DEFINE([HTTP_HAVE_CURL], [1], [Have cURL support])
		
		PHP_CHECK_LIBRARY(curl, curl_multi_strerror, 
			[AC_DEFINE([HAVE_CURL_MULTI_STRERROR], [1], [ ])], [ ], 
			[$CURL_LIBS -L$CURL_DIR/$PHP_LIBDIR]
		)
		PHP_CHECK_LIBRARY(curl, curl_easy_strerror,
			[AC_DEFINE([HAVE_CURL_EASY_STRERROR], [1], [ ])], [ ],
			[$CURL_LIBS -L$CURL_DIR/$PHP_LIBDIR]
		)
		PHP_CHECK_LIBRARY(curl, curl_easy_reset,
			[AC_DEFINE([HAVE_CURL_EASY_RESET], [1], [ ])], [ ],
			[$CURL_LIBS -L$CURL_DIR/$PHP_LIBDIR]
		)
	fi

dnl ----
dnl MHASH
dnl ----
	if test "$PHP_HTTP_MHASH_ETAGS" != "no"; then
	
		AC_MSG_CHECKING([for mhash.h])
		MHASH_DIR=
		for i in "$PHP_HTTP_MHASH_ETAGS" /usr/local /usr /opt; do
			if test -f "$i/include/mhash.h"; then
				MHASH_DIR=$i
				break
			fi
		done
		if test -z "$MHASH_DIR"; then
			AC_MSG_RESULT([not found])
			AC_MSG_ERROR([could not find mhash.h])
		else
			AC_MSG_RESULT([found in $MHASH_DIR])
		fi
	
		PHP_ADD_INCLUDE($MHASH_DIR/include)
		PHP_ADD_LIBRARY_WITH_PATH(mhash, $MHASH_DIR/$PHP_LIBDIR, HTTP_SHARED_LIBADD)
		AC_DEFINE([HTTP_HAVE_MHASH], [1], [Have mhash support])
	fi

dnl ----
dnl MAGIC
dnl ----
	if test "$PHP_HTTP_MAGIC_MIME" != "no"; then
	
		AC_MSG_CHECKING([for magic.h])
		MAGIC_DIR=
		for i in "$PHP_HTTP_MAGIC_MIME" /usr/local /usr /opt; do
			if test -f "$i/include/magic.h"; then
				MAGIC_DIR=$i
				break
			fi
		done
		if test -z "$MAGIC_DIR"; then
			AC_MSG_RESULT([not found])
			AC_MSG_ERROR([could not find magic.h])
		else
			AC_MSG_RESULT([found in $MAGIC_DIR])
		fi
		
		PHP_ADD_INCLUDE($MAGIC_DIR/include)
		PHP_ADD_LIBRARY_WITH_PATH(magic, $MAGIC_DIR/$PHP_LIBDIR, HTTP_SHARED_LIBADD)
		AC_DEFINE([HTTP_HAVE_MAGIC], [1], [Have magic mime support])
	fi

dnl ----
dnl DONE
dnl ----
	PHP_HTTP_SOURCES="missing.c http.c http_functions.c phpstr/phpstr.c \
		http_util_object.c http_message_object.c http_request_object.c http_request_pool_api.c \
		http_response_object.c http_exception_object.c http_requestpool_object.c \
		http_api.c http_cache_api.c http_request_api.c http_date_api.c \
		http_headers_api.c http_message_api.c http_send_api.c http_url_api.c \
		http_info_api.c http_request_method_api.c"
	PHP_NEW_EXTENSION([http], $PHP_HTTP_SOURCES, [$ext_shared])
	PHP_ADD_BUILD_DIR($ext_builddir/phpstr, 1)
	PHP_SUBST([HTTP_SHARED_LIBADD])

	HTTP_HEADER_FILES="php_http_std_defs.h php_http.h php_http_api.h php_http_cache_api.h \
		php_http_date_api.h php_http_headers_api.h php_http_info_api.h php_http_message_api.h \
		php_http_request_api.h php_http_request_method_api.h php_http_send_api.h php_http_url_api.h"
	PHP_SUBST([HTTP_HEADER_FILES])

	ifdef([PHP_INSTALL_HEADERS],
	[
		PHP_INSTALL_HEADERS(ext/http, $HTTP_HEADER_FILES)
		PHP_INSTALL_HEADERS(ext/http/phpstr, [phpstr/phpstr.h])
	], [
		PHP_ADD_MAKEFILE_FRAGMENT
	])

	AC_DEFINE([HAVE_HTTP], [1], [Have extended HTTP support])
fi
