#include "common.h"
#include "logger.h"
#include "jsonparser.h"


#include <sstream>
#include <fstream>
#include <iomanip>
#include <memory>
#include <stdio.h>
#include <string.h>
#include <map>
//OPENSSL
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/cmac.h>
#include <openssl/err.h>

unsigned long int Common::MAX_FCNT_GAP = 1;
unsigned long int Common::currentLine = 0;
bool Common::micBruteforcing = false;
std::string Common::micValuesFileName = "";

bytes Common::nwkSKey = {0xC5,0xCD,0x1F,0x3F,0x85,0x40,0xF0,0xC8,0xA0,0x0F,0x3F,0x5E,0x62,0x44,0x32,0xA0};


std::string Common::bytes2Str(bytes input)
{
    called(Logger::Common);
    std::string output{""};
    for(auto &byte: input)
    {
        output += static_cast<char>(byte);
    }
    return output;
}

std::string Common::getTime()
{
    struct tm *tm;
    struct timeval tv;
    char time_string[40];

    gettimeofday(&tv,nullptr);
    tm = localtime(&tv.tv_sec);
    strftime(time_string, sizeof (time_string), "%H-%M-%S", tm);

    return std::string(time_string);
}

bytes Common::getNextMICValue()
{
    unsigned long int _localLineNo =0;
    std::fstream infile(micValuesFileName);
    called(Logger::Common);
    writeLog(Logger::Common, "value of currentLine = " + std::to_string(currentLine));
    writeLog(Logger::Common, "value of _localLine = " + std::to_string(_localLineNo));
    unsigned long int nextValue =0;

    while(infile >> nextValue)
    {
        if(_localLineNo == currentLine)
        {
            currentLine++;
            break;
        }
        else
        {
            _localLineNo++;
            continue;
        }
    }
    bytes micValue =Common::ulong2Bytes(nextValue);
    writeLog(Logger::Common, "Next value of MIC:");
    writeHexLog(Logger::Common, micValue);

    return micValue;
}



void Common::setMicValuesFileName(const std::string &value)
{
    micValuesFileName = value;
}

void Common::setMicBruteforcing(bool value)
{
    micBruteforcing = value;
}


std::string Common::bytes2HexStr(bytes input, bool withSpace, bool withBreaks)
{
    std::ostringstream stream;
    called(Logger::Common);
    stream << std::hex << std::uppercase << std::setfill( '0');

    int counter{0};

    if(withBreaks)
    {
        stream << "\n\n";
    }
    for(auto &byte : input)
    {
        stream << std::setw(2) << static_cast<int>(byte);

        if(withBreaks)
        {
            if(++counter == 20) // first line is also good
            {
                stream << "\n";
                counter =0;
                continue;
            }
        }

        if(withSpace)
            stream << " ";

    }

    return stream.str();

}

bytes Common::str2Bytes(std::string input)
{
    called(Logger::Common);
    if(input.empty())
        return bytes();

    return bytes(input.begin(),input.end());
}

unsigned long int Common::bytes2ULong(const bytes input, bool bigEndian)
{
    unsigned long int output =0;
    called(Logger::Common);
    if(bigEndian)
    {
        for(size_t i=0; i < input.size() ; i++)
        {
            output =  (output << 8) + size_t(input.at(i));
        }
    }

    return output;
}

bytes Common::ulong2Bytes(unsigned long int value)
{
    bytes v;
    called(Logger::Common);
    if(value == 0)
    {
        v.emplace_back(0x00);
        return v;
    }

    for(int i=7; i >=0; i--)
    {
        v.emplace_back(static_cast<int>(value >> (i*8) & 0xFF));
    }
    //delete uncessery zeros
    size_t i =0;
    for(i =0 ; i <v.size(); i++)
    {
        byte tmp = v.at(i);
        if(tmp != 0x00)
            break;
    }
    if(i > 0)
    {
        v.erase(v.begin(), v.begin()+static_cast<int>(i));
    }

    return v;
}

