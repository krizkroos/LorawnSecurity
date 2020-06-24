#ifndef TESTPREREQUISITE_H
#define TESTPREREQUISITE_H

#include <string>

#include "defineLorawan.h"

class TestPrerequisite
{
private:
    std::string name;
public:
    TestPrerequisite();
    Lorawan_result start();
    Lorawan_result stop();
    void verbose();


};

#endif // TESTPREREQUISITE_H
