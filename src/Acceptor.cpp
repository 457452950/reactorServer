//
// Created by wlb on 2021/10/10.
//

#include "Acceptor.h"

namespace wlb
{

using namespace Log;

Acceptor::Acceptor()
{
    this->m_sSock = -1;
}

Acceptor::~Acceptor()
{
    ::close(this->m_sSock);
}

bool Acceptor::Initialize(int port)
{
    this->m_sSock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);        // tcp v4
    if (m_sSock < 0)
    {
        LOG(L_ERROR) << "socket false ,errno : " << errno;
        return false;
    }
    LOG(L_INFO) << "socket ok : " << m_sSock;
    
    this->setOpt();
    
    int res = this->bind(port);
    if (res < 0)
    {
        LOG(L_ERROR) << "bind failed ,errno : " << errno;
        ::close(this->m_sSock);
        return false; 
    }
    LOG(L_INFO) << "bind ok !";
    
    if ( ::listen(this->m_sSock, LISTEN_LIST_COUNT) != 0 )
    {
        LOG(L_ERROR) << "listen failed ,errno : " << errno;
        ::close(this->m_sSock);
        return false;
    }
    LOG(L_INFO) << "listen ok !";
    return true;
}

void Acceptor::setOpt()
{
    int opt = 1;
    unsigned int len = sizeof(opt);
    ::setsockopt(this->m_sSock, SOL_SOCKET, SO_REUSEADDR, &opt, len);
    ::setsockopt(this->m_sSock, SOL_SOCKET, SO_REUSEPORT, &opt, len);
    ::setsockopt(this->m_sSock, SOL_SOCKET, SO_KEEPALIVE, &opt, len);

    ::fcntl(this->m_sSock, F_SETFL, ::fcntl(this->m_sSock, F_GETFL, 0) | O_NONBLOCK); // set no block
}

int Acceptor::bind(int port)
{
    this->m_endPoint.sin_family       = AF_INET;
    this->m_endPoint.sin_addr.s_addr  = ::htonl(INADDR_ANY);
    this->m_endPoint.sin_port         = ::htons(port);
    
    return ::bind(this->m_sSock, 
                (struct sockaddr * )&(this->m_endPoint), 
                sizeof(this->m_endPoint));
}

BaseAcceptor* CreateAccepter(int port)  // 对外接口
{
    Acceptor* _accept = new(std::nothrow) Acceptor;
    if ( !_accept->Initialize(port) )
    {
        return nullptr;
    }
    
    return _accept;
}


}
