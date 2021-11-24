#ifndef __BASESERVER_H
#define __BASESERVER_H

#include <BaseConnection.h>

namespace wlb
{

class BaseServer
{
public:
    virtual void onConnected(BaseConnection* connect) = 0;
    virtual void onDisConnected(BaseConnection* connect) = 0;
    virtual void onMessage(BaseConnection* connect, std::string& msg) = 0;

    // 重载来设置最大存储容量
    virtual uint SetMaxBufferSize() {
        return 512 * 1024U;
    }
    virtual int SetListenEpollTimeOut() {    // ms
        return -1;
    }
    virtual int SetWorkEpollTimeOut() {      // ms
        return -1;
    }

    virtual ~BaseServer() {};
};



}

#endif

