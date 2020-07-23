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

TestParams::TestParams()
{

}
