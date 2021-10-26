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
    m_mapConns.clear();
}

SubReactor::~SubReactor()
{
    if (epollfd != -1)
    {
        close(epollfd);
    }
}

bool SubReactor::Initialize(ReactorServer* server)
{
    m_pServer = server;
    if (m_pServer == nullptr)
    {
        return false;
    }
    
    epollfd = epoll_create(1);
    if (epollfd == -1){
        return false;
    }
    
    m_bRunning = true;
    return true;
}

bool SubReactor::pushSocket(ClientData* clientData)
{
    if ( !add2Conncts(clientData) ){
        LOG(ERROR) << "cant add to conncts ";
        return false;
    }
    if ( !add2Epoll(clientData->sock))
    {
        LOG(ERROR) << "cant add to epoll ";
        return false;
    }
    
    return true;
}

void SubReactor::Run()
{
    while (m_bRunning)
    {
        struct epoll_event events[MAXEVENTS];
        
        int infds = epoll_wait(epollfd, events, MAXEVENTS, EPOLL_TIME_OUT);
        
        if(infds < 0)   // 返回失败
        {
            LOG(ERROR) << "epoll_wait() failed.";
            break;
        }
        
        if (infds == 0)     // 超时
        {
            LOG(INFO) << "epoll_wait() timeout.";
            continue;
        }
    
        for (int nIndex = 0; nIndex < infds; ++nIndex)
        {
            bool ok = ReadDataFromEvents(events[nIndex]);
            if (!ok)
            {
                RemoveAndCloseConn(events[nIndex]);
                continue;
            }
        }
    }
    LOG(INFO) << "out of work ,thread stop ";
}

bool SubReactor::add2Epoll(socket_type sock)
{
    struct epoll_event ev;
    memset(&ev, 0, sizeof(struct epoll_event));
    ev.data.fd = sock;
    ev.events = EPOLLIN;
    
    return !epoll_ctl(epollfd, EPOLL_CTL_ADD, sock, &ev);
}

bool SubReactor::add2Conncts(ClientData* clientData)
{
    Connection* conn = new Connection();
    if (conn == nullptr){
        return false;
    }
    
    if ( !conn->Initialize(clientData))
    {
        return false;
    }
    
    m_mapConns.insert(std::make_pair(clientData->sock, conn));
    
    m_iConnectCount++;

    m_pServer->onConnected(conn);

    return true;
}

void SubReactor::Stop()
{
    m_bRunning = false;
}

bool SubReactor::ReadDataFromEvents(epoll_event& event)
{
    if (event.events & EPOLLIN)
    {
        auto conn = m_mapConns.find(event.data.fd)->second;
        
        auto recvSize = read(conn->getSocket(),
                             conn->getBuffer()+conn->getRecvOffset(),
                             conn->getRecvSize());
    
        // 发生了错误或socket被对方关闭
        if (recvSize <= 0 && conn->getRecvSize())
        {
            LOG(INFO) << "client eventfd(" << event.data.fd << ") disconnected";
            m_pServer->onDisConnected(conn);
            return false;
        }
        
        conn->hasReadAndUpdata(recvSize);
        m_pServer->onMessage(conn);
    }
    
    return true;
}

bool SubReactor::RemoveAndCloseConn(epoll_event& event)
{
    struct epoll_event ev;
    
    // 把已断开的客户端从epoll中删除
    memset(&ev, 0, sizeof(struct epoll_event));
    ev.data.fd = event.data.fd;
    ev.events = EPOLLIN;
    
    epoll_ctl(epollfd, EPOLL_CTL_DEL, event.data.fd, &ev);
    close(event.data.fd);
    
    //
    m_mapConns.erase(event.data.fd);
    
    return false;
}


}
