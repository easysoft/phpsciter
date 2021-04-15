//
// Created by zhanglei on 2020/6/9.
//

#ifndef PHPSCITER_SCITER_ZEND_HOOK_H
#define PHPSCITER_SCITER_ZEND_HOOK_H

namespace phpsciter{
    class SciterZendHook{

    public:
        SciterZendHook()
        {
//            PHPSCITER_G(origin_zend_compile_file) = zend_compile_file;
//            PHPSCITER_G(phpsciter_compile_file) = phpsciter::ZendApi::zendCompileFile;
            PHPSCITER_G(write_function) = zend_write;
//            zend_compile_file = PHPSCITER_G(phpsciter_compile_file);
            zend_write = phpsciter::ZendApi::sciterWrite;
        }

        ~SciterZendHook()
        {
//            zend_compile_file = PHPSCITER_G(origin_zend_compile_file);
            zend_write = PHPSCITER_G(write_function);
            clearCallBack();
        }
    };
}

#endif //PHPSCITER_SCITER_ZEND_HOOK_H
