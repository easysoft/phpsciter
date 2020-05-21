--TEST--
phpsciter layered
--FILE--
<?php

$oSciter = new PHPSciter();
var_dump($oSciter->getVersion());
$oSciter->setResourcePath('file://' . __DIR__ . '/res/');
$oSciter->setWindowFrame(100, 100, 100 + 500 + 1, 100 + 500 + 1);
$oSciter->setWindowTitle('hello');
$oSciter->loadFile('default.htm');

$oSciter->run();
?>
--EXPECT--
true