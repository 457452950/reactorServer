#include "FixedBufferSession.h"

namespace wlb
{

    FixedBufferSession::FixedBufferSession(/* args */)
    {
    }

    FixedBufferSession::~FixedBufferSession()
    {
    }

    // Based BaseConnection
    void FixedBufferSession::send(const char *msg, uint msg_size) {}
    void FixedBufferSession::send(const std::string &msg) {}

    char *FixedBufferSession::getErrorStr() {}
    char *FixedBufferSession::getPeerIp() {}
    int FixedBufferSession::getPeerPort() {}

    //  Based BaseSession
    bool FixedBufferSession::Initialize(ClientData *clientData,
                                        uint32_t maxBufferSize,
                                        uint32_t maxMessageSize) {}
    bool FixedBufferSession::setSocket(socket_type sock) {}

    bool FixedBufferSession::recv() {}

    int32_t FixedBufferSession::readNextMessage(std::string &message) {}

} // namespace wlb
