//
// Created by wlb on 2021/10/10.
//

#ifndef REACTORSERVER_ACCEPTOR_H
#define REACTORSERVER_ACCEPTOR_H

#include "HEAD.h"
#include "DEFINE.h"
#include "BaseAcceptor.h"

namespace wlb
{

class Acceptor : public BaseAcceptor
{
public:
    Acceptor();             // bind and listen
    ~Acceptor();

    // 初始化
    bool Initialize(int port);

    inline socket_type getSocket()
    { return m_sSock; }
    
private:
    void setOpt();                  // 设置套接字选项
    int bind(int port);             // bind

private:
    socket_type m_sSock;        // 监听套接字
    endpoint    m_endPoint;     // 对端信息
};

}



#endif //REACTORSERVER_ACCEPTOR_H
