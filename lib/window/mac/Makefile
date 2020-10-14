# C/C++ compiler; to use Clang, build with `make CC=clang CXX=clang++`
CXX ?= g++
CC ?= gcc
CLANG ?= clang++

CP ?= cp
OD ?= od

OPT ?= -O3

CXXFLAGS ?= -g $(OPT) -Wall -Wextra -Woverloaded-virtual -pedantic -std=c++0x -fPIC -Iinclude -Ithird_party/md5
CFLAGS ?= -g $(OPT) -Wall -Wextra -pedantic -std=c99 -fPIC -Iinclude
MAKEDEPENDFLAGS ?= -Iinclude -Ithird_party/md5
LDFLAGS ?=

SHARED_LDFLAGS ?= -shared

# C binding.
php-window.o:
	make -p ../../../.libs
	g++ -std=c++11 -g -O2 -Wno-unused-function -Wno-deprecated -Wno-deprecated-declarations -DPIC -c php-window.mm -I../../../include -F Cocoa -o ../../../.libs/php-window.o


clean:
	rm -vf */*~ *~ .*~ */.*.swp .*.swp $(ALL) *.o

-include Makefile.depend