<?php

class A{
    public function __destruct()
    {
        echo "__destruct\n";
    }
}

$data = new A();

static $static_var = 333;

$tem_data = 4;

function test()
{
}

register_shutdown_function(function () {
    echo "function shutdown\n";
});
?>
<a href="index_second.php">return1</a>

