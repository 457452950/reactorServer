#include <iostream>
#include "HEAD.h"
#include "DEFINE.h"
#include "BaseAcceptor.h"
#include "MainReactor.h"

using namespace wlb;

int main()
{
    
    auto* a = CreateAccepter(4001);
    if  (a == nullptr)
        return -1;
    MainReactor m;
    if ( !m.Initialize(4))
    {
        LOG(ERROR) << "Initialize error ";
        return -1;
    }
    m.pushAcceptor(a);
    m.run();
    
    while (1);
    
    m.stop();
    m.waitToExit();
    delete a;
    return 0;
}
