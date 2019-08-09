dnl $Id$
dnl config.m4 for extension phpsciter

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(phpsciter, for phpsciter support,
dnl Make sure that the comment is aligned:
dnl [  --with-phpsciter             Include phpsciter support]) dnl指定了PHP扩展模块的工作方式

dnl Otherwise use enable:

PHP_ARG_ENABLE(phpsciter, whether to enable phpsciter support,
Make sure that the comment is aligned:
[  --enable-phpsciter           Enable phpsciter support]) dnl指定了PHP扩展模块的工作方式

if test "$PHP_PHPsciter" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-phpsciter -> add include path
  dnl PHP_ADD_INCLUDE($PHPsciter_DIR/include)

  dnl # --with-phpsciter -> check for lib and symbol presence
  dnl LIBNAME=phpsciter # you may want to change this
  dnl LIBSYMBOL=phpsciter # you most likely want to change this

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $PHPsciter_DIR/$PHP_LIBDIR, PHPsciter_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_PHPsciterLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong phpsciter lib version or lib not found])
  dnl ],[
  dnl   -L$PHPsciter_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(PHPsciter_SHARED_LIBADD)

  THIS_DIR=`dirname $0`
  MAKE_LIB_JSONNET=`cd ${THIS_DIR}/lib && make php-window.o`

  PHP_REQUIRE_CXX()
  PHP_SUBST(PHPSCITER_SHARED_LIBADD)
  PHP_ADD_LIBRARY(stdc++, 1, PHPSCITER_SHARED_LIBADD)
  PHP_ADD_LIBRARY(objc, 1, PHPSCITER_SHARED_LIBADD)

  dnl PHP_ADD_LIBRARY(libsciter-gtk-64, ./lib, PHPSCITER_SHARED_LIBADD)
  PHP_ADD_LIBRARY_WITH_PATH(sciter-osx-64, ./lib, PHPSCITER_SHARED_LIBADD)


  PHP_ADD_FRAMEWORK_WITH_PATH(Cocoa, /System/Library/Frameworks)
  # PHP_SUBST(PHP_FRAMEWORKS)
  # PHPSCITER_CFLAGS=-DKAFFE
  case $host in
    *darwin*)
      CXXFLAGS="$CXXFLAGS -Wno-unused-function -Wno-deprecated -Wno-deprecated-declarations -std=c++11"
    ;;
    *linux*)
      ;;
  esac

  source_file="lib/php-window.mm \
    src/callback.cpp \
    src/tool.cpp \
    src/value.cpp \
    phpsciter.cpp"

  PHP_ADD_INCLUDE(./include)
  PHP_ADD_BUILD_DIR($ext_builddir/lib)
  PHP_ADD_BUILD_DIR($ext_builddir/src)

  PHP_NEW_EXTENSION(phpsciter, $source_file, $ext_shared)

  PHP_ADD_MAKEFILE_FRAGMENT
fi
