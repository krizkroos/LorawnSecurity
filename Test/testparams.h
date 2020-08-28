#ifndef TESTPARAMS_H
#define TESTPARAMS_H

#include "defineLorawan.h"
#include "Utils/logger.h"

class TestParams
{
private:
    std::string logFileName;
    std::string micValuesFileName;
    std::string interfaceName;
    std::string filter;
    int downlinkCounter = 0;
    int requestCounter = 0;
    unsigned long max_gap = 0;
    unsigned long long int _logLevel = 0;

    bytes nwkSKey;
    bytes appKey;
public:
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
    int getRequestCounter() const;
    void setRequestCounter(int value);
    unsigned long getMax_gap() const;
    void setMax_gap(unsigned long value);
    bytes getNwkSKey() const;
    void setNwkSKey(const bytes &value);
    std::string getMicValuesFileName() const;
    void setMicValuesFileName(const std::string &value);
    bytes getAppKey() const;
    void setAppKey(const bytes &value);
};

#endif // TESTPARAMS_H
