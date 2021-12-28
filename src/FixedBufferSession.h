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
    FixedBufferSession(const FixedBufferSession& right) = delete;
    FixedBufferSession& operator=(const FixedBufferSession& right) = delete;
    ~FixedBufferSession();

// Based BaseConnection
    void send(const char* msg, uint msg_size) override;
    void send(const std::string& msg) override;

    char* getErrorStr() override;
    char* getPeerIp() override;
    int  getPeerPort() override;

//  Based BaseSession
    bool Initialize(ClientDate* clientDate, 
                    uint32_t maxBufferSize, 
                    uint32_t maxMessageSize) override;
    bool setSocket(socket_type sock) override;

    bool recv() override;

    int32_t readNextMessage(std::string& message) override;

    // private methos
    uint32_t getMaxRecvSize();

private:
    // session config
    uint32_t m_uBufferSize;
    uint32_t m_uMaxMessageSize;

    ClientDate m_ClientDate;

    socket_type m_Socket{0};
    char* m_pBuffer{nullptr};

    uint32_t m_uWriteOffset{0};
    uint32_t m_uReadOffset{0};
    bool m_bBuffIsFull{false};
};


} // namespace wlb