//function calcDecodeLength is from source:
//https://gist.github.com/barrysteyn/7308212
size_t Common::calcDecodeLength(const char* b64input)
{ //Calculates the length of a decoded string
    size_t len = strlen(b64input), padding = 0;
    called(Logger::Common);
    if (b64input[len-1] == '=' && b64input[len-2] == '=') //last two chars are =
        padding = 2;
    else if (b64input[len-1] == '=') //last char is =
        padding = 1;

    return (len*3)/4 - padding;
}

void Common::setNwkSKey(const bytes &value)
{
    nwkSKey = value;
}

void Common::setMAX_FCNT_GAP(unsigned long value)
{
    Common::MAX_FCNT_GAP = value;
    writeLog(Logger::Common,"MAX_FCNT_GAP = " + std::to_string(MAX_FCNT_GAP));
}

Lorawan_result Common::calculateMIC(DataPacket& dataPkt, byte direction, bool changeFCnt)
{
    called(Logger::Common);
    writeLog(Logger::Common,"Calculating new MIC value");
    writeLog(Logger::Common,"previous value of MIC: " + Common::bytes2HexStr(dataPkt.getMIC()) );

    bytes initBlock{};
    bytes _localFCnt{};
    bytes _msg{};

    bytes calcCMAC{};

    if(Common::micBruteforcing)
    {
       bytes mic = Common::getNextMICValue();
       writeLog(Logger::Common,"Setting up MIC value from file");
       dataPkt.setMIC(mic);

       return Lorawan_result::Success;
    }

    bytes frameCounter = dataPkt.getFrameCounter();

    initBlock.emplace_back(0x49);

    for(int i=0; i < 4; i++)
        initBlock.emplace_back(0x00);

    if(direction != 0x00 && direction != 0x01)
    {
        return Lorawan_result::ErrorCalcMIC;
    }

    initBlock.emplace_back(direction);


    bytes littleEndianDevAddr{};
    if(Common::convertToLittleEndian(dataPkt.getDevAddr(),littleEndianDevAddr) != Lorawan_result::Success)
    {
        return Lorawan_result::ErrorConvert2LittleEndian;
    }

    initBlock.insert(initBlock.end(),littleEndianDevAddr.begin(), littleEndianDevAddr.end());

    if(frameCounter.empty())
    {
        writeLog(Logger::Common,"Empty frame counter");
        return Lorawan_result::ErrorCalcMIC;
    }
    _localFCnt.emplace_back(0x00);
    _localFCnt.emplace_back(0x00);

    if(frameCounter.size() == 1)
    {
        _localFCnt.emplace_back(0x00);
        _localFCnt.emplace_back(frameCounter.front());
    }
    else
    {
        _localFCnt.insert(_localFCnt.begin(), frameCounter.begin(), frameCounter.end());
    }
    if(changeFCnt)
    {
        unsigned long int dosCount = Common::bytes2ULong(_localFCnt);

        dosCount += Common::MAX_FCNT_GAP;

        _localFCnt = Common::ulong2Bytes(dosCount);
        writeLog(Logger::Common,"changed value of FCnt: " + Common::bytes2HexStr(_localFCnt));
    }

    bytes littleEndianFCnt{};
    if(Common::convertToLittleEndian(_localFCnt,littleEndianFCnt) != Lorawan_result::Success)
    {
        return Lorawan_result::ErrorConvert2LittleEndian;
    }


    initBlock.insert(initBlock.end(), littleEndianFCnt.begin(), littleEndianFCnt.end());

    initBlock.emplace_back(0x00);

    bytes _rawPacket = dataPkt.getRawData();
    bytes::iterator nextByte = _rawPacket.begin();

    while(nextByte != _rawPacket.end() -4)
    {
        _msg.emplace_back(*nextByte);
        nextByte++;
    }

    if(_msg.empty())
        return Lorawan_result::ErrorCalcMIC;

    initBlock.emplace_back(static_cast<byte>(_msg.size()));

    bytes message{};
    message.insert(message.begin(), initBlock.begin(), initBlock.end());
    message.insert(message.end(), _msg.begin(), _msg.end());

    writeLog(Logger::Common, "key value used for calculationg CMAC:");
    writeHexLog(Logger::Common,Common::nwkSKey);

    if(Common::calculate_cmac(Common::nwkSKey,message, calcCMAC) != Lorawan_result::Success)
    {
        writeLog(Logger::Common,"Error calculating CMAC");
        return Lorawan_result::ErrorCalcMIC;
    }

    dataPkt.setMIC(bytes(calcCMAC.begin(), calcCMAC.begin() + 4));

    writeHexLog(Logger::Common, dataPkt.getMIC());

    writeLog(Logger::Common,"Finished calculating MIC");
    return Lorawan_result::Success;
}

