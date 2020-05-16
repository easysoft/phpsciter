//
// Created by zhanglei on 2020/5/16.
//

#ifndef PHPSCITER_SCITER_OP_RARRAY_LIFE_H
#define PHPSCITER_SCITER_OP_RARRAY_LIFE_H

#include "sciter-common.h"
#include "phpsciter_global.h"

namespace phpsciter{

    class OpArrayCriticalSection :public NoCopy{
    public:
        OpArrayCriticalSection() = default;
        ~OpArrayCriticalSection()
        {
            if(PHPSCITER_G(cureent_op_array))
            {
                efree(PHPSCITER_G(cureent_op_array));
            }
            PHPSCITER_G(cureent_op_array) = nullptr;
        }
    };

}

#endif //PHPSCITER_SCITER_OP_RARRAY_LIFE_H
