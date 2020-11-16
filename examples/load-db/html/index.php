<?php
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
$defineResult = PHPSciter::defineFunction('get_list', 'TestSciter::getDataFromMySQL');
include_once "index.html";