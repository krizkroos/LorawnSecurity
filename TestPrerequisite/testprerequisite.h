#ifndef TESTPREREQUISITE_H
#define TESTPREREQUISITE_H

#include <string>

#include "defineLorawan.h"

class TestPrerequisite
{
protected:
    std::string name;
public:
    virtual Lorawan_result start() =0;
    virtual Lorawan_result stop() = 0;  
    virtual ~TestPrerequisite() =0;

    Lorawan_result isValid();
    void verbose();
    std::string getName() const;
    void setName(const std::string &value);
};

#endif // TESTPREREQUISITE_H
