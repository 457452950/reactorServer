//
// Created by wlb on 2021/10/22.
//

#ifndef REACTORSERVER_SUBREACTORMGR_H
#define REACTORSERVER_SUBREACTORMGR_H

#include "HEAD.h"
#include "DEFINE.h"
#include "SubReactor.h"
#include "ReactorServer.h"

namespace wlb
{

class SubReactorMgr
{
public:
    SubReactorMgr();
    ~SubReactorMgr();
    
    bool Initialize(ReactorServer* server, unsigned int workThreadCount);
    
    void run();
    void stop();
    void waitToExit();
    
    bool insertSocket(ClientDate* clientData);

private:
    std::vector<SubReactor*>    m_vWorker;
    std::vector<std::thread>    m_vWorkThreads;
    
    int                         m_iWorkThreadCount;
    bool                        m_bRunning;
    ReactorServer*              m_pServer;
};

}

#endif //REACTORSERVER_SUBREACTORMGR_H
