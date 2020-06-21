--TEST--
phpsciter load mysql
--FILE--
<?php

//include_once 'resource/mysql_pdo.php';


$oSciter = new PHPSciter();
$oSciter->setResourcePath('file://' . __DIR__ . '/html/');
$oSciter->setWindowFrame(100, 100, 100 + 500 + 1, 100 + 500 + 1);
$oSciter->setWindowTitle('hello');
$oSciter->loadFile('index.php');



$oSciter->setOption(PHPSciter::SCITER_SET_SCRIPT_RUNTIME_FEATURES,
PHPSciter::ALLOW_FILE_IO | PHPSciter::ALLOW_SOCKET_IO | PHPSciter::ALLOW_EVAL |
                               PHPSciter::ALLOW_SYSINFO);


$oSciter->run(PHPSciter::SW_TITLEBAR | PHPSciter::SW_RESIZEABLE | PHPSciter::SW_MAIN | PHPSciter::SW_ENABLE_DEBUG
|PHPSciter::SW_CONTROLS);
?>
--EXPECT--
true