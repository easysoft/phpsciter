<?php

var_dump($oSciter = phpsciter::getInstance("default"));
var_dump($oSciter->getVersion());
var_dump($oSciter->setResourcePath('file://'.__DIR__.'/examples/sciter_frame/res/'));
// var_dump($oSciter->setWindowFrame(100,100,100+500+1,100+500+1));
var_dump($oSciter->setWindowTitle('hello'));
// var_dump($oSciter->setIndexPage('demo1/default.htm'));

var_dump($oSciter->setWindowFrame(100,100,100+800+1,100+600+1));
var_dump($oSciter->setIndexPage('default.htm'));

var_dump($oSciter->run());
?>
