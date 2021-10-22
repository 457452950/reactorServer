//
// Created by wlb on 2021/10/22.
//

#include "SubReactorMgr.h"

namespace wlb
{

SubReactorMgr::SubReactorMgr()
{
    m_iWorkThreadCount = 0;
    m_bRunning = false;
    m_vWorkThreads.clear();
    m_vWorker.clear();
}

SubReactorMgr::~SubReactorMgr()
{

}

bool SubReactorMgr::Initialize()
{
    if (m_iWorkThreadCount == 0)
        m_iWorkThreadCount = 1;
    
    if (m_iWorkThreadCount <= 0)
        return false;
    
    for (int nIndex = 0; nIndex < m_iWorkThreadCount; ++nIndex)
    {
        SubReactor* worker = new SubReactor();
        if (!worker->Initialize())
        {
            return false;
        }
        m_vWorker.push_back(worker);
    }
    
    m_bRunning = true;
    return true;
}

void SubReactorMgr::setWorkThradCount(uint count)
{
    m_iWorkThreadCount = count;
}

void SubReactorMgr::run()
{
    if (m_iWorkThreadCount != m_vWorkThreads.size())
    {
        return;
    }
    
    for (int nIndex = 0; nIndex < m_iWorkThreadCount; ++nIndex)
    {
        m_vWorkThreads.emplace_back(&SubReactor::Run, m_vWorker[nIndex]);
    }
}

void SubReactorMgr::stop()
{
    m_bRunning = false;
    for (auto worker : m_vWorker)
    {
        worker->Stop();
    }
}

void SubReactorMgr::waitToExit()
{
    for ( auto& worker : m_vWorkThreads)
    {
        if (worker.joinable())
            worker.join();
    }
}

void SubReactorMgr::release()
{

}

//
bool SubReactorMgr::insertSocket(SubReactor::socket_type sock)
{
    m_vWorker[0]->pushSocket(sock);
    return true;
}


}
