#ifndef TESTPARAMS_H
#define TESTPARAMS_H

#include <string>

class TestParams
{
private:
    std::string logFileName;
public:
    TestParams();
    std::string getLogFileName() const;
    void setLogFileName(const std::string &value);
};

#endif // TESTPARAMS_H
