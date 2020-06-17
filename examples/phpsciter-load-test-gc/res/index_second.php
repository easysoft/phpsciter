<?php
class A{
    public function __destruct()
    {
        echo "__destruct\n";
    }
}
$first = new A();
var_dump($first);

var_dump($static_var);

var_dump($tem_data);

register_shutdown_function(function () {
    echo "function shutdown\n";
});

?>
<a href="index.php">return2</a>