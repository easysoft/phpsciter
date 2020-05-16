//
// Created by zhanglei on 2020/5/16.
//

#ifndef PHPSCITER_SCITER_NOCOPY_H
#define PHPSCITER_SCITER_NOCOPY_H
class NoCopy{
public:
    NoCopy() = default;
    NoCopy(const NoCopy&) = delete;

protected:
    NoCopy &operator= (const NoCopy&) = delete;
    ~NoCopy() = default;
};
#endif //PHPSCITER_SCITER_NOCOPY_H
