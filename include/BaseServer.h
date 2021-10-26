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

    virtual ~BaseServer() {};
};



}

#endif

