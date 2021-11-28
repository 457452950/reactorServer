//
// Created by wlb on 2021/10/22.
//

#include "SubReactorMgr.h"

namespace wlb
{
    using namespace Log;

SubReactorMgr::SubReactorMgr()
{
    this->m_iWorkThreadCount = 0;
    this->m_bRunning = false;
    this->m_vWorkThreads.clear();
    this->m_vWorker.clear();
}

SubReactorMgr::~SubReactorMgr()
{
    for (int nIndex = 0; nIndex < m_iWorkThreadCount; ++nIndex)
    {
        delete this->m_vWorker[nIndex];
        this->m_vWorker[nIndex] = nullptr;
    }
}

bool SubReactorMgr::Initialize(ReactorServer* server, unsigned int workThreadCount)
{
    this->m_pServer = server;
    if (this->m_pServer == nullptr)
    {
        return false;
    }
    
    this->m_iWorkThreadCount = workThreadCount ? workThreadCount : 1;
    
    for (int nIndex = 0; nIndex < this->m_iWorkThreadCount; ++nIndex)
    {
        LOG(INFO) << "new worker ," << nIndex;
        SubReactor* worker = new(std::nothrow) SubReactor();
        if ( worker == nullptr || !worker->Initialize(m_pServer))
        {
            return false;
        }
        this->m_vWorker.push_back(worker);
    }
    
    this->m_bRunning = true;
    return true;
}


void SubReactorMgr::run()
{
    if (this->m_iWorkThreadCount != this->m_vWorker.size())
    {
        LOG(ERROR) << "bad size " << m_iWorkThreadCount << "/" << m_vWorker.size();
        return;
    }
    
    for (int nIndex = 0; nIndex < this->m_iWorkThreadCount; ++nIndex)
    {
        LOG(INFO) << "start thread :" << nIndex;
        this->m_vWorkThreads.emplace_back(&SubReactor::Run, m_vWorker[nIndex]);
    }
}

void SubReactorMgr::stop()
{
    this->m_bRunning = false;
    for (auto worker : this->m_vWorker)
    {
        worker->Stop();
    }
}

void SubReactorMgr::waitToExit()
{
    for ( auto& worker : this->m_vWorkThreads)
    {
        if (worker.joinable())
            worker.join();
    }
}

// 需要做负载均衡
bool SubReactorMgr::insertSocket(ClientData* clientData)
{
    ::fcntl(clientData->sock, F_SETFL, ::fcntl(clientData->sock, F_GETFL, 0) | O_NONBLOCK);
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
