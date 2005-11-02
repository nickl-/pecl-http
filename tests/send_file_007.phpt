--TEST--
http_send_file() oversized range
--SKIPIF--
<?php 
include 'skip.inc';
checkcgi();
checkmax(5.0);
?>
--ENV--
HTTP_RANGE=bytes=-1111
--FILE--
<?php
http_send_file('data.txt');
?>
--EXPECTF--
Status: 416
Content-type: %s
X-Powered-By: PHP/%s
