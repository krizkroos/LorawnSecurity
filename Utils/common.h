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
    static unsigned long int bytes2ULong(const bytes input, bool bigEndian =true);
    static Lorawan_result decodeBase64(bytes input, bytes &decoded);
    static Lorawan_result encodeBase64(bytes input, bytes &encoded);
    static Lorawan_result convertToBigEndian(bytes littleEndianValue, bytes &bigEndianValue);
    static Lorawan_result convertToLittleEndian(bytes bigEndianValue, bytes &littleEndianValue);

    static Lorawan_result testDecodingEncoding();
    static Lorawan_result testBigAndLittleEndian();
    static Lorawan_result convertIPAddress(const bytes address, bytes &convertedAddr);

    static bytes ulong2Bytes(unsigned long int value);
    static Lorawan_result calculate_cmac(bytes keyVal, bytes msgVal, bytes &cmac);
private:
    static size_t calcDecodeLength(const char *b64input);
};

#endif // COMMON_H
