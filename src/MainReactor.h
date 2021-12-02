//
// Created by wlb on 2021/10/10.
//

#ifndef REACTORSERVER_MAINREACTOR_H
#define REACTORSERVER_MAINREACTOR_H

#include "../include/HEAD.h"
#include "../include/DEFINE.h"
#include "../include/BaseAcceptor.h"

namespace wlb
{

class MainReactor
{
public:
    using accept_type = BaseAcceptor;
    using accept_ptr = accept_type*;
    using acceptVec = std::vector<accept_type *>;
public:
    MainReactor();
    bool Initialize();
    ~MainReactor();
    
    void pushAcceptor(accept_ptr acceptor);
    
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
