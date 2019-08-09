@ECHO OFF

SET CURRENT=%~dp0%
SET BUILD_COMMAND=g++
SET BUILD_OPTION=-Wno-unused-function -Wno-deprecated -Wno-deprecated-declarations -mwindows

pkg-config --cflags gtk+-2.0 > tmp_GTK_CFLAGS.txt
set /p "GTK_CFLAGS="<tmp_GTK_CFLAGS.txt
del tmp_GTK_CFLAGS.txt

pkg-config --libs gtk+-2.0 > tmp_GTK_LIBS.txt
set /p "GTK_LIBS="<tmp_GTK_LIBS.txt
del tmp_GTK_LIBS.txt

if "%1"=="build" goto build
goto eof
:build
	%BUILD_COMMAND% %BUILD_OPTION% %GTK_CFLAGS% -I../include -g -O2 -Wall -DPIC -o ../.libs/php-window.o -c php-window.cpp %GTK_LIBS%
    goto eof
:eof

@ECHO ON
