--TEST--
http_send_data() syntactically invalid range
--SKIPIF--
<?php 
include 'skip.inc';
?>
--ENV--
HTTP_RANGE=bytes=123,-wtf ?
--FILE--
<?php
http_send_content_type('text/plain');
http_send_data(str_repeat('123abc', 1000));
?>
--EXPECTF--
X-Powered-By: PHP/%s
Content-Type: text/plain
Accept-Ranges: bytes

123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc123abc