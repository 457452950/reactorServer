#ifndef BASE_BUFFER__
#define BASE_BUFFER__

#include "../include/DEFINE.h"

namespace wlb
{

using namespace rs;

class BaseSession
{
public:
    virtual ~BaseSession();

    virtual bool Initialize(uint32_t maxBufferSize, uint32_t maxMessageSize) = 0;

    bool setSocket(socket_type sock);
    
public:
    virtual send(std::string& msg) { this->send(msg.c_str(), msg.size()); };
    virtual send(const char* msg, uint32_t msg_len) = 0;

    // 接收消息
    virtual bool recv() = 0;

    // 读取下一条消息、返回消息长度
    virtual uint32_t readNextMessage(std::string& msg) = 0;

protected:
    socket_type _socket;
    char*       _buf;
};


}
#endif
