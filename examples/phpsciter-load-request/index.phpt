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

define("TEST","test");
$oSciter->setOption(PHPSciter::SCITER_SET_SCRIPT_RUNTIME_FEATURES,
PHPSciter::ALLOW_FILE_IO | PHPSciter::ALLOW_SOCKET_IO | PHPSciter::ALLOW_EVAL |
                               PHPSciter::ALLOW_SYSINFO);


$r = $oSciter->run(PHPSciter::SW_TITLEBAR | PHPSciter::SW_RESIZEABLE | PHPSciter::SW_MAIN | PHPSciter::SW_ENABLE_DEBUG
|PHPSciter::SW_CONTROLS);
var_dump($r);
?>
--EXPECT--
true