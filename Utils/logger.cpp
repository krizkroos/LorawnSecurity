#include "logger.h"

#include <iostream>
#include <sstream>
#include <sys/time.h>
#include <iomanip>
#include "common.h"

unsigned long long int Logger::_currentLevel = 0;
std::string Logger::_filename{};

Logger::Logger()
{

}

std::string Logger::getTime()
{
    struct tm *tm;
    struct timeval tv;
    char time_string[40];

    const int bufSize = 30;
    char buf[bufSize];

    gettimeofday(&tv,nullptr);

    tm = localtime(&tv.tv_sec);
    strftime(time_string, sizeof (time_string), "%H:%M:%S", tm);
    snprintf(buf,bufSize, "0x%1x %s.%03d ", static_cast<byte>(pthread_self()),
             time_string,static_cast<int>(tv.tv_sec)/1000);

    return std::string(buf);
}

Logger::Logger(std::string filename, unsigned long long int level)
{
    _filename = filename;
    _currentLevel = level;
}

Lorawan_result Logger::write(unsigned long long level, std::string function, int line, std::string content)
{
    FILE* outfile;

    if((_currentLevel & level) != level)
        return Lorawan_result::WrongLogLevel;

    if(_filename.empty())
        return Lorawan_result::Error;


    outfile = fopen(_filename.c_str(), "a");

    if(outfile == nullptr)
    {
        std::cout << "No output log file available" << std::endl;
        return Lorawan_result::Error;
    }

    setbuf(outfile,nullptr);

    std::stringstream ss;

    ss << getTime() << std::string(" | ");

    ss << std::setw(20) << std::left << std::string("LEVEL: " + std::to_string(level)) << " | ";

    ss << std::setw(25) << std::left << std::string(function + ": " + std::to_string(line)) << " :  ";

    ss << content;

    std::string newLine = ss.str();

    fprintf(outfile, "%s\n", newLine.c_str());

    fclose(outfile);

    return Lorawan_result::Success;

}

Lorawan_result Logger::writeHex(unsigned long long level, std::string function, int line, bytes content, bool withSpace)
{
    std::string prepContent{};

    prepContent = Common::bytes2HexStr(content,withSpace,true);


    return Logger::write(level,function,line,prepContent);

}


