--TEST--
phpsciter load html
--FILE--
<?php

include_once 'res/OutPut.php';

$oSciter = new PHPSciter();
$oSciter->setResourcePath('file://' . __DIR__ . '/res/');

$oSciter->setWindowFrame(100, 100, 100 + 500 + 1, 100 + 500 + 1);

$oSciter->setWindowTitle('hello');

$oTestOutPut = new TestOutPutHtml();

//Test 1
// ob_start();
// echo $oTestOutPut->contact('neeke test output','hello world.');
// $output = ob_get_contents();
// ob_end_clean();
// $oSciter->loadHtml('<html><body><dev>'. $output .'</dev></body></html>');

//Test 2;
$oSciter->loadHtml($oTestOutPut->procHtml($oTestOutPut->contact('neeke test output case 2.', 'hello world. 汉字')));

$oSciter->setOption(PHPSciter::SCITER_SET_SCRIPT_RUNTIME_FEATURES,
PHPSciter::ALLOW_FILE_IO | PHPSciter::ALLOW_SOCKET_IO | PHPSciter::ALLOW_EVAL |
                               PHPSciter::ALLOW_SYSINFO);


$oSciter->run(PHPSciter::SW_TITLEBAR | PHPSciter::SW_RESIZEABLE | PHPSciter::SW_MAIN | PHPSciter::SW_ENABLE_DEBUG
|PHPSciter::SW_CONTROLS);

?>
--EXPECT--
true