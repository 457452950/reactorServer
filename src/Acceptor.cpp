//
// Created by wlb on 2021/10/10.
//

#include "src/Acceptor.h"

namespace wlb
{

Acceptor::Acceptor(int port)
{
    m_sSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_sSock < 0)
    {
        LOG(ERROR) << "socket false ,errno : " << errno;
        return;
    }
    LOG(INFO) << "socket ok : " << m_sSock;
    
    setOpt();
    
    int res = bind(port);
    if (res < 0)
    {
        LOG(ERROR) << "bind failed ,errno : " << errno;
        close(m_sSock);
        return;
    }
    LOG(INFO) << "bind ok !";
    
    if (listen(m_sSock, LISTEN_LIST_COUNT) != 0)
    {
        LOG(ERROR) << "listen failed ,errno : " << errno;
        close(m_sSock);
        return;
    }
    LOG(INFO) << "listen ok !";
}

Acceptor::~Acceptor()
{
    close(m_sSock);
}

void Acceptor::setOpt()
{
    int opt = 1;
    unsigned int len = sizeof(opt);
    setsockopt(m_sSock, SOL_SOCKET, SO_REUSEADDR, &opt, len);
    setsockopt(m_sSock, SOL_SOCKET, SO_KEEPALIVE, &opt, len);
}

int Acceptor::bind(int port)
{
    m_endPoint.sin_family       = AF_INET;
    m_endPoint.sin_addr.s_addr  = htonl(INADDR_ANY);
    m_endPoint.sin_port         = htons(port);
    
    return ::bind(m_sSock, (struct sockaddr * )&m_endPoint, sizeof(m_endPoint));
}

BaseAcceptor* CreateAccepter(int port)
{
    return new Acceptor(port);
}


}