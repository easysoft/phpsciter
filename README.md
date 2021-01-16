中文 | [English](./README-EN.md)

## 关于PHPSciter
[Sciter](http://www.sciter.com)是一个非常优秀的的跨平台GUI框架，它使用HTML5 + CSS来绘制界面，使用TIScritpt来做交互。 PHPSciter是一个PHP绑定Sciter的扩展。PHP程序员可以使用这个扩展来开发跨平台的桌面应用，并且可以重用之前B/S架构下的开发经验。

Sciter由Andrew发起。这是一位非常资深的UI专家，以邀请专家的身份参与HTML5标准的制定，Evernote最早的三个开发者之一。Sciter主要的特点：
- 跨平台：Sciter支持Windows、Linux和MacOS
- 轻量级：Sciter整体的实现非常轻，只有几兆大小
- 使用Html5实现：Sciter使用HTML，CSS来渲染窗口，使用TIScript做交互。表现力强，上手简单。
![](https://sciter.com/wp-content/uploads/2015/10/schema1.png)

Sciter作者正在开发[Sciter-JS](https://github.com/c-smile/sciter-js-sdk "Sciter-JS")项目，这个项目集成了quickjs。这个项目发布以后很快得到了很多人的关注，相信会吸引更多的人使用Sciter项目。

## 如何安装PHPSciter
PHPSciter扩展运行的时候依赖Sciter的库。Sciter为不同的平台提供了不同的动态库文件，Windows下面是sciter.dll，Linux下面是libsciter-gtk.so，OSX下面是libsciter-osx-64.dylib。Sciter动态库的安装有两种方式，一种是直接将其放在PHP的可执行目录下面，一种是安装到系统路径中。

Sciter官方推荐是将动态库直接安装到php可执行路径中即可。这样方便交付，也不会和其他的项目产生冲突。

### Ubuntu 16.04下面手工编译
```
apt-get install build-essential pkg-config
apt install libgtk-3-dev

git clone https://github.com/longyan/phpsciter.git
cd phpsciter
phpize
./configure
make
make install

php examples/phpsciter-load-frame/index.phpt
```

### MacOS 下面手工编译
```
git clone https://github.com/longyan/phpsciter.git
cd phpsciter
phpize
./configure
make
make install

php examples/phpsciter-load-frame/index.phpt
```

#### 注意事项

1.如果你在mac或者linux下面调用了make clean，必须要重新使用./configure后在继续make && make install

2.出现找不到VCRUNTIME140.dll 的运行代码应该如何解决？

 - 请到，https://support.microsoft.com/zh-cn/help/2977003/the-latest-supported-visual-c-downloads 去下载相对应的  vc_redist.exe，
就可以解决这个问题
 
#### 如何调试
- Windows：需要将sciter-sdk下面的inspector.exe 拷贝到 与php.exe 同级目录下，点击Dom inspector
- Linux 和 Mac 需要将 inspector 放置到和php可执行文件的同一级别目录下，就可以使用调试工具了
