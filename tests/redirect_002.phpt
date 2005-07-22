--TEST--
http_redirect() with session
--SKIPIF--
<?php 
include 'skip.inc';
checkcgi();
?>
--FILE--
<?php
session_start();
http_redirect('redirect', array('a' => 1), true);
?>
--EXPECTF--
Status: 302
Content-type: text/html
X-Powered-By: PHP/%s
Set-Cookie: PHPSESSID=%s; path=/
Expires: %s
Cache-Control: %s
Pragma: %s
Location: http://localhost/redirect?a=1&PHPSESSID=%s
