//
// Created by root on 20-3-7.
//

#ifndef PHPSCITER_PHPSCITER_GLOBAL_H
#define PHPSCITER_PHPSCITER_GLOBAL_H

typedef struct _zend_phpsciter_globals {
    uint8_t loadModal;
    zend_bool loadFile;
    zend_bool loadHtml;
    char *resource_base_path = nullptr;
    char *default_title = nullptr;
    zend_op_array *current_op_array = nullptr;
#if PHP_VERSION_ID >= 70000
    int (*write_function)(const char *str, size_t str_length);
#else
    int (*write_function)(const char *str, uint str_length);
#endif
    std::shared_ptr<phpsciter::Util> tool;
    std::shared_ptr<phpsciter::ZendApi> zend;
    std::shared_ptr<phpsciter::ZendSciterRequest> request;
    std::string output_buffer;
} zend_phpsciter_globals;

extern ZEND_DECLARE_MODULE_GLOBALS(phpsciter)
#ifdef ZTS
# define PHPSCITER_G(v) TSRMG(phpsciter_globals_id, zend_phpsciter_globals *, v)
#else
# define PHPSCITER_G(v) (phpsciter_globals.v)
#endif

#endif //PHPSCITER_PHPSCITER_GLOBAL_H
