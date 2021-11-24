#ifndef BASE_BUFFER__
#define BASE_BUFFER__

#include "../include/HEAD.h"

class BaseBuffer
{
private:
    /* data */
public:
    virtual ~BaseBuffer();

    virtual bool Initialize(uint32_t maxBufferSize, uint32_t maxMessageSize) = 0;
public:
    virtual uint32_t readNextMessage(std::string& msg) = 0;


};



#endif
