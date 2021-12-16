#include "../include/DEFINE.h"
#include "BaseSession.h"

namespace wlb
{
    
using namespace rs;


class FixedBufferSession : public BaseSession
{
private:
    /* data */
public:
    FixedBufferSession(/* args */);
    ~FixedBufferSession();

// Based BaseConnection
    void send(const char* msg, uint msg_size) override;
    void send(const std::string& msg) override;

    char* getErrorStr() override;
    char* getPeerIp() override;
    int  getPeerPort() override;

//  Based BaseSession
    bool Initialize(ClientData* clientData, 
                    uint32_t maxBufferSize, 
                    uint32_t maxMessageSize) override;
    bool setSocket(socket_type sock) override;

    bool recv() override;

    int32_t readNextMessage(std::string& message) override;
};


} // namespace wlb
