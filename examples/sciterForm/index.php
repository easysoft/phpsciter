<?php

$oSciter = PHPSciter::getInstance("default");
$oSciter->setResourcePath('file://' . __DIR__ . '/res/');
$oSciter->setWindowFrame(100, 100, 100, 100);
$oSciter->setWindowTitle('hello');
$oSciter->loadFile('index2.php');

class classTest
{
    static public function plus()
    {
        $_GET["data"] = "aaa";
        return $a + 30;
    }

    static public function sum($a, $b, $c)
    {
        return $a + $b + $c;
    }

    static public function float_($float)
    {

        return 33.44;
    }

    static public function contact($str_1, $str_2)
    {
        return $str_1 . '-' . $str_2;
    }

    static public function contact_chinese($str_1, $str_2)
    {
        return $str_1 . '-' . $str_2;
    }

    static public function response_chinese()
    {
        return '我是汉字，你好世界';
    }

    static public function null_($null)
    {

        return null;
    }

    static public function bool_false($false)
    {

        return false;
    }

    static public function bool_true($true)
    {

        return true;
    }

    static public function arrmap($array, $str_1, $str_2, $num_1, $num_2)
    {
        // var_dump(is_array($array),$array, $str_1, $str_2, $num_1, $num_2);
        $str = self::contact($str_1, $str_2);
        $num = self::sum($num_1, $num_2, 300);

        return array(
            $str,
            'str'           => $str,
            'num'           => $num,
            'this_is_array' => array(
                'key_1'             => 'this is string a',
                'key_2'             => 100,
                'this_is_array_too' => array(
                    'key_3' => 'this is string b',
                    'key_4' => null,
                ),
            ),
            300,
            400             => 'this is string',
            'key_5'         => null,
        );
    }
}


$defineResult = $oSciter->defineFunction('plus', 'classTest::plus');
$defineResult = $oSciter->defineFunction('sum', 'classTest::sum');
$defineResult = $oSciter->defineFunction('float', 'classTest::float_');
$defineResult = $oSciter->defineFunction('contact', 'classTest::contact');
$defineResult = $oSciter->defineFunction('contact_chinese', 'classTest::contact_chinese');
$defineResult = $oSciter->defineFunction('response_chinese', 'classTest::response_chinese');
$defineResult = $oSciter->defineFunction('nul', 'classTest::null_');
$defineResult = $oSciter->defineFunction('bool_true', 'classTest::bool_true');
$defineResult = $oSciter->defineFunction('bool_false', 'classTest::bool_false');
$defineResult = $oSciter->defineFunction('arrmap', 'classTest::arrmap');




$oSciter->run();
?>
