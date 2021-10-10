//
// Created by wlb on 2021/10/10.
//

#include "MainReactor.h"

namespace wlb
{

MainReactor::MainReactor()
{
    epollfd = epoll_create(1);
    m_bRunning = false;
}

void MainReactor::pushAcceptor(MainReactor::accept_type *acceptor)
{
    accepts.push_back(acceptor);
}

void MainReactor::pushSockToThread(MainReactor::socket_type sock)
{

}

void MainReactor::run()
{
    m_bRunning = true;
    
    for (auto iter : accepts)
    {
        struct epoll_event ev;
        ev.data.fd = iter->getSocket();
        ev.events = EPOLLIN;
        epoll_ctl(epollfd, EPOLL_CTL_ADD, iter->getSocket(), &ev);
    }
    LOG(INFO) << "epoll add ok ,size : " << accepts.size();
    
    // start threads
    
    
    while (m_bRunning)
    {
        struct epoll_event events[MAXEVENTS];
    
        int infds = epoll_wait(epollfd, events, MAXEVENTS, EPOLL_TIME_OUT);
        if(infds < 0)
        {
            LOG(ERROR) << "epoll_wait failed ";
            break;
        }
        // 超时
        if (infds == 0)
        {
            LOG(ERROR) << "epoll_wait() timeout";
            continue;
        }
    
        LOG(INFO) << "has event ,return size : " <<infds;
    
        for (int i = 0; i < infds; ++i)
        {
            accept_type::endpoint client_endPoint;
            socklen_t len = sizeof(client_endPoint);
            
            socket_type clientsock = accept(
                                    events[i].data.fd,
                                    (struct sockaddr*)&client_endPoint,
                                    &len);
            if (clientsock < 0)
            {
                LOG(WARNING) << "accept failed ,errno : " << errno << " socketfd = " << events[i].data.fd;
                continue;
            }
    
            LOG(INFO) << "new connection ,IP : "
                    << inet_ntoa(client_endPoint.sin_addr)
                    << " ,port : "
                    << ntohs(client_endPoint.sin_port);
            pushSockToThread(clientsock);
            onConnection();
        }
        
        if (accepts.empty())
            m_bRunning = false;
    }
}

void MainReactor::stop()
{
    m_bRunning = false;
}

void MainReactor::onConnection()
{

}


}
