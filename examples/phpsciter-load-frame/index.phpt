--TEST--
phpsciter load frame
--FILE--
<?php

var_dump($oSciter = new PHPSciter());
var_dump($oSciter->getVersion());
var_dump($oSciter->setResourcePath('file://' . __DIR__ . '/res/'));
var_dump($oSciter->setWindowTitle('hello'));
var_dump($oSciter->setWindowFrame(100, 100, 100 + 800 + 1, 100 + 600 + 1));
var_dump($oSciter->loadFile('default.htm'));

var_dump($oSciter->run());
?>
--EXPECT--
true