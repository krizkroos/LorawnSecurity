#include "lorawantest.h"

#include "Utils/logger.h"

std::shared_ptr<LorawanDevice> LorawanTest::getTestDevice() const
{
    return testDevice;
}


void LorawanTest::setTestDevice(std::shared_ptr<LorawanDevice> device)
{
    if(device != nullptr)
    {
        testDevice = device;
    }
}

Lorawan_result LorawanTest::addPrerequisite(std::shared_ptr<TestPrerequisite> p)
{
    if(p->isValid() == Lorawan_result::Success)
    {
        prerequisite.emplace_back(p);
    }
    else
    {
        writeLog(Logger::LorawanTest,"Error addPrerequisite");
        return Lorawan_result::Error;
    }

    return Lorawan_result::Success;
}

std::string LorawanTest::getDescription() const
{
    return description;
}

void LorawanTest::setDescription(const std::string &value)
{
    description = value;
}

std::vector<std::shared_ptr<TestPrerequisite> >& LorawanTest::getPrerequisite()
{
    return prerequisite;
}

Lorawan_result LorawanTest::isValid()
{
    if(testDevice->isValid() != Lorawan_result::Success)
        return Lorawan_result::Error;

    for(auto& prereq: prerequisite)
    {
        if(prereq->isValid() != Lorawan_result::Success)
            return Lorawan_result::Error;
        else
            continue;
    }

    return Lorawan_result::Success;
}

LorawanTest::~LorawanTest()
{

}
