//
// Created by wlb on 2021/10/15.
//

#ifndef REACTORSERVER_SUBREACTOR_H
#define REACTORSERVER_SUBREACTOR_H

#include "HEAD.h"
#include "DEFINE.h"
#include "RingBufferSession.h"
#include "FixedBufferSession.h"
#include "ReactorServer.h"

namespace wlb
{

using namespace rs;

class SubReactor
{
public:
    
    using Connects = std::set<BaseSession*>;
    using ConnectMap = std::map<socket_type, BaseSession*>;

public:
    SubReactor();
    ~SubReactor();
    
    bool Initialize(ReactorServer*);
    void Run();
    void Stop();

    bool pushSocket(ClientDate* clientData);        // 插入连接

private:
    bool add2Conncts(ClientDate* clientData);
    bool add2Epoll(socket_type sock);

    bool ReadDataFromEvents(epoll_event& event);
    bool RemoveAndCloseConn(epoll_event& event);
    
private:
    epoll_type          epollfd;                // 采用epoll
    int                 m_iEpollTimeOut;
    ConnectMap          m_mapConns;             // 连接池
    
    std::mutex          m_mMutex;               // stl 操作时上锁
    bool                m_bRunning;
    std::atomic_uint    m_iConnectCount;        // 连接数 

    ReactorServer*      m_pServer;              // 不负责析构
};


}

#endif //REACTORSERVER_SUBREACTOR_H
