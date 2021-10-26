#ifndef __SERVER_H
#define __SERVER_H

#include "BaseServer.h"
#include "Connection.h"


namespace wlb
{
    


class ReactorServer
{
private:
    /* data */
public:
    ReactorServer() {}
    ~ReactorServer() {
        if (_server != nullptr)
        {
            delete _server;
            _server = nullptr;
        }
        
    }

    bool Initialize(BaseServer* server){
        _server = server;
        if (_server != nullptr){
            return true;
        }
        return false;
    }

    void onConnected(Connection* conn){
        _server->onConnected(conn);
    }
    void onDisConnected(Connection* conn){
        _server->onDisConnected(conn);
    }
    void onMessage(Connection* conn){
        std::string cMsg;
        while ( conn->readNextMessage(cMsg) )
        {
            _server->onMessage(conn, cMsg);
            cMsg.clear();
        }
    }

private:
    BaseServer* _server;
};

} // namespace wlb

#endif


