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
using namespace rs;

struct ClientData
{
    socket_type sock;
    struct _ipv4
    {
        char* IP;
        uint port;
    }ipv4;
    struct _ipv6                // 保留空间 可做联合体
    {
        char* IP;
        uint port;
    }ipv6;
      
};

class Connection : public BaseConnection
{
public:
    virtual void send(const char* msg, uint msg_size) override;
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
    bool Initialize(ClientData* clientData, uint32_t maxBufferSize = 512*1024U);

    // 接收
    bool recv();

    // 获取下一条信息
    int readNextMessage(std::string& msg);
private:
    bool createBuffer();
    
    void getMsgSize(uint16_t& size);
    
    // 接受数据，刷新写指针
    void        hasReadAndUpdata(uint size);
    
    // 最大可接收长度
    uint        getRecvSize();
    
    inline void setBufferSize(unsigned int size){
        m_iBufferSize = size;
    }
    
private:
    socket_type             m_sSock;
    char*                   m_pBuffer;
    unsigned int            m_iBufferSize;      // 缓冲区大小
    uint                    m_iReadOffset;      // 读指针、消费者指针
    uint                    m_iRecvOffset;      // 写指针、生产者指针

    // 对端信息
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
    
    bool                    m_bRunning;     // 暂未启用
    bool                    m_bIsFull;      // 状态值：缓冲区满
    char*                   m_strErrorStr;  // 错误信息
};

}
#endif //REACTORSERVER_CONNECTION_H
