//
// Created by wlb on 2021/10/22.
//

#ifndef REACTORSERVER_SUBREACTORMGR_H
#define REACTORSERVER_SUBREACTORMGR_H

#include "HEAD.h"
#include "DEFINE.h"
#include "SubReactor.h"

namespace wlb
{

class SubReactorMgr
{
public:
    SubReactorMgr();
    ~SubReactorMgr();
    
    bool Initialize(unsigned int workThreadCount);
    
    void run();
    void stop();
    void waitToExit();
    void release();
    
    bool insertSocket(SubReactor::socket_type sock);

private:
    std::vector<SubReactor*>    m_vWorker;
    std::vector<std::thread>    m_vWorkThreads;
    
    int                         m_iWorkThreadCount;
    bool                        m_bRunning;

};

}

#endif //REACTORSERVER_SUBREACTORMGR_H
