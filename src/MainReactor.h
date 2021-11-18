//
// Created by wlb on 2021/10/10.
//

#ifndef REACTORSERVER_MAINREACTOR_H
#define REACTORSERVER_MAINREACTOR_H

#include "HEAD.h"
#include "DEFINE.h"
#include "BaseAcceptor.h"
#include "SubReactorMgr.h"
#include "ReactorServer.h"

namespace wlb
{

class MainReactor
{
public:
    using epoll_type    = int;
    using socket_type   = int;
    using socket_ptr    = socket_type*;
    
    using accept_type   = BaseAcceptor;
    using accept_ptr    = accept_type*;
    using acceptVec     = std::vector<accept_type *>;
    
public:
    MainReactor();
    bool Initialize(BaseServer* server, unsigned int threadCount);
    ~MainReactor();
    
    void pushAcceptor(accept_ptr acceptor);
    
    void run();
    void stop();
    void waitToExit();
    

private:
    void runLoop();

private:
    epoll_type                  epollfd;
    acceptVec                   accepts;
    
    std::thread*                m_pMainThread;
    SubReactorMgr               m_ReactorMgr;
    unsigned int                m_iWorkThreadCount;

    bool                        m_bRunning;
    ReactorServer*              m_pServer;
};

}

#endif //REACTORSERVER_MAINREACTOR_H
