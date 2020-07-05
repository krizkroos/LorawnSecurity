#ifndef DEFINELORAWAN_H
#define DEFINELORAWAN_H

#include <vector>
#include <string>
#include <memory>
enum class Lorawan_result
{
    Success,
    Error,
    ErrorTest,
    WrongSniffingParameters,
    NoValueAvailable,
    NotSupportedFeature,
    ErrorOpenSSL,
    ErrorDecodingBase64,
    ErrorEncodingBase64,
    ErrorConvert2BigEndian,
    InputSizeZero,
    ErrorAddPacket
};



typedef unsigned char byte;
typedef std::vector<byte> bytes;
typedef std::vector<std::string> jsonKeys;

#define UNUSED(x) (void)(x)
#endif // DEFINELORAWAN_H

