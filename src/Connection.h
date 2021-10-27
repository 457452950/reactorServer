//
// Created by wlb on 2021/10/12.
//

#ifndef REACTORSERVER_CONNECTION_H
#define REACTORSERVER_CONNECTION_H

#include "HEAD.h"
#include "DEFINE.h"
#include "BaseConnection.h"


namespace wlb
{

    struct ClientData
    {
        using socket_type = int;

        socket_type sock;
        struct _ipv4
        {
            char* IP;
            uint port;
        }ipv4;
        struct _ipv6
        {
            char* IP;
            uint port;
        }ipv6;
        
    };

class Connection : public BaseConnection
{
public:
    using socket_type   = ClientData::socket_type;
    using socket_ptr    = socket_type*;

public:
    virtual void send(const char* msg) override;
    virtual void send(const std::string& msg) override;
    virtual char* getErrorStr() override{
        return m_strErrorStr;
    };
    virtual char* getPeerIp() override{
        return m_sClientData.ipv4.IP;
    };
    virtual int getPeerPort() override{
        return m_sClientData.ipv4.port;
    }
    
public:
    Connection();
    ~Connection();
    
    bool setSocket(socket_type sock);
    bool createBuffer();

    bool Initialize(ClientData* clientData);

    int readNextMessage(std::string& msg){
        uint16_t size;
        msg.clear();

        if ( m_iReadOffset == m_iRecvOffset || 
                (m_iRecvOffset - m_iReadOffset <= 4) ||
                (s_iBufferSize - m_iReadOffset) + (m_iRecvOffset) <= 4 )
        {
            LOG(INFO) << "no enough message to read";
            return 0;
        }
        
        if ( m_iReadOffset + 4 <= s_iBufferSize )
            memcpy(&size, m_pBuffer+m_iReadOffset, 4);
        else
        {
            char* temp = new char[4];
            int back = s_iBufferSize - m_iReadOffset;
            memcpy(temp, m_pBuffer+m_iReadOffset, back);
            memcpy(temp+back, m_pBuffer, 4-back);
            memcpy(&size, temp, 4);
        }
        LOG(INFO) << "recv size : " << size
                   << " buf : " << m_pBuffer;

        if (m_iRecvOffset > m_iReadOffset)
        { 
            LOG(INFO) << "m_iRecvOffset >= m_iReadOffset";
            // havent enough message
            if (size + 4 > m_iRecvOffset - m_iReadOffset )
            {
                LOG(INFO) << "no enough message to read"
                    << "size : " << size
                    << " recv offset : " << m_iRecvOffset
                    << " read offset : " << m_iReadOffset;
                return 0;
            }
            msg.assign(m_pBuffer+m_iReadOffset+4,
                        size);
            m_iReadOffset += size + 4;
            LOG(INFO) << "msg : " << msg
                    << " recv offset : " << m_iRecvOffset
                    << " read offset : " << m_iReadOffset;
            return size;
        }
        else if (m_iRecvOffset < m_iReadOffset)
        {
            LOG(INFO) << "m_iRecvOffset < m_iReadOffset";
            // havent enough message
            // size + 4 > (s_iBufferSize - m_iReadOffset + m_iRecvOffset + 1)
            if (m_iReadOffset - m_iRecvOffset + size + 4
                                                > s_iBufferSize)
            {
                LOG(INFO) << "no enough message to read";
                return 0;
            }
            if ( m_iReadOffset + 4 >= s_iBufferSize )
            {
                int start = (m_iReadOffset + 4) % s_iBufferSize;
                msg.append(m_pBuffer + start, size);
            }
            else
            {
                int back = s_iBufferSize - m_iReadOffset - 4;
                msg.append(m_pBuffer + m_iReadOffset + 4, back);
                msg.append(m_pBuffer, size - back);
                LOG(DEBUG) << "back : " << back << " front " << size - back;
            }

            LOG(INFO) << "msg : " << msg;
            m_iReadOffset = (m_iReadOffset + size + 4) % s_iBufferSize;
            return size;
        }
        return false;
    }
    
    
    inline socket_type getSocket(){
        return m_sSock;
    }
    
    inline char * getBuffer(){
        return m_pBuffer;
    }
    inline uint getRecvOffset(){
        return m_iRecvOffset;
    }
    
    void        hasReadAndUpdata(uint size);
    uint        getRecvSize(){      // 最大可接收长度
        if (m_iRecvOffset >= m_iReadOffset)
        {
            return s_iBufferSize - m_iRecvOffset;
        }
        else if (m_iRecvOffset < m_iReadOffset)
        {
            return m_iReadOffset - m_iRecvOffset;
        }
        return 0;
    }
    
    inline void setBufferSize(unsigned int size){
        s_iBufferSize = size;
    }
    inline const unsigned int getBufferDefaultSize(){
        return DEFAULT_BUFFER_SIZE;
    }
    inline const unsigned int getBufferSize() {
        return s_iBufferSize;
    }
    
private:
    socket_type             m_sSock;
    char*                   m_pBuffer;
    static unsigned int     s_iBufferSize;
    uint                    m_iReadOffset;
    uint                    m_iRecvOffset;

    union _ClientData{
        struct _ipv4{
            char* IP;
            uint  port;
        }   ipv4;
        struct _ipv6{
            char* IP;
            uint  port;
        }   ipv6;
        
    }                       m_sClientData;
    
    bool                    m_bRunning;
    char*                   m_strErrorStr;
};

}
#endif //REACTORSERVER_CONNECTION_H
