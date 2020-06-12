--TEST--
phpsciter load form
--FILE--
<?php
var_dump(realpath('file://' . __DIR__ . '/res/'));die;
$oSciter = new PHPSciter();
$oSciter->setResourcePath('file://' . __DIR__ . '/res/');
$oSciter->setWindowFrame(100, 100, 100, 100);
$oSciter->setWindowTitle('hello');
$oSciter->loadFile('index2.php');

$oSciter->run();
?>
--EXPECT--
true