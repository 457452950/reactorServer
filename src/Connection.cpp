//
// Created by wlb on 2021/10/12.
//

#include "Connection.h"


namespace wlb
{


Connection::Connection() : m_sSock(-1)
{
    m_pBuffer = nullptr;
    m_iReadOffset = 0;
    m_iRecvOffset = 0;

    m_bRunning = false;
    m_bIsFull = false;
    m_strErrorStr = nullptr;
}

Connection::~Connection()
{
    if (m_pBuffer != nullptr){
        delete[] m_pBuffer;
        m_pBuffer = nullptr;
    }
}

bool Connection::setSocket(socket_type sock)
{
    m_sSock = sock;
    if (m_sSock <= 0)
    return false;
}
bool Connection::Initialize(ClientData* clientData, uint maxBufferSize)
{
    m_sSock = clientData->sock;
    m_sClientData.ipv4.IP = clientData->ipv4.IP;
    m_sClientData.ipv4.port = clientData->ipv4.port;

    this->setBufferSize(maxBufferSize);

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

// 初始化存储
bool Connection::createBuffer()
{
    m_pBuffer = new(std::nothrow) char[m_iBufferSize];
    if (m_pBuffer == nullptr)
    {
        return false;
    }
    
    m_iReadOffset = 0;
    m_iRecvOffset = 0;
    
    return true;
}

// 更新 recv offset 位置
void Connection::hasReadAndUpdata(uint size)
{
    m_iRecvOffset = (m_iRecvOffset + size) % m_iBufferSize;
    if (m_iRecvOffset == m_iReadOffset)
    {
        m_bIsFull = true;
    }
    else
    {
        m_bIsFull = false;
    }
    
}

void Connection::send(const char* msg)
{
    if (strlen(msg) <= 0)
    {
        return;
    }
    
    char* sendMsg = new char[strlen(msg) + 4 + 1];
    uint32_t strLen = strlen(msg);
    memcpy(sendMsg, (void*)&strLen, 4);
    strcpy(sendMsg + 4, msg);
    ssize_t sendLen = ::send(this->m_sSock, (void*)sendMsg, strlen(msg) + 4, 0);
}

void Connection::send(const std::string& msg)
{
    if (msg.empty())
    {
        return;
    }
    

    this->send(msg.c_str());
}


}

