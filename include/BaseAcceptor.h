//
// Created by wlb on 2021/10/10.
//

#ifndef REACTORSERVER_BASEACCEPTOR_H
#define REACTORSERVER_BASEACCEPTOR_H

#include "HEAD.h"

namespace wlb
{

class BaseAcceptor
{
public:
    using socket_type = int;
    using endpoint = sockaddr_in;

public:
    virtual ~BaseAcceptor() {}
    
    virtual socket_type getSocket() = 0;
    
};

BaseAcceptor* CreateAccepter(int port);

}

#endif //REACTORSERVER_BASEACCEPTOR_H
