//
// Created by wlb on 2021/10/15.
//

#ifndef REACTORSERVER_SUBREACTOR_H
#define REACTORSERVER_SUBREACTOR_H

#include "HEAD.h"
#include "DEFINE.h"
#include "Connection.h"

namespace wlb
{


class SubReactor
{
public:
    using socket_type = int;
    using socket_ptr = socket_type*;
    
    using epoll_type = int;
    
    using Connects = std::set<Connection*>;
    using ConnectMap = std::map<socket_type, Connection*>;

public:
    SubReactor();
    ~SubReactor();
    
    bool Initialize();
    void Run();
    void Stop();

    bool pushSocket(socket_type sock);

private:
    bool add2Conncts(socket_type sock);
    bool add2Epoll(socket_type sock);
    bool ReadDataFromEvents(epoll_event& event);
    bool RemoveAndCloseConn(epoll_event& event);
    
private:
    epoll_type          epollfd;
    ConnectMap          m_mapConns;
    
    std::mutex          m_mMutex;
    bool                m_bRunning;
    std::atomic_uint    m_iConnectCount;
};


}

#endif //REACTORSERVER_SUBREACTOR_H
