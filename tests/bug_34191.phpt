--TEST--
bug #34191
--SKIPIF--
<?php
include 'skip.inc';
checkver(5.1);
checkcgi();
checkext('zlib');
?>
--ENV--
HTTP_ACCEPT_ENCODING=gzip
--FILE--
<?php
HttpResponse::setGzip(1);
HttpResponse::capture();
echo "foo\n";
?>
--EXPECTF--
X-Powered-By: PHP/%s
Content-Type: text/html
Accept-Ranges: bytes
Content-Encoding: gzip
Vary: Accept-Encoding

�      J���   �� �e2~