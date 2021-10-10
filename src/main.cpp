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
    m.pushAcceptor(a);
    m.run();
    
    return 0;
}
