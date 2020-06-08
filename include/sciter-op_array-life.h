//
// Created by zhanglei on 2020/5/16.
//

#ifndef PHPSCITER_SCITER_OP_ARRAY_LIFE_H
#define PHPSCITER_SCITER_OP_ARRAY_LIFE_H


namespace phpsciter{

    class OpArrayCriticalSection :public NoCopy{
    public:
        OpArrayCriticalSection();
        ~OpArrayCriticalSection();
    };

}

#endif //PHPSCITER_SCITER_OP_ARRAY_LIFE_H
