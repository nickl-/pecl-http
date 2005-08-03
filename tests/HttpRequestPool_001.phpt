--TEST--
HttpRequestPool
--SKIPIF--
<?php
include 'skip.inc';
checkver(5);
checkcls('HttpRequestPool');
checkurl('www.php.net');
checkurl('pear.php.net');
checkurl('pecl.php.net');
?>
--FILE--
<?php
echo "-TEST\n";
$pool = new HttpRequestPool(
    new HttpRequest('http://www.php.net/', HTTP_HEAD),
    new HttpRequest('http://pear.php.net/', HTTP_HEAD),
    new HttpRequest('http://pecl.php.net/', HTTP_HEAD)
);
$pool->send();
foreach ($pool as $req) {
    echo $req->getUrl(), '=',
        $req->getResponseCode(), ':',
        $req->getResponseMessage()->getResponseCode(), "\n";
}
echo "Done\n";
?>
--EXPECTF--
%sTEST
http://www.php.net/=200:200
http://pear.php.net/=200:200
http://pecl.php.net/=200:200
Done
