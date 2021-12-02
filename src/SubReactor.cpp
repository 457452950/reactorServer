//
// Created by wlb on 2021/10/15.
//

#include "SubReactor.h"

namespace wlb
{

SubReactor::SubReactor()
{
    epollfd = -1;
    m_bRunning = false;
}

SubReactor::~SubReactor()
{
    if (epollfd != -1)
    {
        close(epollfd);
    }
}

bool SubReactor::Initialize()
{
    epollfd = epoll_create(1);
    if (epollfd == -1){
        return false;
    }
    
    return true;
}

bool SubReactor::pushSocket(socket_type sock)
{
    if ( !add2Conncts(sock) ){
        return false;
    }
    if ( !add2Epoll(sock))
        return false;
    
    return true;
}

void SubReactor::Run()
{
    m_bRunning = true;
    
    while (m_bRunning)
    {
    
    }
}

bool SubReactor::add2Epoll(socket_type sock)
{
    struct epoll_event ev;
    ev.data.fd = sock;
    ev.events = EPOLLIN;
    
    return !epoll_ctl(epollfd, EPOLL_CTL_ADD, sock, &ev);
}

bool SubReactor::add2Conncts(socket_type sock)
{
    Connection* conn = new Connection();
    if (conn == nullptr){
        return false;
    }
    
    conn->createBuffer();
    conn->setSocket(sock);
    m_sConns.insert(conn);
    
    m_iConnectCount++;
    return true;
}


}
