#include "logger.h"

#include <iostream>
#include <sstream>

#include <iomanip>

#include "common.h"


unsigned long long int Logger::_currentLevel = 0;
std::string Logger::_filename{};

Logger::Logger()
{

}
Logger::Logger(std::string filename, unsigned long long int level)
{
    _filename = filename;
    _currentLevel = level;
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

Lorawan_result Logger::writePacket(unsigned long long level, std::string file, std::string function, int line, const Tins::Packet &packet)
{
    std::stringstream ss;

    ss << std::string("\n\n--------------------");
    if(packet.pdu()->find_pdu<Tins::IP>())
    {
        const Tins::IP &ip = packet.pdu()->rfind_pdu<Tins::IP>();

        ss << std::endl;
        ss <<"- IP protocol -" <<std::endl;
        ss << ip.src_addr() << "  -->  " << ip.dst_addr() << std::endl;

        if(ip.inner_pdu()->find_pdu<Tins::UDP>())
        {
            ss <<"- UDP packet -" << std::endl;
            const Tins::UDP &udp = ip.inner_pdu()->rfind_pdu<Tins::UDP>();
            ss << "src port: " << udp.sport() << "--> dst port: " << udp.dport() << std::endl;

        }

        return Logger::write(level, file, function, line, ss.str());
    }
    else
    {
        writeLog(Logger::Packet, "Unknown packet");
        return Lorawan_result::NotSupportedFeature;
    }

}

Lorawan_result Logger::write(unsigned long long level, std::string file, std::string function, int line, std::string content)
{
    FILE* outfile;

    if((_currentLevel & level) != level)
    {
        std::cout << "Current level mismatches: " << std::endl;
        std::cout << "Current: " << _currentLevel << std::endl;
        std::cout << "Incoming: " << level << std::endl;
        return Lorawan_result::WrongLogLevel;
    }

    if(_filename.empty())
    {
        std::cout << "Log file name is empty!" << std::endl;
        return Lorawan_result::Error;
    }


    outfile = fopen(_filename.c_str(), "a");

    if(outfile == nullptr)
    {
        std::cout << "No output log file available" << std::endl;
        return Lorawan_result::Error;
    }

    setbuf(outfile,nullptr);

    std::stringstream ss;


    ss << std::string("\n");
    ss << getTime() << std::string(" | ");

    ss << std::setw(11) << std::string("LEVEL: ") << parseLevel(level) << std::left  << " | ";

    ss << std::setw(12) << std::left << std::string(file + " :" + function + ": " + std::to_string(line)) << ":  ";

    ss << std::setw(10) << std::left << content;

    std::string newLine = ss.str();
    fprintf(outfile, "%s\n", newLine.c_str());

    std::cout << newLine << std::endl;

    fclose(outfile);

    return Lorawan_result::Success;

}

Lorawan_result Logger::writeHex(unsigned long long level, std::string file, std::string function, int line, bytes content, bool withSpace)
{
    std::string prepContent{};

    prepContent = Common::bytes2HexStr(content,withSpace,true);

    prepContent += "\n";

    return Logger::write(level,file, function,line,prepContent);

}

std::string Logger::parseLevel(unsigned long long level)
{
    switch (level) {

    case Logger::Device:
        return "Device";
    case Logger::Packet:
        return "Packet";
    case Logger::LorawanTest:
       return "LorawanTest";
    case Logger::MiTM:
        return "MiTM";
    case Logger::Security:
        return "Security";
    case Logger::Common:
        return "Common";
    case Logger::BruteforcingMIC:
        return "BruteforcingMIC";
    case Logger::JSON:
        return "JSON";
    case Logger::PacketData:
        return "PacketData";
    case Logger::RawData:
        return "RawData";
    case Logger::BatteryDepletion:
        return "Battery";
    default:
        return "Unknown";

    }

}


