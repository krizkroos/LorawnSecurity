#ifndef LORAWANTEST_H
#define LORAWANTEST_H

#include "defineLorawan.h"
#include "Device/lorawandevice.h"
#include "Packet/sendcontroller.h"
#include "TestPrerequisite/testprerequisite.h"

#include <memory>

class LorawanTest
{
protected:
  std::shared_ptr<LorawanDevice> testDevice;
  std::vector<std::shared_ptr<TestPrerequisite>> prerequisite;
  std::string description;


public:
    virtual Lorawan_result launch() =0;
    virtual Lorawan_result stop() =0;
    virtual ~LorawanTest()= 0;


    Lorawan_result isValid();
    void setTestDevice(std::shared_ptr<LorawanDevice> device);
    std::shared_ptr<LorawanDevice> getTestDevice() const;
    Lorawan_result addPrerequisite(std::shared_ptr<TestPrerequisite> p);
    std::string getDescription() const;
    void setDescription(const std::string &value);
    std::vector<std::shared_ptr<TestPrerequisite>>& getPrerequisite();
};

#endif // LORAWANTEST_H
