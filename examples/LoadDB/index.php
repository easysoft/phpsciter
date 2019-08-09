<?php

//include_once 'resource/mysql_pdo.php';

class TestSciter
{
    static public function getDataFromMySQL($limit)
    {
//        $sMysqlPdo = mysql_pdo::instance();
//        $data      = $sMysqlPdo->testQuery($limit);
//        var_dump($data);

        $data = array(
            array(
                'user_id'      => 1,
                'user_company' => 'aaa',
            ), array(
                'user_id'      => 2,
                'user_company' => 'bbb',
            ),
        );
//
//        $data = array('user_id','aaaa','user_company','cccc','dddd');

        return $data;
    }
}

$oSciter = PHPSciter::getInstance("default");
$oSciter->setResourcePath('file://' . __DIR__ . '/html/');
$oSciter->setWindowFrame(100, 100, 100 + 500 + 1, 100 + 500 + 1);
$oSciter->setWindowTitle('hello');
$oSciter->loadFile('index.html');

$defineResult = $oSciter->defineFunction('get_list', 'TestSciter::getDataFromMySQL');

var_dump($defineResult);
var_dump($oSciter->ifDefined('get_list'));


$oSciter->run();

