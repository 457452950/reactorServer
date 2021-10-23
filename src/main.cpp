#include <iostream>
#include "HEAD.h"
#include "DEFINE.h"
#include "Acceptor.h"
#include "MainReactor.h"

using namespace wlb;

int main()
{
    
    Acceptor* a = new Acceptor(4000);
    if  (a == nullptr)
        return -1;
    MainReactor m;
    m.setWorkerThreadCount(4);
    if ( !m.Initialize())
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
