#include <iostream>
#include "include/HEAD.h"
#include "include/DEFINE.h"
#include "Acceptor.h"
#include "MainReactor.h"

using namespace wlb;
using namespace Log;

int main()
{
    Log::Logger::Init(LOG_LEVEL::DEBUG, "rs");
    
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
