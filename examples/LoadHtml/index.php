<?php

include_once 'res/OutPut.php';

$oSciter = PHPSciter::getInstance("default");
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

$oSciter->run();

?>