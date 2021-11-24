//
// Created by wlb on 2021/10/10.
//

#ifndef REACTORSERVER_DEFINE_H
#define REACTORSERVER_DEFINE_H

#include "HEAD.h"

#define LISTEN_LIST_COUNT 1024
#define MAXEVENTS 1024

namespace wlb
{
namespace rs
{

using epoll_type  = int32_t;
using socket_type = int32_t;
using socket_ptr  = socket_type*;

using endpoint = sockaddr_in;



}

    
} // namespace wlb


#endif //REACTORSERVER_DEFINE_H
