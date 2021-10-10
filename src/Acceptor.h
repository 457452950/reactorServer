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
    Acceptor(int port);
    ~Acceptor();

    inline socket_type getSocket()
    { return m_sSock; }
    
private:
    void setOpt();
    int bind(int port);

private:
    socket_type m_sSock;
    endpoint    m_endPoint;
};

}



#endif //REACTORSERVER_ACCEPTOR_H
