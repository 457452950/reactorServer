//
// Created by wlb on 2021/10/15.
//

#include "SubReactor.h"

namespace wlb
{

SubReactor::SubReactor()
{
    this->epollfd = -1;
    this->m_iEpollTimeOut = 0;
    this->m_mapConns.clear();

    this->m_bRunning = false;
    this->m_iConnectCount.store(0);

    this->m_pServer = nullptr;      
}

SubReactor::~SubReactor()
{
    if (this->epollfd != -1)
    {
        ::close(this->epollfd);
    }
    this->m_pServer = nullptr;      // 不由自己构造，不负责析构
}

bool SubReactor::Initialize(ReactorServer* server)
{
    this->m_pServer = server;
    if (this->m_pServer == nullptr)
    {
        return false;
    }
    
    this->epollfd = epoll_create(1);
    if (this->epollfd == -1){
        return false;
    }
    
    this->m_bRunning = true;
    this->m_iEpollTimeOut = this->m_pServer->getWorkEpollTimeOut();

    return true;
}

bool SubReactor::pushSocket(ClientData* clientData)
{
    if ( !this->add2Conncts(clientData) ){
        LOG(ERROR) << "cant add to conncts ";
        return false;
    }
    if ( !this->add2Epoll(clientData->sock))
    {
        LOG(ERROR) << "cant add to epoll ";
        return false;
    }
    
    return true;
}

void SubReactor::Run()
{
    struct epoll_event events[MAXEVENTS];
    
    while (this->m_bRunning)
    {
        int infds = ::epoll_wait(this->epollfd, events, MAXEVENTS, this->m_iEpollTimeOut);
        
        if(infds < 0)   // 返回失败
        {
            LOG(ERROR) << "epoll_wait() failed. errno :" << errno;
            break;
        }
        
        if (infds == 0)     // 超时
        {
            // LOG(INFO) << "epoll_wait() timeout.";
            continue;
        }
    
        for (int nIndex = 0; nIndex < infds; ++nIndex)
        {
            bool ok = this->ReadDataFromEvents(events[nIndex]);
            if (!ok)
            {
                this->RemoveAndCloseConn(events[nIndex]);
                continue;
            }
        }
    }
    LOG(INFO) << "out of work ,thread stop ";
}

bool SubReactor::add2Epoll(socket_type sock)
{
    struct epoll_event ev;
    try
    {
        ::memset(&ev, 0, sizeof(struct epoll_event));
    }
    catch(const std::exception& e)
    {
        LOG(ERROR) << e.what();
    }
    
    ev.data.fd = sock;
    ev.events = EPOLLIN;
    
    return !::epoll_ctl(epollfd, EPOLL_CTL_ADD, sock, &ev);
}

bool SubReactor::add2Conncts(ClientData* clientData)
{
    BaseSession* conn = new(std::nothrow) Connection();
    if (conn == nullptr){
        return false;
    }
    
    if ( !conn->Initialize(clientData, this->m_pServer->getMaxBufferSize(), this->m_pServer->getMaxBufferSize()))
    {
        return false;
    }
    
    {
        std::lock_guard<std::mutex> l(this->m_mMutex);
        m_mapConns.insert(std::make_pair(clientData->sock, conn));
    }
    this->m_iConnectCount++;

    this->m_pServer->onConnected(conn);

    return true;
}

void SubReactor::Stop()
{
    this->m_bRunning = false;
}

bool SubReactor::ReadDataFromEvents(epoll_event& event)
{
    if (event.events & EPOLLIN)
    {
        auto iter = m_mapConns.find(event.data.fd);
        if ( iter == m_mapConns.end())
        {
            return false;
        }
        
        auto conn = iter->second;

        bool ok = conn->recv();
    
        // 发生了错误或socket被对方关闭
        if (!ok)
        {
            LOG(INFO) << "client eventfd(" << event.data.fd << ") disconnected";
            m_pServer->onDisConnected(conn);
            return false;
        }
        
        return this->m_pServer->onMessage(conn);
    }
    
    return true;
}

bool SubReactor::RemoveAndCloseConn(epoll_event& event)
{
    LOG(INFO) << "close connect";

    struct epoll_event ev;
    
    // 把已断开的客户端从epoll中删除
    try
    {
        ::memset(&ev, 0, sizeof(struct epoll_event));
        ev.data.fd = event.data.fd;
        ev.events = EPOLLIN;
        
        ::epoll_ctl(this->epollfd, EPOLL_CTL_DEL, event.data.fd, &ev);
        ::close(event.data.fd);
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return false;
    }
    
    // 从容器中删除连接
    auto ite = this->m_mapConns.find(event.data.fd);
    if (ite == this->m_mapConns.end())
    {
        return true;
    }
    
    delete(ite->second);

    std::lock_guard<std::mutex> l(this->m_mMutex);
    this->m_mapConns.erase(ite);
    
    return true;
}


}
