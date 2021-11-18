#ifndef __BASECONNECTION_H
#define __BASECONNECTION_H

#include "HEAD.h"

namespace wlb
{

class BaseConnection
{
public:
    virtual void send(const char* msg)          = 0;
    virtual void send(const std::string& msg)   = 0;

    virtual char* getErrorStr()                 = 0;
    virtual char* getPeerIp()                   = 0;
    virtual int  getPeerPort()                  = 0;


    virtual ~BaseConnection() {}
};








}

#endif
