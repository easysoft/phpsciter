<html>
<link href="main.css" rel="stylesheet" type="text/css"/>
<body>
    <toolbar #header-toolbar>
        <button #red title="service">服务</button>
        <button #red title="password">密码</button>
        <button #red title="log">日志</button>
        <button #red title="tool">工具</button>
        <button #red title="language">语言</button>
        <button #red title="help">帮助</button>
        <button #red title="exit">退出</button>

    </toolbar>

    <toolbar #button-collect>
        <button #red title="service">正在运行</button>
        <button onclick="" #red title="password">访问禅道</button>
        <button #red title="log">停止</button>
        <button #red title="tool">禅道官网</button>
        <button #red title="language">启动聊天</button>
    </toolbar>

    <textarea disabled="disabled">
        php版本: <?php echo PHP_VERSION . "\n"; ?>
        apache版本:
        mysql版本:
        amd64版本:
        禅道版本: 2.8
        当前目录: <?php echo __DIR__; ?>
    </textarea>

    <div style="margin-top: 30px">
        <input type="checkbox"> 启用apache访问用户验证
    </div>
</body>

