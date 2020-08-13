#ifndef DEFINELORAWAN_H
#define DEFINELORAWAN_H

#include <vector>
#include <string>
#include <memory>
#include <map>
#include <string>

enum class Lorawan_result
{
    Success,
    Error,
    ErrorPrerequisite,
    ErrorTest,
    ErrorTestSetUp,
    WrongSniffingParameters,
    NoValueAvailable,
    NotSupportedFeature,
    ErrorOpenSSL,
    ErrorDecodingBase64,
    ErrorEncodingBase64,
    ErrorConvert2BigEndian,
    ErrorConvert2LittleEndian,
    InputSizeZero,
    ErrorAddPacket,
    ErrorParsing,
    ErrorSerialize,
    ErrorCalcMIC,
    ErrorCreatingPacket,
    WrongLogLevel,
    ErrorLogging
};

typedef unsigned char byte;
typedef std::vector<byte> bytes;
typedef std::vector<std::string> jsonKeys;

#define UNUSED(x) (void)(x)

#endif // DEFINELORAWAN_H

