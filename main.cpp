#include <iostream>
#include "include/DEFINE.h"
#include "include/BaseAcceptor.h"
#include "src/MainReactor.h"

using namespace wlb;
using namespace Log;

int main()
{
    Log::Logger::Init(LOG_LEVEL::L_DEBUG, "rs");
    
    auto* a = createAcceptor(4000);
    MainReactor m;
    if ( !m.Initialize())
    {
        LOG(L_ERROR) << "Initialize error ";
        return -1;
    }
    m.pushAcceptor(a);
    m.run();
    
    m.stop();
    delete a;
    return 0;
}
