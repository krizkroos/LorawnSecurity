#ifndef DEFINELORAWAN_H
#define DEFINELORAWAN_H

#include <vector>

enum class Lorawan_result
{
    Success,
    Error,
    WrongSniffingParameters,
    NoValueAvailable
};



typedef unsigned char byte;
typedef std::vector<byte> bytes;

#endif // DEFINELORAWAN_H

