#ifndef LORAWANTEST_H
#define LORAWANTEST_H

#include "defineLorawan.h"
#include "lorawandevice.h"
#include "testprerequisite.h"

class LorawanTest
{
private:
  LorawanDevice testDevice;
  std::vector<TestPrerequisite> prerequisite;
public:
    LorawanTest();
    Lorawan_result launch();
    Lorawan_result stop();
};

#endif // LORAWANTEST_H
