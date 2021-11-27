#ifndef BASE_BUFFER__
#define BASE_BUFFER__

#include "../include/DEFINE.h"

namespace wlb
{

using namespace rs;

class BaseSession : public BaseConnection
{
public:
    virtual ~BaseSession() { };

    virtual bool Initialize(ClientData* clientData, uint32_t maxBufferSize, uint32_t maxMessageSize) = 0;
    virtual bool setSocket(socket_type sock) = 0;

    // 接收消息
    virtual bool recv() = 0;

    // 读取下一条消息、返回消息长度
    virtual int32_t readNextMessage(std::string& msg) = 0;

};


}
#endif
