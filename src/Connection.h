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
    
    
    inline socket_type getSocket(){
        return m_sSock;
    }
    
    inline char * getBuffer(){
        return m_pBuffer;
    }
    inline uint getReadOffset(){
        return m_iReadOffset;
    }
    inline uint getRecvOffset(){
        return m_iRecvOffset;
    }
    
    void        hasReadAndUpdata(uint size);
    
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
