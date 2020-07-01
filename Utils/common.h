#ifndef COMMON_H
#define COMMON_H

#include "defineLorawan.h"
#include <string>

class Common
{
public:
    Common();
    static std::string bytes2Str(bytes input);
    static std::string bytes2HexStr(bytes input, bool withSpace);
    static long decodeBase64(bytes input, bytes decoded);
};

#endif // COMMON_H
