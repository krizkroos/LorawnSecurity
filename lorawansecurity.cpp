#include "lorawansecurity.h"
#include <iostream>
#include "Utils/logger.h"

LorawanSecurity::LorawanSecurity()
{

}

Lorawan_result LorawanSecurity::addTest(std::shared_ptr<LorawanTest> test)
{
    if(test->isValid() == Lorawan_result::Success)
    {
        _test.emplace_back(test);
    }
    else
    {
        std::cout << "Error addTest"<< std::endl;
        return Lorawan_result::Error;
    }

    return Lorawan_result::Success;
}

Lorawan_result LorawanSecurity::setUpTestParams(TestParams &params)
{
    _testParams = params;
    Logger logger("lorawan.log", Logger::JSON | Logger::RawData | Logger::MiTM);
    return Lorawan_result::Success;
}

Lorawan_result LorawanSecurity::startPrerequisites()
{
    Lorawan_result result = Lorawan_result::Success;

    std::cout << std::endl;
    std::cout <<"----------------- Starting test prerequisites ----------------"<< std::endl;
    for(auto &test : _test)
    {
        std::cout << "Test describtion : " + test->getDescription() << std::endl;

        auto &prerequisite = test->getPrerequisite();

        if(prerequisite.size() == 0) // vector size
        {
            std::cout << "No prerequisite required"<< std::endl;
        }
        for(auto &prereq : prerequisite)
        {
            std::cout << "Prerequisite : " + prereq->getName() << std::endl;

            result = prereq->start();
            if(result != Lorawan_result::Success)
            {
                std::cout << "Error starting condition"<< std::endl;
                return result;
            }
            else
                continue;
        }
    }

    return result;
}

Lorawan_result LorawanSecurity::launchTest()
{
    std::cout << std::endl;
    std::cout <<"----------------- Starting tests -----------------------------" << std::endl;
    Lorawan_result result = Lorawan_result::Success;
    for(auto &test : _test)
    {
        std::cout << "Test describtion : " + test->getDescription() << std::endl;
        if(test->launch() != Lorawan_result::Success)
        {
            std::cout << "starting test failed" << std::endl;
        }
    }
    return result;
}
