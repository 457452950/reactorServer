//
// Created by wlb on 2021/10/12.
//

#include "Connection.h"


namespace wlb
{

unsigned int Connection::m_iBufferSize = DEFAULT_BUFFER_SIZE;

Connection::Connection() : m_sSock(-1)
{
    m_pBuffer = nullptr;
}

Connection::~Connection()
{

}

bool Connection::setSocket(socket_type sock)
{
    m_sSock = sock;
    if (m_sSock)
    return false;
}

bool Connection::createBuffer()
{
    m_pBuffer = new(std::nothrow) char(m_iBufferSize);
    if (m_pBuffer == nullptr)
    {
        return false;
    }
    return true;
}


}

