//
// Created by wlb on 2021/10/10.
//

#include "MainReactor.h"

namespace wlb
{

MainReactor::MainReactor()
{
    this->epollfd = -1;
    this->m_iEpollTimeout = 0;
    this->accepts.clear();

    this->m_pMainThread = nullptr;
    this->m_iWorkThreadCount = 0;

    this->m_bRunning = false;
    this->m_pServer = nullptr;
}

bool MainReactor::Initialize(BaseServer* server, unsigned int threadCount)
{
    if (server == nullptr)
    {
        return false;
    }
    this->m_iWorkThreadCount = threadCount ? threadCount : 1;

    this->epollfd = epoll_create(1);
    if (this->epollfd < 0)
    {
        LOG(ERROR) << "epoll_create error ,errno : " << errno;
        return false;
    }
    
    this->m_pServer = new(std::nothrow) ReactorServer();
    if (this->m_pServer == nullptr)
    {
        return false;
    }
    if ( !this->m_pServer->Initialize(server) )
    {
        return false;
    }

    if ( !this->m_ReactorMgr.Initialize(this->m_pServer, threadCount) )
        return false;
    
    this->m_iEpollTimeout = this->m_pServer->getListenEpollTimeOut();

    return true;
}

MainReactor::~MainReactor()
{
    if (this->epollfd != -1){
        close(this->epollfd);
    }
    if (this->m_pMainThread != nullptr)
    {
        delete this->m_pMainThread;
        this->m_pMainThread = nullptr;
    }
    if (this->m_pServer != nullptr)
    {
        delete this->m_pServer;
        this->m_pServer = nullptr;
    }
}

void MainReactor::pushAcceptor(accept_ptr acceptor)
{
    this->accepts.push_back(acceptor);
}

void MainReactor::run()
{
    this->m_bRunning = true;
    
    for (auto iter : this->accepts)
    {
        struct epoll_event ev;
        ev.data.fd = iter->getSocket();
        ev.events = EPOLLIN;
        ::epoll_ctl(this->epollfd, EPOLL_CTL_ADD, iter->getSocket(), &ev);
    }
    LOG(INFO) << "epoll add ok ,size : " << this->accepts.size();
    
    if (this->accepts.empty())
        this->m_bRunning = false;
    
    // start Main Thread
    this->m_pMainThread = new(std::nothrow) std::thread(&MainReactor::runLoop, this);
    if (this->m_pMainThread == nullptr)
    {
        this->m_bRunning = false;
        return;
    }

    // start subreactor thread
    this->m_ReactorMgr.run();
}

void MainReactor::stop()
{
    this->m_bRunning = false;
    this->m_ReactorMgr.stop();
}

void MainReactor::runLoop()
{
    struct epoll_event events[MAXEVENTS];

    while (this->m_bRunning)
    {
        int infds = ::epoll_wait(this->epollfd, events, MAXEVENTS, this->m_iEpollTimeout);
        if(infds < 0)       // error 
        {
            LOG(ERROR) << "listen epoll failed ,errno" << errno;
            break;
        }
        if (infds == 0)     // 超时
        {
            continue;
        }
        
        for (int i = 0; i < infds; ++i)
        {
            endpoint client_endPoint;
            socklen_t len = sizeof(client_endPoint);
            
            socket_type clientsock = ::accept(
                    events[i].data.fd,
                    (struct sockaddr*)&client_endPoint,
                    &len);
            if (clientsock < 0)
            {
                LOG(WARNING) << "accept failed ,errno : " << errno << " socketfd = " << events[i].data.fd;
                continue;
            }

            struct ClientData* clientData = new ClientData;
            clientData->sock = clientsock;
            clientData->ipv4.IP = ::inet_ntoa(client_endPoint.sin_addr);
            clientData->ipv4.port = ::ntohs(client_endPoint.sin_port);
            
            LOG(INFO) << "new connection ,IP : "
                      << clientData->ipv4.IP
                      << " ,port : "
                      << clientData->ipv4.port;
            
            this->m_ReactorMgr.insertSocket(clientData);
            
        }
        
        if (this->accepts.empty())
            this->m_bRunning = false;
    }
}

void MainReactor::waitToExit()
{
    if (this->m_pMainThread->joinable())
        this->m_pMainThread->join();
    
    this->m_ReactorMgr.waitToExit();
}


}
