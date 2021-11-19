//
// Created by wlb on 2021/10/10.
//

#include "MainReactor.h"

namespace wlb
{

MainReactor::MainReactor()
{
    epollfd = -1;
    m_iEpollTimeout = 0;
    accepts.clear();

    m_pMainThread = nullptr;
    m_iWorkThreadCount = 0;

    m_bRunning = false;
    m_pServer = nullptr;
}
bool MainReactor::Initialize(BaseServer* server, unsigned int threadCount)
{
    m_iWorkThreadCount = threadCount;

    epollfd = epoll_create(1);
    if (epollfd < 0)
    {
        LOG(ERROR) << "epoll_create error ,errno : " << errno;
        return false;
    }
    
    m_pServer = new ReactorServer();
    if (m_pServer == nullptr)
    {
        return false;
    }
    if ( !m_pServer->Initialize(server) )
    {
        return false;
    }

    if ( !m_ReactorMgr.Initialize(m_pServer, threadCount) )
        return false;
    
    m_iEpollTimeout = m_pServer->getListenEpollTimeOut();

    return true;
}

MainReactor::~MainReactor()
{
    if (epollfd != -1){
        close(epollfd);
    }
    if (m_pMainThread != nullptr)
    {
        delete m_pMainThread;
        m_pMainThread = nullptr;
    }
    if (m_pServer != nullptr)
    {
        delete m_pServer;
        m_pServer = nullptr;
    }
}


void MainReactor::pushAcceptor(accept_ptr acceptor)
{
    accepts.push_back(acceptor);
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
    
    if (accepts.empty())
        m_bRunning = false;
    
    LOG(INFO) << "Main thread start.";
    // start Main Thread
    m_pMainThread = new std::thread(&MainReactor::runLoop, this);
    
    LOG(INFO) << "Sub thread start.";
    // start subreactor thread
    m_ReactorMgr.run();
}

void MainReactor::stop()
{
    m_bRunning = false;
    m_ReactorMgr.stop();
}

void MainReactor::runLoop()
{
    struct epoll_event events[MAXEVENTS];

    while (m_bRunning)
    {
        int infds = epoll_wait(epollfd, events, MAXEVENTS, m_iEpollTimeout);
        if(infds < 0)
        {
            LOG(ERROR) << "epoll_wait failed ";
            break;
        }
        if (infds == 0)     // 超时
        {
            LOG(INFO) << "epoll_wait() timeout";
            continue;
        }
        
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

            struct ClientData* clientData = new ClientData;
            clientData->sock = clientsock;
            clientData->ipv4.IP = inet_ntoa(client_endPoint.sin_addr);
            clientData->ipv4.port = ntohs(client_endPoint.sin_port);
            
            LOG(INFO) << "new connection ,IP : "
                      << clientData->ipv4.IP
                      << " ,port : "
                      << clientData->ipv4.port;
            
            m_ReactorMgr.insertSocket(clientData);
            
        }
        
        if (accepts.empty())
            m_bRunning = false;
    }
}

void MainReactor::waitToExit()
{
    if (m_pMainThread->joinable())
        m_pMainThread->join();
    
    m_ReactorMgr.waitToExit();
}


}
