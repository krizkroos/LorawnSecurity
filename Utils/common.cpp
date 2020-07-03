#include "common.h"

#include <sstream>
#include <iomanip>
#include <memory>
#include <stdio.h>
#include <string.h>
//OPENSSL
#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>

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

bytes Common::str2Bytes(std::string input)
{
    if(input.empty())
        return bytes();

    return bytes(input.begin(),input.end());
}

//https://gist.github.com/barrysteyn/7308212
size_t Common::calcDecodeLength(const char* b64input)
{ //Calculates the length of a decoded string
    size_t len = strlen(b64input), padding = 0;

    if (b64input[len-1] == '=' && b64input[len-2] == '=') //last two chars are =
        padding = 2;
    else if (b64input[len-1] == '=') //last char is =
        padding = 1;

    return (len*3)/4 - padding;
}

Lorawan_result Common::decodeBase64(bytes input, bytes& decoded)
{

    BIO *bio, *b64;
    Lorawan_result rv = Lorawan_result::Success;

    if(input.size() == 0)
       return Lorawan_result::InputSizeZero;

    decoded.clear();

    char* b64message = reinterpret_cast<char*>(&input.at(0));
    size_t decodeLen = 0;
    decodeLen = calcDecodeLength(b64message);
    byte* buffer = reinterpret_cast<unsigned char*>(malloc(decodeLen +1));
    (buffer)[decodeLen] = '\0';

    bio = BIO_new_mem_buf(b64message, -1);
    b64 = BIO_new(BIO_f_base64());
    bio = BIO_push(b64, bio);

    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); //Do not use newlines to flush buffer

    int length = BIO_read(bio, buffer, static_cast<int>(strlen(b64message)));

    if(length == static_cast<int>(decodeLen)) //length should equal decodeLen, else something went horribly wrong
    {
        decoded = bytes(buffer, buffer + length);
    }
    else
        rv = Lorawan_result::ErrorDecodingBase64;


    BIO_free_all(bio);

    return rv;
}

Lorawan_result Common::encodeBase64(bytes input, bytes &encoded)
{
    BIO *bio, *b64;
    BUF_MEM *bufferPtr;
    char* buffer = reinterpret_cast<char*>(&input.at(0));
    Lorawan_result rv = Lorawan_result::Success;

    if(input.size() == 0)
       return Lorawan_result::InputSizeZero;

    encoded.clear();

    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new(BIO_s_mem());
    bio = BIO_push(b64, bio);

    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL); //Ignore newlines - write everything in one line
    BIO_write(bio, buffer, static_cast<int>(input.size()));
    BIO_flush(bio);
    BIO_get_mem_ptr(bio, &bufferPtr);
    BIO_set_close(bio, BIO_NOCLOSE);
    BIO_free_all(bio);

    encoded = bytes(reinterpret_cast<byte*>((*bufferPtr).data), reinterpret_cast<byte*>((*bufferPtr).data) + (*bufferPtr).length);

    if(encoded.empty())
        rv = Lorawan_result::ErrorEncodingBase64;

    return rv;
}
