--TEST--
phpsciter load form
--FILE--
<?php

$oSciter = new PHPSciter();
$oSciter->setResourcePath('file://' . __DIR__ . '/res/');
$oSciter->setWindowFrame(100, 100, 100, 100);
$oSciter->setWindowTitle('hello');
$oSciter->loadFile('index.php');

$oSciter->run();
?>
--EXPECT--
true