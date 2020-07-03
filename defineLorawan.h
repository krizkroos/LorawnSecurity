#ifndef DEFINELORAWAN_H
#define DEFINELORAWAN_H

#include <vector>
#include <string>
#include <memory>
enum class Lorawan_result
{
    Success,
    Error,
    WrongSniffingParameters,
    NoValueAvailable,
    NotSupportedFeature,
    ErrorOpenSSL,
    ErrorDecodingBase64,
    ErrorEncodingBase64,
    InputSizeZero
};



typedef unsigned char byte;
typedef std::vector<byte> bytes;
typedef std::vector<std::string> jsonKeys;

#endif // DEFINELORAWAN_H

