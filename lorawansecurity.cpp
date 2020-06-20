#include "lorawansecurity.h"
#include <iostream>

LorawanSecurity::LorawanSecurity()
{
}

void LorawanSecurity::testLib()
{
    std::cout <<"Test shared library"<<std::endl;
    insideFunction();
}

void LorawanSecurity::insideFunction()
{
    std::cout <<"inside function private"<<std::endl;
}
