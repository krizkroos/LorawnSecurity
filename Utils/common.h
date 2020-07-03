#ifndef COMMON_H
#define COMMON_H

#include "defineLorawan.h"
#include <string>

class Common
{
public:
    Common();
    static std::string bytes2Str(bytes input);
    static std::string bytes2HexStr(bytes input, bool withSpace=true);
    static bytes str2Bytes(std::string input);
    static Lorawan_result decodeBase64(bytes input, bytes &decoded);
    static Lorawan_result encodeBase64(bytes input, bytes &encoded);
private:
    static size_t calcDecodeLength(const char *b64input);
};

#endif // COMMON_H
