<?php

class classTest
{
    static public function plus($a)
    {
        return $a + 30;
    }

    static public function sum($a, $b, $c)
    {
        return $a + $b + $c;
    }

    static public function float_($float)
    {
        var_dump(is_float($float), $float);

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
        var_dump($null == null);

        return null;
    }

    static public function bool_false($false)
    {
        var_dump($false == false);

        return false;
    }

    static public function bool_true($true)
    {
        var_dump($true == true);

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

PHPSciter::defineFunction('plus', 'classTest::plus');
PHPSciter::defineFunction('sum', 'classTest::sum');
PHPSciter::defineFunction('float', 'classTest::float_');
PHPSciter::defineFunction('contact', 'classTest::contact');
PHPSciter::defineFunction('contact_chinese', 'classTest::contact_chinese');
PHPSciter::defineFunction('response_chinese', 'classTest::response_chinese');
PHPSciter::defineFunction('nul', 'classTest::null_');
PHPSciter::defineFunction('bool_true', 'classTest::bool_true');
PHPSciter::defineFunction('bool_false', 'classTest::bool_false');
PHPSciter::defineFunction('arrmap', 'classTest::arrmap');


?>