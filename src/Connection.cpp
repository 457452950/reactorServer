//
// Created by wlb on 2021/10/12.
//

#include "Connection.h"


namespace wlb
{

unsigned int Connection::s_iBufferSize = DEFAULT_BUFFER_SIZE;

Connection::Connection() : m_sSock(-1)
{
    m_pBuffer = nullptr;
    m_iReadOffset = 0;
    m_iRecvOffset = 0;

    m_bRunning = false;
    m_strErrorStr = nullptr;
}

Connection::~Connection()
{

}

bool Connection::setSocket(socket_type sock)
{
    m_sSock = sock;
    if (m_sSock <= 0)
    return false;
}
bool Connection::Initialize(ClientData* clientData)
{
    m_sSock = clientData->sock;
    m_sClientData.ipv4.IP = clientData->ipv4.IP;
    m_sClientData.ipv4.port = clientData->ipv4.port;

    if (m_sSock <= 0)
    {
        return false;
    }
    
    if (!createBuffer())
    {
        return false;
    }
    
    return true;
}

bool Connection::createBuffer()
{
    m_pBuffer = new(std::nothrow) char(s_iBufferSize);
    if (m_pBuffer == nullptr)
    {
        return false;
    }
    
    m_iReadOffset = 0;
    m_iRecvOffset = 0;
    
    return true;
}

void Connection::hasReadAndUpdata(uint size)
{
    m_iRecvOffset += size;
}

void Connection::send(const char* msg)
{
    ::send(this->m_sSock, (void*)msg, strlen(msg), 0);
}

void Connection::send(const std::string& msg)
{
    ::send(this->m_sSock, (void*)msg.c_str(), msg.size(), 0);
}


}

