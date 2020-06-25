#include "testprerequisite.h"

std::string TestPrerequisite::getName() const
{
    return name;
}

void TestPrerequisite::setName(const std::string &value)
{
    name = value;
}

TestPrerequisite::~TestPrerequisite()
{

}

Lorawan_result TestPrerequisite::isValid()
{

    return Lorawan_result::Success;

}

void TestPrerequisite::verbose()
{

}
