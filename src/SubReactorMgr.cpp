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
    for (int nIndex = 0; nIndex < m_iWorkThreadCount; ++nIndex)
    {
        delete m_vWorker[nIndex];
    }
}

bool SubReactorMgr::Initialize(ReactorServer* server, unsigned int workThreadCount)
{
    m_pServer = server;
    if (m_pServer == nullptr)
    {
        return false;
    }
    
    m_iWorkThreadCount = workThreadCount;
    if (m_iWorkThreadCount == 0)
        m_iWorkThreadCount = 1;
    
    if (m_iWorkThreadCount <= 0)
        return false;
    
    for (int nIndex = 0; nIndex < m_iWorkThreadCount; ++nIndex)
    {
        LOG(INFO) << "new worker ," << nIndex;
        SubReactor* worker = new SubReactor();
        if (!worker->Initialize(m_pServer))
        {
            return false;
        }
        m_vWorker.push_back(worker);
    }
    
    m_bRunning = true;
    return true;
}


void SubReactorMgr::run()
{
    if (m_iWorkThreadCount != m_vWorker.size())
    {
        LOG(ERROR) << "bad size " << m_iWorkThreadCount << "/" << m_vWorker.size();
        return;
    }
    
    for (int nIndex = 0; nIndex < m_iWorkThreadCount; ++nIndex)
    {
        LOG(INFO) << "start thread :" << nIndex;
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

//
bool SubReactorMgr::insertSocket(ClientData* clientData)
{
    fcntl(clientData->sock, F_SETFL, fcntl(clientData->sock, F_GETFL, 0) | O_NONBLOCK);
    int index = 0;
    LOG(INFO) << "put into no." << index;
    if ( !m_vWorker[index]->pushSocket(clientData) )
    {
        LOG(ERROR) << "put into NO." << index << " false";
        return false;
    }
    return true;
}


}
