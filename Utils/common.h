#ifndef COMMON_H
#define COMMON_H

#include "defineLorawan.h"
#include "packetstorage.h"


class Common
{
public:
    static std::string bytes2Str(bytes input);
    static bytes str2Bytes(std::string input);
    static bytes ulong2Bytes(unsigned long int value);
    static unsigned long int bytes2ULong(const bytes input, bool bigEndian =true);
    static std::string bytes2HexStr(bytes input, bool withSpace=true, bool withBreaks = false);

    static Lorawan_result convertToBigEndian(bytes littleEndianValue, bytes &bigEndianValue);
    static Lorawan_result convertToLittleEndian(bytes bigEndianValue, bytes &littleEndianValue);


    static Lorawan_result testDecodingEncoding();
    static Lorawan_result testBigAndLittleEndian();

    static Lorawan_result decodeBase64(bytes input, bytes &decoded);
    static Lorawan_result encodeBase64(bytes input, bytes &encoded);
    static Lorawan_result calculate_cmac(bytes keyVal, bytes msgVal, bytes &cmac);
    static Lorawan_result calculateMIC(DataPacket &dataPkt, byte direction, bool changeFCnt = false);

    static Lorawan_result createJsonToSend(bytes rawPacket, std::string refJson, std::string &jsonToSend, std::string msgType = "rxpk");


    static std::string getTime();

    static void setNwkSKey(const bytes &value);
    static void setMicBruteforcing(bool value);
    static void setMAX_FCNT_GAP(unsigned long value);
    static void setMicValuesFileName(const std::string &value);

private:
    static bytes getNextMICValue();
    static size_t calcDecodeLength(const char *b64input);

    static unsigned long int MAX_FCNT_GAP;
    static unsigned long int currentLine;
    static std::string micValuesFileName;
    static bool micBruteforcing;
    static bytes nwkSKey;

};

#endif // COMMON_H
