#include <iostream>
#include "HEAD.h"
#include "DEFINE.h"
#include "Acceptor.h"
#include "MainReactor.h"

using namespace wlb;

int main()
{
    
    Acceptor* a = new Acceptor(4000);
    MainReactor m;
    if ( !m.Initialize())
    {
        LOG(ERROR) << "Initialize error ";
        return -1;
    }
    m.pushAcceptor(a);
    m.run();
    
    m.stop();
    delete a;
    return 0;
}
