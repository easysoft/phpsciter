//
// Created by zhanglei on 2020/6/9.
//

#ifndef PHPSCITER_SCITER_SYMBOL_TABLE_GUARD_H
#define PHPSCITER_SCITER_SYMBOL_TABLE_GUARD_H

namespace phpsciter{
    class ZendSymbolTableGuard{

    public:
        ZendSymbolTableGuard()
        {
            PHPSCITER_G(storage_symbol_table) = zend_array_dup(&EG(symbol_table));
        }

        ~ZendSymbolTableGuard()
        {
            zend_array_destroy(PHPSCITER_G(storage_symbol_table));
            PHPSCITER_G(storage_symbol_table) = nullptr;
        }
    };
}

#endif //PHPSCITER_SCITER_SYMBOL_TABLE_GUARD_H
