<?php
//创建一个sciter的实例
$oSciter = new PHPSciter();
//设置资源加载路径
$oSciter->setResourcePath('file://' . __DIR__ . '/res/');
//设置sciter的窗口大小
$oSciter->setWindowFrame(100, 100, 100 + 500 + 1, 100 + 500 + 1);
//设置sciter的标题为hello
$oSciter->setWindowTitle('hello');
//设置资源目录下加载的入口文件
$oSciter->loadFile('helloword.php');

//设置PHPSCITER的选项
$oSciter->setOption(PHPSciter::SCITER_SET_SCRIPT_RUNTIME_FEATURES,
PHPSciter::ALLOW_FILE_IO | PHPSciter::ALLOW_SOCKET_IO | PHPSciter::ALLOW_EVAL |
                               PHPSciter::ALLOW_SYSINFO);

//运行dialog窗体
$res = $oSciter->run(PHPSciter::SW_TITLEBAR | PHPSciter::SW_RESIZEABLE | PHPSciter::SW_MAIN | PHPSciter::SW_ENABLE_DEBUG
|PHPSciter::SW_CONTROLS);
?>
