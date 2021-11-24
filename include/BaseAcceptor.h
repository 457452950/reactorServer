//
// Created by wlb on 2021/10/10.
//

#ifndef REACTORSERVER_BASEACCEPTOR_H
#define REACTORSERVER_BASEACCEPTOR_H

#include "DEFINE.h"

namespace wlb
{

using namespace rs;

class BaseAcceptor
{
public:
    virtual ~BaseAcceptor() {}
    
    virtual socket_type getSocket() = 0;
    
};

BaseAcceptor* CreateAccepter(int32_t port);         // 获取监听者

}

#endif //REACTORSERVER_BASEACCEPTOR_H
