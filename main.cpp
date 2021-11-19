#include <iostream>
#include <atomic>
#include "HEAD.h"
#include "DEFINE.h"
#include "BaseAcceptor.h"
#include "BaseServer.h"
#include "src/MainReactor.h"

using namespace wlb;

std::atomic_int64_t _count{0};

class test : public BaseServer
{
public:
    virtual void onConnected(BaseConnection* connect) {
        LOG(INFO) << connect->getPeerIp() << " : " << connect->getPeerPort();
    };
    virtual void onDisConnected(BaseConnection* connect) {
        LOG(INFO);
    };
    virtual void onMessage(BaseConnection* connect, std::string& msg) {
        // LOG(INFO) << msg;
        _count++;
        if ( !(_count % 100000U) )
        {
            std::cout << "_count:" << _count.load() << "\n";
        }
        
        connect->send(msg);
    };

    test() {}
    virtual ~test() {};
};

int main()
{
    Logger::Init(0, "rs");
    
    auto* a = CreateAccepter(4001);
    test * t = new test();
    if  (a == nullptr)
        return -1;
    MainReactor m;
    if ( !m.Initialize(t, 4))
    {
        LOG(ERROR) << "Initialize error ";
        return -1;
    }
    m.pushAcceptor(a);
    m.run();
    
    // while (1);
    
    // m.stop();
    m.waitToExit();
    delete a;
    return 0;
}
