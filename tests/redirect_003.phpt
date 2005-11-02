--TEST--
http_redirect() permanent
--SKIPIF--
<?php 
include 'skip.inc';
checkcgi();
checkmax(5.0);
?>
--ENV--
HTTP_HOST=localhost
--FILE--
<?php
include 'log.inc';
log_prepare(_REDIR_LOG);
http_redirect('redirect', null, false, HTTP_REDIRECT_PERM);
?>
--EXPECTF--
Status: 301
Content-type: %s
X-Powered-By: PHP/%s
Location: http://localhost/redirect

Redirecting to <a href="http://localhost/redirect">http://localhost/redirect</a>.

