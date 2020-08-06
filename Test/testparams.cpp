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

TestParams::TestParams()
{
    
}
