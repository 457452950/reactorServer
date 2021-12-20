#include <iostream>
#include <atomic>
#include <signal.h>
#include "reactorServer.h"

using namespace wlb;
using namespace Log;

std::atomic_int64_t _count{0};

void handle_pipe(int sig)
{
    LOG(L_ERROR) << "error sigpipe!";
}

class test : public BaseServer
{
public:
    virtual void onConnected(BaseConnection* connect) {
        LOG(L_INFO) << connect->getPeerIp() << " : " << connect->getPeerPort();
    };
    virtual void onDisConnected(BaseConnection* connect) {
        LOG(L_INFO);
    };
    virtual void onMessage(BaseConnection* connect, std::string& msg) {
        LOG(L_INFO) << msg;
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
    // signal(SIGPIPE, SIG_IGN);   // 忽略sigpipe信号
    signal(SIGPIPE, handle_pipe);   // 自定义处理函数

    Logger::Init(Log::LOG_LEVEL::L_DEBUG, "rs");
    
    auto* a = CreateAccepter(4001);
    if (a == nullptr)
    {
        return -1;
    }
    

    test * t = new(std::nothrow) test();

    MainReactor m;
    if ( !m.Initialize(t, 4))
    {
        LOG(L_ERROR) << "Initialize error ";
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
