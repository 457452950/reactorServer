//
// Created by wlb on 2021/10/10.
//

#ifndef REACTORSERVER_MAINREACTOR_H
#define REACTORSERVER_MAINREACTOR_H

#include "HEAD.h"
#include "DEFINE.h"
#include "BaseAcceptor.h"

namespace wlb
{

class MainReactor
{
public:
    using epoll_type = int;
    using socket_type = int;
    
    using accept_type = BaseAcceptor;
    using acceptVec = std::vector<accept_type *>;
public:
    MainReactor();
    
    void pushAcceptor(accept_type *acceptor);
    
    void run();
    void stop();
    

private:
    void onConnection();
    void pushSockToThread(socket_type sock);

private:
    epoll_type epollfd;
    acceptVec accepts;
    
    bool m_bRunning;
};

}

#endif //REACTORSERVER_MAINREACTOR_H
