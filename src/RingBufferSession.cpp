//
// Created by wlb on 2021/10/12.
//

#include "RingBufferSession.h"

namespace wlb
{
    using namespace Log;

RingBufferSession::RingBufferSession() : m_sSock(-1)
{
    this->m_pBuffer = nullptr;
    this->m_iBufferSize = 0;
    this->m_iReadOffset = 0;
    this->m_iRecvOffset = 0;

    this->m_bRunning = false;
    this->m_bIsFull = false;
    this->m_strErrorStr = nullptr;
}

RingBufferSession::~RingBufferSession()
{
    if (this->m_pBuffer != nullptr)
    {
        delete[] this->m_pBuffer;
        this->m_pBuffer = nullptr;
    }

    if (this->m_strErrorStr != nullptr)
    {
        delete this->m_strErrorStr;
        this->m_strErrorStr = nullptr;
    }
}

bool RingBufferSession::setSocket(socket_type sock)
{
    this->m_sSock = sock;
    if (this->m_sSock <= 0)
        return false;
}

bool RingBufferSession::Initialize(ClientDate *clientData, 
                            uint maxBufferSize, 
                            uint32_t maxMessageSize)
{
    this->m_sSock = clientData->sock;
    this->m_sClientDate.ipv4.IP = clientData->ipv4.IP;
    this->m_sClientDate.ipv4.port = clientData->ipv4.port;

    this->setBufferSize(maxBufferSize);

    if (this->m_sSock <= 0)
    {
        return false;
    }

    if (!this->createBuffer())
    {
        return false;
    }

    return true;
}

// 初始化存储
bool RingBufferSession::createBuffer()
{
    this->m_pBuffer = new (std::nothrow) char[m_iBufferSize];
    if (this->m_pBuffer == nullptr)
    {
        return false;
    }

    this->m_iReadOffset = 0;
    this->m_iRecvOffset = 0;

    return true;
}

bool RingBufferSession::recv()
{
    auto recvSize = ::recv(this->m_sSock,
                m_pBuffer+m_iRecvOffset,
                this->getRecvSize(),
                0);

    if (recvSize <= 0 && this->getRecvSize())
    {
        return false;
    }
    
    this->hasReadAndUpdata(recvSize);   // 更新 recv offset
    return true;
}

// 更新写指针位置
void RingBufferSession::hasReadAndUpdata(uint size)
{
    this->m_iRecvOffset = (this->m_iRecvOffset + size) % this->m_iBufferSize;
    if (this->m_iRecvOffset == this->m_iReadOffset)
    {
        this->m_bIsFull = true;
    }
    else
    {
        this->m_bIsFull = false;
    }
}

void RingBufferSession::send(const char *msg, uint msg_size)
{
    if (msg_size == 0)
    {
        LOG(L_WARN) << "msg cant be empty";
        return;
    }

    char *sendMsg = new (std::nothrow) char[msg_size + 4 + 1];
    uint32_t strLen = msg_size;
    ::memcpy(sendMsg, (void *)&strLen, 4);
    ::strcpy(sendMsg + 4, msg);
    ssize_t sendLen = ::send(this->m_sSock, (void *)sendMsg, msg_size + 4, 0);

    LOG(L_INFO) << "send size : " << sendLen << " str : " << msg;
}

void RingBufferSession::send(const std::string &msg)
{
    if (msg.empty())
    {
        LOG(L_WARN) << "msg cant be empty";
        return;
    }

    this->send(msg.c_str(), msg.size());
}

int RingBufferSession::readNextMessage(std::string &msg)
{
    uint16_t size;
    msg.clear();

    int pWrite = this->m_iRecvOffset;
    int pRead = this->m_iReadOffset;

    if (!this->m_bIsFull)
    {
        if ((pWrite - pRead < 4) || (this->m_iBufferSize - pRead + pWrite) <= 4)
        {
            LOG(L_INFO) << "no enough message to read";
            return 0;
        }
    }

    // read msg len
    this->getMsgSize(size);

    LOG(L_INFO) << "recv size : " << size;
    if (size == 0)
    {
        LOG(L_WARN) << "recv size <= 0!!!";
    }
    else if (pWrite > pRead)
    {
        // havent enough message
        if (size + 4 > pWrite - pRead)
        {
            return 0;
        }
        msg.assign(m_pBuffer + pRead + 4, size);
    }
    else if (pWrite <= pRead)
    {
        // havent enough message
        // size + 4 > (m_iBufferSize - pRead + pWrite + 1)
        if (pRead - pWrite + size + 4 > m_iBufferSize)
        {
            LOG(L_INFO) << "no enough message to read";
            return 0;
        }
        if (pRead + 4 >= m_iBufferSize)
        {
            int start = (pRead + 4) % m_iBufferSize;
            msg.append(m_pBuffer + start, size);
        }
        else
        {
            int back = (m_iBufferSize - pRead - 4 + 1 > size) ? size : m_iBufferSize - pRead - 4 + 1;
            LOG(L_DEBUG) << "back : " << back << " front : " << size - back;
            msg.append(m_pBuffer + pRead + 4, back);
            msg.append(m_pBuffer, size - back);
        }
        LOG(L_INFO) << "msg : " << msg;
    }

    // update 读指针
    m_iReadOffset = (pRead + size + 4) % m_iBufferSize;

    return size ? size : 1; // size为0时返回1
}

void RingBufferSession::getMsgSize(uint16_t &size)
{

    int pWrite = this->m_iRecvOffset;
    int pRead = this->m_iReadOffset;

    if (pRead + 4 <= this->m_iBufferSize)
    {
        // LOG(L_INFO) << "m_iReadOffset " <<  m_iReadOffset;
        memcpy(&size, m_pBuffer + pRead, 4);
    }
    else
    {
        char *temp = new (std::nothrow) char[4];
        if (temp == nullptr)
        {
            LOG(L_INFO) << "new error bad allocate";
            size = 0;
            return;
        }

        int back = this->m_iBufferSize - pRead;
        if (back > 4 || back < 0)
        {
            LOG(L_WARN) << "error pRead:" << pRead << " buff size:" << m_iBufferSize;
            size = 0;
            return;
        }

        memcpy(temp, m_pBuffer + pRead, back);
        memcpy(temp + back, m_pBuffer, 4 - back);
        memcpy(&size, temp, 4);
    }
}

uint RingBufferSession::getRecvSize()
{
    if (m_bIsFull == true) // 存储已满
    {
        return 0;
    }
    if (m_iRecvOffset >= m_iReadOffset)
    {
        return m_iBufferSize - m_iRecvOffset;
    }
    else if (m_iRecvOffset < m_iReadOffset)
    {
        return m_iReadOffset - m_iRecvOffset;
    }
}

}
