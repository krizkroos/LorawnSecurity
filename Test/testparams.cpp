#include "testparams.h"
#include "Utils/logger.h"

std::string TestParams::getLogFileName() const
{
    return logFileName;
}

void TestParams::setLogFileName(const std::string &value)
{
    logFileName = value;
}

unsigned long long int TestParams::getLogLevel() const
{
    return _logLevel;
}

void TestParams::setLogLevel(const unsigned long long int &logLevel)
{
    _logLevel = logLevel;
}

std::string TestParams::getInterfaceName() const
{
    return interfaceName;
}

void TestParams::setInterfaceName(const std::string &value)
{
    interfaceName = value;
}

std::string TestParams::getFilter() const
{
    return filter;
}

void TestParams::setFilter(const std::string &value)
{
    filter = value;
}

int TestParams::getDownlinkCounter() const
{
    return downlinkCounter;
}

void TestParams::setDownlinkCounter(int value)
{
    downlinkCounter = value;
}

int TestParams::getRequestCounter() const
{
    return requestCounter;
}

void TestParams::setRequestCounter(int value)
{
    requestCounter = value;
}

unsigned long TestParams::getMax_gap() const
{
    return max_gap;
}

void TestParams::setMax_gap(unsigned long value)
{
    max_gap = value;
}

bytes TestParams::getNwkSKey() const
{
    return nwkSKey;
}

void TestParams::setNwkSKey(const bytes &value)
{
    nwkSKey = value;
}
