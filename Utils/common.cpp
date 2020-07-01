#include "common.h"

#include <sstream>
#include <iomanip>

//OPENSSL
//#include <sha.h>
//#include <hmac.h>
//#include <evp.h>
//#include <bio.h>
//#include <buffer.h>

Common::Common()
{

}

std::string Common::bytes2Str(bytes input)
{
    std::string output{""};
    for(auto &byte: input)
    {
        output += static_cast<char>(byte);
    }
    return output;
}

std::string Common::bytes2HexStr(bytes input, bool withSpace)
{
    std::ostringstream stream;

    stream << std::hex << std::uppercase << std::setfill( '0');

    for(auto &byte : input)
    {
        stream << std::setw(2) << static_cast<int>(byte);
        if(withSpace)
            stream << " ";
    }

    return stream.str();

}

long Common::decodeBase64(bytes input, bytes decoded)
{
//    BIO *b64;
//    b64 = BIO_new(BIO_f_base64());
}
