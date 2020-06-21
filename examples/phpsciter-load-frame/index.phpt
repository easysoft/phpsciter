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

$oSciter->setOption(PHPSciter::SCITER_SET_SCRIPT_RUNTIME_FEATURES,
PHPSciter::ALLOW_FILE_IO | PHPSciter::ALLOW_SOCKET_IO | PHPSciter::ALLOW_EVAL |
                               PHPSciter::ALLOW_SYSINFO);


$oSciter->run(PHPSciter::SW_TITLEBAR | PHPSciter::SW_RESIZEABLE | PHPSciter::SW_MAIN | PHPSciter::SW_ENABLE_DEBUG
|PHPSciter::SW_CONTROLS);
?>
--EXPECT--
true