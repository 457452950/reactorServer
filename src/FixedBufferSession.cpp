#include "FixedBufferSession.h"


namespace wlb
{
using namespace Log;

    FixedBufferSession::FixedBufferSession(/* args */)
    {
    }

    FixedBufferSession::~FixedBufferSession()
    {
        if (m_Socket != 0)
        {
            ::close(m_Socket);
            m_Socket = 0;
        }
        
    }

    // Based BaseConnection
    void FixedBufferSession::send(const char *msg, uint msg_size) 
    {
        ::send(m_Socket, msg, msg_size, 0);
    }
    void FixedBufferSession::send(const std::string &msg) 
    {
        this->send(msg.c_str(), msg.size());
    }

    char *FixedBufferSession::getErrorStr() {}
    char *FixedBufferSession::getPeerIp() 
    {
        return m_ClientDate.ipv4.IP;
    }
    int FixedBufferSession::getPeerPort() 
    {
        return m_ClientDate.ipv4.port;
    }

    //  Based BaseSession
    bool FixedBufferSession::Initialize(ClientDate *clientData,
                                        uint32_t maxBufferSize,
                                        uint32_t maxMessageSize) 
    {
        m_ClientDate = *clientData;

        if ( !this->setSocket(clientData->sock) )
            return false;
        m_uBufferSize = maxBufferSize;
        m_uMaxMessageSize = maxMessageSize;

        m_pBuffer = new(std::nothrow) char[m_uBufferSize];
        if (m_pBuffer == nullptr)
        {
            LOG(L_ERROR) << "new char[] false";
            return false;
        }
        return true;
    }

    bool FixedBufferSession::setSocket(socket_type sock) 
    {
        m_Socket = sock;
        if (m_Socket == 0)
        {
            LOG(L_ERROR) << "set socket false" << static_cast<int32_t>(sock);
            return false;
        }
        return true;
    }

    bool FixedBufferSession::recv() 
    {
        auto recv_len = ::recv(m_Socket, m_pBuffer + m_uWriteOffset, this->getMaxRecvSize(), 0);
        LOG(L_DEBUG) << "recv len: " << recv_len;
        if (recv_len != 0 || this->getMaxRecvSize() == 0)
        {
            m_uWriteOffset = (m_uWriteOffset + recv_len) % m_uBufferSize;
            if (m_uWriteOffset == m_uReadOffset)
            {
                m_bBuffIsFull = true;
            }
            return true;
        }
        return false;
    }

    int32_t FixedBufferSession::readNextMessage(std::string &message) 
    {
        message.clear();

        if (m_uWriteOffset > m_uReadOffset)
        {
            if ((m_uWriteOffset - m_uReadOffset) >= m_uMaxMessageSize)
            {
                message.append(m_pBuffer + m_uReadOffset, m_uMaxMessageSize);
            }
            else
                return 0;
        }
        else if ((m_uWriteOffset < m_uReadOffset) || m_bBuffIsFull) // m_uWriteOffset <= m_uReadOffset
        {
            uint32_t back = m_uBufferSize - m_uReadOffset;
            uint32_t front = m_uWriteOffset;
            if ((back + front) >= m_uMaxMessageSize)
            {
                message.append(m_pBuffer + m_uReadOffset, back);
                message.append(m_pBuffer, front);
            }
            else
                return 0;
        }
        else
        {
            return 0;
        }

        m_uReadOffset = (m_uReadOffset + m_uMaxMessageSize) % m_uBufferSize;
        LOG(L_DEBUG) << "readoffset : " << m_uReadOffset << " writeoffset : " << m_uWriteOffset;
        m_bBuffIsFull = false;
        return m_uMaxMessageSize;
    }

    
    uint32_t FixedBufferSession::getMaxRecvSize()
    {
        if (m_bBuffIsFull)
        {
            return 0;
        }
        else
        {
            if (m_uWriteOffset >= m_uReadOffset)
            {
                return m_uBufferSize - m_uWriteOffset;
            }
            else        // m_uWriteOffset < m_uReadOffset
            {
                return m_uReadOffset - m_uWriteOffset;
            }
        }
    }

} // namespace wlb
