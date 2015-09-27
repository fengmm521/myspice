//
//  WorkerDeviceManger.cpp
//  myspice
//
//  Created by woodcol on 15/9/26.
// 当前电路图中的元器件管理类
//

#include "WorkerDeviceManger.h"
#include "DeviceShowBase.h"
#include "ModeNetLine.h"
#include "ModeNetNode.h"

static WorkerDeviceManger *s_sharedWorkerDeviceManger = nullptr;

WorkerDeviceManger *WorkerDeviceManger::sharedWorkerDeviceManger()
{
    if (s_sharedWorkerDeviceManger == nullptr)
    {
        s_sharedWorkerDeviceManger = new WorkerDeviceManger();
        if (!s_sharedWorkerDeviceManger || !s_sharedWorkerDeviceManger->init())
        {
            CC_SAFE_DELETE(s_sharedWorkerDeviceManger);
        }
    }
    return s_sharedWorkerDeviceManger;
}

void WorkerDeviceManger::destroyInstance()
{
    
    CC_SAFE_RELEASE_NULL(s_sharedWorkerDeviceManger);
}

WorkerDeviceManger::WorkerDeviceManger(void)
{
  
}


WorkerDeviceManger::~WorkerDeviceManger(void)
{
   
}


bool WorkerDeviceManger::init()
{
    bool bRet = false;
    do
    {
        m_selectDevice = nullptr;
        
        bRet = true;
    }
    while (0);
    
    return bRet;
}
