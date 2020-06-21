<?php
 include_once "callback.php";
 ?>
<html>
<meta http-equiv=Content-Type content="text/html;charset=utf-8">
<div>
    Request & Get Int:
    <button id="plus_btn">Click Plus</button>
    <button id="sum_btn">Click Sum</button>
    <br/>

    Request & Get Float:
    <button id="float_btn">Click Float</button>
    <br/>

    Request & Get String:
    <button id="contact_btn">Click Contact</button>
    <button id="contact_abc_btn">Click ABC Contact</button>
    <button id="chinese_btn">Click 汉字参数与响应</button>
    <button id="response_chinese_btn">Click 响应汉字</button>
    <br/>

    Request & Get NULL:
    <button id="null_btn">Click Null</button>
    <br/>

    Request & Get Bool:
    <button id="bool_true_btn">Click Bool True</button>
    <button id="bool_false_btn">Click Bool Flase</button>
    <br/>

    Request & Get Array:
    <button id="array_btn">Click Array</button>
    <br/>

    Request & Get Map:
    <button id="map_btn">Cliek Map</button>
    <br/>
</div>
<br /><hr />
<div id="data_json"> </div>
<br /><hr />
<div id="data_area">

</div>
<script type="text/tiscript">
$(#plus_btn).on("click", function() {
    var plus = view.plus(100);
    $(#data_json).html = JSON.stringify(plus);
    $(#data_area).html = "";
});

$(#sum_btn).on("click", function() {
    var sum = view.sum(1,2,3);
    $(#data_json).html = JSON.stringify(sum);
    $(#data_area).html = "";
});

$(#float_btn).on("click", function() {
    var float = view.float(11.22);
    $(#data_json).html = JSON.stringify(float);
    $(#data_area).html = "";
});

$(#contact_btn).on("click", function() {
    var str = view.contact("test汉字","neeke");
    $(#data_json).html = JSON.stringify(str);
    $(#data_area).html = str;
});

$(#contact_abc_btn).on("click", function() {
    var str = view.contact("aaa","bbb");
    $(#data_json).html = JSON.stringify(str);
    $(#data_area).html = str;
});

$(#chinese_btn).on("click", function() {
    var str = view.contact_chinese("我是汉字","你好世界");
    $(#data_json).html = JSON.stringify(str);
    $(#data_area).html = str;
});

$(#response_chinese_btn).on("click", function() {
    var str = view.response_chinese();
    $(#data_json).html = JSON.stringify(str);
    $(#data_area).html = str;
});

$(#null_btn).on("click", function() {
    var nul = view.nul(null);
    var str = "get null faild";
    if (nul == null) {
        str = "get null success";
    }
    $(#data_json).html = "";
    $(#data_area).html = str;
});

$(#bool_true_btn).on("click", function() {
    var bol = view.bool_true(true);
    var str = "get true faild";
    if (bol == true) {
        str = "get true success";
    }

    $(#data_json).html = JSON.stringify(bol);
    $(#data_area).html = str;
});

$(#bool_false_btn).on("click", function() {
    var bol = view.bool_false(false);
    var str = "get flase faild";
    if (bol == false) {
        str = "get flase success";
    }
    $(#data_json).html = JSON.stringify(bol);
    $(#data_area).html = str;
});

$(#array_btn).on("click", function() {
    var alphas = [1111,2222,3333,4444];

    var arr = view.arrmap(alphas,"test","neeke",100,200);

    var result = "";
    result += "arr.length -> " + arr.length + "<br />";

    var i = 0;
    for(var key in arr) {
        i++;
        result += " i: " + i;
        result += " key -> " + key + " val -> " + JSON.stringify(arr[key]) + "<br />";
    }

    $(#data_json).html = JSON.stringify(arr);
    $(#data_area).html = result;
});

$(#map_btn).on("click", function() {
    //var map_ = {"a","b",110,119};
    var map_ = {"aaaa":123,"bbbb":"cccc","d":"110","e":null};
    var map = view.arrmap(map_,"test","neeke",100,200);

    var result = "";
    result += "map.length -> " + map.length + "<br />";

    var i = 0;
    for(var key in map) {
        i++;
        result += " i: " + i;
        result += " key -> " + key + " val -> " + JSON.stringify(map[key]) + "<br />";
    }

    $(#data_json).html = JSON.stringify(map);
    $(#data_area).html = result;

    //view.msgbox(#information, "map.this_is_array.this_is_array_too.key_3 -> " + map.this_is_array.this_is_array_too.key_3);
});


</script>
</html>