#ifndef TESTPARAMS_H
#define TESTPARAMS_H

#include <string>
#include "Utils/logger.h"

class TestParams
{
private:
    std::string logFileName;
    std::string interfaceName;
    std::string filter;
    int downlinkCounter = 0;
    unsigned long long int _logLevel = 0;
public:
    TestParams();
    std::string getLogFileName() const;
    void setLogFileName(const std::string &value);
    unsigned long long int getLogLevel() const;
    void setLogLevel(const unsigned long long &logLevel);
    std::string getInterfaceName() const;
    void setInterfaceName(const std::string &value);
    std::string getFilter() const;
    void setFilter(const std::string &value);
    int getDownlinkCounter() const;
    void setDownlinkCounter(int value);
};

#endif // TESTPARAMS_H
