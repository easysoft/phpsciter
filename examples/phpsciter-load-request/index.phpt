--TEST--
phpsciter load form
--FILE--
<?php
$oSciter = new PHPSciter();
$oSciter->setResourcePath('file://' . __DIR__ . '/res/');
$oSciter->setWindowFrame(100, 100, 100, 100);
$oSciter->setWindowTitle('hello');
$oSciter->loadFile('index2.php');

function A()
{
}

function B()
{
}

function C()
{
}

function D()
{
}

$r = $oSciter->run();
var_dump($r);
?>
--EXPECT--
true