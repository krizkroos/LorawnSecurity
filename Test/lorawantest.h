#ifndef LORAWANTEST_H
#define LORAWANTEST_H

#include "defineLorawan.h"
#include "lorawandevice.h"
#include "testprerequisite.h"

class LorawanTest
{
protected:
  LorawanDevice testDevice;
  std::vector<TestPrerequisite> prerequisite;
public:
    virtual Lorawan_result launch();
    virtual Lorawan_result stop();
    virtual ~LorawanTest();
};

#endif // LORAWANTEST_H
