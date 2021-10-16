//
// Created by wlb on 2021/10/12.
//

#ifndef REACTORSERVER_CONNECTION_H
#define REACTORSERVER_CONNECTION_H

#include "HEAD.h"
#include "DEFINE.h"


namespace wlb
{

class Connection
{
    using socket_type = int;
    using socket_ptr  = socket_type*;
    
public:
    Connection();
    ~Connection();
    
    bool setSocket(socket_type sock);
    bool createBuffer();
    
    
    inline socket_type getSocket(){
        return m_sSock;
    }
    
    inline void setBuffer (char* buf){
        m_pBuffer = buf;
    }
    inline void setBufferSize(unsigned int size){
        m_iBufferSize = size;
    }
    inline const unsigned int getBufferDefaultSize(){
        return DEFAULT_BUFFER_SIZE;
    }
    inline const unsigned int getBufferSize() {
        return m_iBufferSize;
    }
    
private:
    socket_type             m_sSock;
    char*                   m_pBuffer;
    static unsigned int     m_iBufferSize;
    
    bool                    m_bRunning;
};

}
#endif //REACTORSERVER_CONNECTION_H
