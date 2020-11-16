<?php

class TestOutPutHtml
{
    public function contact($str_1, $str_2)
    {
        return $str_1 . ' -- ' . $str_2;
    }

    public function procHtml($str)
    {
        return '<html><meta http-equiv=Content-Type content="text/html;charset=utf-8"><body><dev>' . $str . '</dev></body></html>';
    }
}


