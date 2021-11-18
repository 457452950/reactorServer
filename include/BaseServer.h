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
    inline virtual uint SetMaxBufferSize() {
        return 512 * 1024U;
    }

    virtual ~BaseServer() {};
};



}

#endif

