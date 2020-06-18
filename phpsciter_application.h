/*
  +----------------------------------------------------------------------+
  | PHPSciter                                                            |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Chitao Gao  <neeke@php.net>                                  |
  +----------------------------------------------------------------------+
*/

#ifndef PHPSCITER_PHPSCITER_APPLICATIOON_H
#define PHPSCITER_PHPSCITER_APPLICATIOON_H

#include "sciter-common.h"
//
// Created by root on 20-3-7.
//
PHP_METHOD(phpsciter, __construct);
PHP_METHOD(phpsciter, __destruct);
PHP_METHOD(phpsciter, __sleep);
PHP_METHOD(phpsciter, __wakeup);
PHP_METHOD(phpsciter, __clone);
PHP_METHOD(phpsciter, getInstance);
PHP_METHOD(phpsciter, getVersion);
PHP_METHOD(phpsciter, getPHPSciterVersion);
PHP_METHOD(phpsciter, setResourcePath);
PHP_METHOD(phpsciter, setWindowFrame);
PHP_METHOD(phpsciter, setWindowTitle);
PHP_METHOD(phpsciter, setOption);
PHP_METHOD(phpsciter, loadFile);
PHP_METHOD(phpsciter, loadPHP);
PHP_METHOD(phpsciter, loadHtml);
PHP_METHOD(phpsciter, defineFunction);
PHP_METHOD(phpsciter, ifDefined);
PHP_METHOD(phpsciter, run);

ZEND_BEGIN_ARG_INFO_EX(phpsciter_setResourcePath_arginfo, 0, 0, 1)
ZEND_ARG_INFO(0, resource_path)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(phpsciter_setWindowFrame_arginfo, 0, 0, 4)
ZEND_ARG_INFO(0, frame_top)
ZEND_ARG_INFO(0, frame_left)
ZEND_ARG_INFO(0, frame_right)
ZEND_ARG_INFO(0, frame_bottom)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(phpsciter_setWindowTitle_arginfo, 0, 0, 1)
ZEND_ARG_INFO(0, title)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(phpsciter_loadFile_arginfo, 0, 0, 1)
ZEND_ARG_INFO(0, file_name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(phpsciter_loadPHP_arginfo, 0, 0, 1)
ZEND_ARG_INFO(0, php)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(phpsciter_loadHtml_arginfo, 0, 0, 1)
ZEND_ARG_INFO(0, html)
ZEND_END_ARG_INFO()


ZEND_BEGIN_ARG_INFO_EX(phpsciter_defineFunction_arginfo, 0, 0, 2)
ZEND_ARG_INFO(0, event_name)
ZEND_ARG_INFO(0, function_name)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO_EX(phpsciter_ifDefined_arginfo, 0, 0, 1)
ZEND_ARG_INFO(0, event_name)
ZEND_END_ARG_INFO()

enum {
    LOAD_EMPTY,
    LOAD_HTML,
    LOAD_PHP,
    LOAD_HTML_FILE,
    LOAD_PHP_FILE
};

void load_phpsciter_application();

#endif //PHPSCITER_PHPSCITER_APPLICATIOON_H


