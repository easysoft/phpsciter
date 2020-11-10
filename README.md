中文 | [英文](./README-EN.md)

## 关于PHPSciter
[Sciter](http://www.sciter.com)是一个非常优秀的的跨平台GUI框架，它使用HTML5 + CSS来绘制界面，使用TIScritpt来做交互。 PHPSciter是一个PHP绑定Sciter的扩展。PHP程序员可以使用这个扩展来开发跨平台的桌面应用，并且可以重用之前B/S架构下的开发经验。

## 如何安装PHPSciter
PHPSciter扩展运行的时候依赖Sciter的库。Sciter为不同的平台提供了不同的动态库文件，Windows下面是sciter.dll，Linux下面是libsciter-gtk.so，OSX下面是libsciter-osx-64.dylib。Sciter动态库的安装有两种方式，一种是直接将其放在PHP的可执行目录下面，一种是安装到系统路径中。




* Windows: 
      - 仅仅只需要简单拷贝 `lib\sciter\win\sciter.dll` 到 `c:\windows\system32`
    * Linux: 
      - `cd lib/sciter/linux`
      - `tar zxvf libsciter-gtk-lnx.tar.gz`
      - `echo $PWD >> libsciter.conf`
      - `sudo cp libsciter.conf /etc/ld.so.conf.d/`
      - `sudo ldconfig`
      - `ldconfig -p | grep sciter` should print libsciter-gtk-64.so location
    * OSX:
      - `cd lib/sciter/mac`
      - `export DYLD_LIBRARY_PATH=$PWD`
      
#### Windows 

- 将phpsciter.dll加入到php.ini中

```
php examples/phpsciter-load-frame/index.phpt
```

#### Ubuntu 16.04
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

#### Mac OS
```
git clone https://github.com/longyan/phpsciter.git
cd phpsciter
phpize
./configure
make
make install

php examples/phpsciter-load-frame/index.phpt
```

#### 注意了!

 - 如果你在mac或者linux下面调用了make clean，必须要重新使用./configure后在继续make && make install
 
#### Sciter调试

 - 如何进行Sciter软件方面的调试？
 
 #### Windows 

 - 需要将sciter-sdk下面的inspector.exe 拷贝到 与php.exe 同级目录下，点击Dom inspector
 
  ![](./cover/sciter-inspector.jpg)

 #### Linux 和 Mac
 
  - 需要将 inspector 放置到和php可执行文件的同一级别目录下，就可以使用调试工具了
 
