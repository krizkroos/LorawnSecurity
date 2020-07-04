#include "common.h"

#include <sstream>
#include <iomanip>
#include <memory>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <map>
//OPENSSL
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
    size_t decodeLen = 0;
    Lorawan_result rv = Lorawan_result::Success;


    if(input.size() == 0)
       return Lorawan_result::InputSizeZero;

    decoded.clear();
    std::string tmp = Common::bytes2Str(input);
    const char* b64message = tmp.c_str();

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

Lorawan_result Common::testDecodingEncoding()
{
    bytes decoded;
    std::map<std::string, bytes> loraBase64 = {
        {"AKbtAtB+1bNwgdYfAAujBAAJGFILVhM=", bytes({0x00,0xA6,0xED,0x02,0xD0,0x7E,0xD5,0xB3,0x70,0x81,0xD6,0x1F,0x00,0x0B,0xA3,0x04,0x00,0x09,0x18,0x52,0x0B,0x56,0x13})},
        {"II9uo5ycgdFjz/RVwUO88Q+vnqghauvzU9kSal2wUwFu", bytes({0x20,0x8F,0x6E,0xA3,0x9C,0x9C,0x81,0xD1,0x63,0xCF,0xF4,0x55,0xC1,0x43,0xBC,0xF1,0x0F,0xAF,0x9E,0xA8,0x21,0x6A,0xEB,                       0xF3,0x53,0xD9,0x12,0x6A,0x5D,0xB0,0x53,0x01,0x6E})},
        {"QOYoASYAAAABRMXmr1IxVcy+qw==", bytes({0x40,0xE6,0x28,0x01,0x26,0x00,0x00,0x00,0x01,0x44,0xC5,0xE6,0xAF,0x52,0x31,0x55,0xCC,0xBE,0xAB})},
        {"QOYoASYAAQADmUq+6E3NTDa6Yg==", bytes({0x40,0xE6,0x28,0x01,0x26,0x00,0x01,0x00,0x03,0x99,0x4A,0xBE,0xE8,0x4D,0xCD,0x4C,0x36,0xBA,0x62})}
                                                };
    for(auto& value: loraBase64)
    {
        std::cout << std::endl;
        std::cout << "-------------------"<< std::endl;
        std::string data = value.first;
        bytes refDecoded = value.second;
        bytes encoded = Common::str2Bytes(data);
        bytes calcDecoded;

        if(Common::decodeBase64(encoded, calcDecoded) == Lorawan_result::Success)
        {
            std::cout << "string: " << data <<std::endl;
            std::cout << "reference decoded: "<< Common::bytes2HexStr(refDecoded) <<std::endl;
            std::cout << "decoded bytes:     "<< Common::bytes2HexStr(calcDecoded) <<std::endl;
            bytes myEncoded;


            if(Common::encodeBase64(calcDecoded,myEncoded) == Lorawan_result::Success)
            {
                std::cout << "reference encoded: "<< Common::bytes2HexStr(encoded) <<std::endl;
                std::cout << "encoded bytes:     "<< Common::bytes2HexStr(myEncoded) <<std::endl;
            }
            else
                std::cout << "string cannot encode" <<std::endl;
        }
        else
            std::cout << "string cannot decode" <<std::endl;
    }

}