Lorawan_result Common::createJsonToSend(bytes rawPacket, std::string refJson, std::string &jsonToSend, std::string msgType)
{
    JsonParser jParser;
    called(Logger::Common);
    if(jParser.parse(refJson) != Lorawan_result::Success)
        return Lorawan_result::ErrorCreatingPacket;

    jsonToSend.clear();
    bytes encodedPacket{};
    writeLog(Logger::BruteforcingMIC,"ref: " + refJson);

    if(Common::encodeBase64(rawPacket,encodedPacket) != Lorawan_result::Success)
        return Lorawan_result::ErrorCreatingPacket;

    if(encodedPacket.empty())
        return Lorawan_result::ErrorCreatingPacket;

    std::string changedValue = Common::bytes2Str(encodedPacket);

    writeLog(Logger::JSON, "Looking for a message type in json: " + msgType);

    if(jParser.changeValue(jsonKeys({msgType,"data"}),changedValue) != Lorawan_result::Success)
        return Lorawan_result::ErrorCreatingPacket;

    jsonToSend = jParser.getJson();


    writeLog(Logger::BruteforcingMIC," changed: " + jsonToSend );


    return Lorawan_result::Success;
}

Lorawan_result Common::decodeBase64(bytes input, bytes& decoded)
{

    BIO *bio, *b64;
    size_t decodeLen = 0;
    Lorawan_result rv = Lorawan_result::Success;

    called(Logger::Common);
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
    called(Logger::Common);
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

Lorawan_result Common::calculate_cmac(bytes keyVal, bytes msgVal, bytes &cmac)
{
    unsigned char mact[16] = {0};
    size_t mactlen;
    unsigned char *key = reinterpret_cast<unsigned char*>(&keyVal.at(0));
    unsigned char *message = reinterpret_cast<unsigned char*>(&msgVal.at(0));
    called(Logger::Common);
    CMAC_CTX *ctx = CMAC_CTX_new();
    CMAC_Init(ctx, key, 16, EVP_aes_128_cbc(), NULL);

    CMAC_Update(ctx, message, msgVal.size());
    CMAC_Final(ctx, mact, &mactlen);

    /*  TEST:
     *     bytes key = { 0x2b,0x7e,0x15,0x16,
                            0x28,0xae,0xd2,0xa6,
                            0xab,0xf7,0x15,0x88,
                            0x09,0xcf,0x4f,0x3c};

    //
          bytes message = { 0x6b,0xc1,0xbe,0xe2,
                                0x2e,0x40,0x9f,0x96,
                                0xe9,0x3d,0x7e,0x11,
                                0x73,0x93,0x17,0x2a };
     *
     * expected result T = 070a16b4 6b4d4144 f79bdd9d d04a287c */
    cmac = bytes(mact, mact + mactlen);
    CMAC_CTX_free(ctx);

    return Lorawan_result::Success;
}

Lorawan_result Common::encrypt_aes128(bytes key, bytes message, bytes encMsg)
{
    UNUSED(key);
    UNUSED(message);
    UNUSED(encMsg);
    //    EVP_CIPHER_CTX *ctx;

    //        int len;

    //        int ciphertext_len;

    //        /* Create and initialise the context */
    //        if(!(ctx = EVP_CIPHER_CTX_new()))
    //        {

    //        }


    //        /*
    //         * Initialise the encryption operation. IMPORTANT - ensure you use a key
    //         * and IV size appropriate for your cipher
    //         * In this example we are using 256 bit AES (i.e. a 256 bit key). The
    //         * IV size for *most* modes is the same as the block size. For AES this
    //         * is 128 bits
    //         */
    //        if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv))
    //        {

    //        }

    //        /*
    //         * Provide the message to be encrypted, and obtain the encrypted output.
    //         * EVP_EncryptUpdate can be called multiple times if necessary
    //         */
    //        if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
    //        {
    //        }

    //        ciphertext_len = len;

    //        /*
    //         * Finalise the encryption. Further ciphertext bytes may be written at
    //         * this stage.
    //         */
    //        if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
    //        {

    //        }
    //        ciphertext_len += len;

    //        /* Clean up */
    //        EVP_CIPHER_CTX_free(ctx);

    //        return ciphertext_len;

     return Lorawan_result::NoValueAvailable;
}

Lorawan_result Common::convertToLittleEndian(bytes bigEndianValue, bytes &littleEndianValue)
{
    called(Logger::Common);
    littleEndianValue = std::vector<byte>(bigEndianValue.size(), 0x00); //init

    if(bigEndianValue.empty())
        return Lorawan_result::InputSizeZero;

    size_t refSize = bigEndianValue.size();

    for(size_t index=0; index < refSize; index++)
    {
        int newPos = static_cast<int>(refSize -index-1);
        littleEndianValue.at(static_cast<size_t>(newPos)) = bigEndianValue.at(index);
    }

    return Lorawan_result::Success;
}

Lorawan_result Common::convertToBigEndian(bytes littleEndianValue, bytes &bigEndianValue)
{
    called(Logger::Common);
    if(littleEndianValue.empty())
        return Lorawan_result::InputSizeZero;

    size_t refSize = littleEndianValue.size();
    bigEndianValue = std::vector<byte>(littleEndianValue.size(), 0x00); //init

    for(size_t index=0; index < refSize; index++)
    {
        int newPos = static_cast<int>(refSize -index-1);
        bigEndianValue.at(static_cast<size_t>(newPos)) = littleEndianValue.at(index);
    }

    return Lorawan_result::Success;
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
        writeLog(Logger::Common,"-------------------");
        std::string data = value.first;
        bytes refDecoded = value.second;
        bytes encoded = Common::str2Bytes(data);
        bytes calcDecoded;

        if(Common::decodeBase64(encoded, calcDecoded) == Lorawan_result::Success)
        {
            writeLog(Logger::Common,"string: " + data);
            writeLog(Logger::Common,"reference decoded: " + Common::bytes2HexStr(refDecoded));
            writeLog(Logger::Common, "decoded bytes:     " +  Common::bytes2HexStr(calcDecoded));
            bytes myEncoded;


            if(Common::encodeBase64(calcDecoded,myEncoded) == Lorawan_result::Success)
            {
                writeLog(Logger::Common,"reference encoded: " + Common::bytes2HexStr(encoded));
                writeLog(Logger::Common,"encoded bytes:     " + Common::bytes2HexStr(myEncoded));
            }
            else
            {
                writeLog(Logger::Common,"string cannot encode");
                return Lorawan_result::ErrorTest;
            }
        }
        else
        {
            writeLog(Logger::Common,"string cannot decode");
            return Lorawan_result::ErrorTest;
        }
    }

    return Lorawan_result::Success;
}

Lorawan_result Common::testBigAndLittleEndian()
{

    std::vector<bytes> testsEndian;

    testsEndian.push_back(bytes({0xE6, 0x28, 0x01, 0x26}));
    testsEndian.push_back(bytes({0xE6, 0x28, 0x01, 0x26, 0x11, 0x22, 0x12}));
    testsEndian.push_back(bytes({0xE6, 0x28}));
    Lorawan_result rv  = Lorawan_result::Success;
    for(auto &value: testsEndian)
    {
        bytes bigEndian{};
        bytes littleEndian = value;

        writeLog(Logger::Common,"----------------");

        rv = Common::convertToBigEndian(littleEndian, bigEndian);
        if(rv == Lorawan_result::Success)
        {
            writeLog(Logger::Common,"littleEnd Value:" + Common::bytes2HexStr(littleEndian));
            writeLog(Logger::Common,"bigEndian Value:" + Common::bytes2HexStr(bigEndian));

            littleEndian.clear();
            rv = Common::convertToLittleEndian(bigEndian, littleEndian);
            if (rv == Lorawan_result::Success)
            {
                writeLog(Logger::Common,"littleEnd Value:" + Common::bytes2HexStr(littleEndian));
            }

        }
    }

    return rv;

}
