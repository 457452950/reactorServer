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
    using endpoint = sockaddr_in;

public:
    virtual ~BaseAcceptor() {}
    
    virtual socket_type getSocket() = 0;
    
};

BaseAcceptor* createAcceptor(uint32_t port);

}

#endif //REACTORSERVER_BASEACCEPTOR_H
