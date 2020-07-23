#ifndef LOGGER_H
#define LOGGER_H


#include <string>

#include "defineLorawan.h"

#define writeLog(level, line) Logger::write(level, std::string(__FUNCTION__), __LINE__, line)
#define writeHexLog(level, data) Logger::writeHex(level, std::string(__FUNCTION__), __LINE__, data, true)

class Logger
{
public:
    enum LogLevel{
        RAW_PACKET = 0,
        JSON = 1,
        NORMAL = 2,
        FILENAME = 3,
    };
    Logger();
    Logger(std::string filename, unsigned long long int level);
    static Lorawan_result write(unsigned long long int level, std::string function, int line, std::string content);
    static Lorawan_result writeHex(unsigned long long int level, std::string function, int line, bytes content, bool withSpace);
    static std::string _filename;
    static unsigned long long int _currentLevel;
private: 
    static std::string getTime();
};

#endif // LOGGER_H
