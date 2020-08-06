#ifndef LOGGER_H
#define LOGGER_H

#include <sys/time.h>
#include "tins/tins.h"
#include <string>
#include "defineLorawan.h"

#define FILENAME (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define called(level) writeLog(level,std::string("Called"))
#define writeLog(level, line) Logger::write(level,std::string(FILENAME), std::string(__FUNCTION__), __LINE__, line)
#define writeHexLog(level, data) Logger::writeHex(level, std::string(FILENAME), std::string(__FUNCTION__), __LINE__, data, true)
#define writePacketLog(packet) Logger::writePacket(Logger::Packet | Logger::PacketData, std::string(FILENAME), std::string(__FUNCTION__), __LINE__,packet)


class Logger
{
public:
    enum LogLevel{      
        Device,
        Packet,
        LorawanTest,
        MiTM,
        Security,
        Common,
        BruteforcingMIC,
        BatteryDepletion,
        JSON,
        PacketData,
        RawData
    };
    Logger();
    Logger(std::string filename, unsigned long long int level);
    static Lorawan_result write(unsigned long long int level, std::string file, std::string function, int line, std::string content);
    static Lorawan_result writeHex(unsigned long long int level, std::string file, std::string function, int line, bytes content, bool withSpace);
    static Lorawan_result writePacket(unsigned long long level, std::string file, std::string function, int line, const Tins::Packet &packet);
    static std::string getTime();
    static std::string _filename;
    static unsigned long long int _currentLevel;

private:
    static std::string parseLevel(unsigned long long level);
};

#endif // LOGGER_H
