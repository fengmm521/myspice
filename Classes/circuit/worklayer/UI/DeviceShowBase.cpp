//
//  DeviceShowBase.cpp
//  myspice
//
//  Created by woodcol on 15/9/19.
// 绘制电路图时的所有元器件基类
//

#include "DeviceShowBase.h"
#include "WorkerMainLayer.h"

static DeviceShowBase* createWithNameAndPoint(WorkerMainLayer* worklayer,std::string modeName,Point pini /*= Point(0,0)*/)
{
    DeviceShowBase* tmp = new DeviceShowBase();
    if(tmp){
        tmp->autorelease();
        tmp->m_worklayer = worklayer;
        tmp->m_modeName = modeName;
        tmp->initBase();
        return tmp;
    }else{
        return nullptr;
    }
}

void DeviceShowBase::initBase()
{
    //////////////////////////////
    // 1. super init first
    
}
bool DeviceShowBase::init()
{
    if (!cocos2d::Sprite::init()) {
        return false;
    }
    
    
    bool bRet = false;
    do
    {
        
        this->initBase();
        
        
        
        bRet = true;
    }
    while (0);
    
    return bRet;
}
void DeviceShowBase::setModeDateWithModeName()
{
    
}
void DeviceShowBase::setPinConnectNetLabe(int pintNum,std::string PinNetName)
{
    
}