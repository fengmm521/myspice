//
//  SUBCKTManger.cpp
//  myspice
//
//  Created by woodcol on 15/9/19.
//
//  子电路模型管理器
//
//

#include "SUBCKTManger.h"

static SUBCKTManger *s_sharedSUBCKTManger = nullptr;

SUBCKTManger *SUBCKTManger::sharedSUBCKTManger()
{
    if (s_sharedSUBCKTManger == nullptr)
    {
        s_sharedSUBCKTManger = new SUBCKTManger();
        if (!s_sharedSUBCKTManger || !s_sharedSUBCKTManger->init())
        {
            CC_SAFE_DELETE(s_sharedSUBCKTManger);
        }
    }
    return s_sharedSUBCKTManger;
}

void SUBCKTManger::destroyInstance()
{
    
    CC_SAFE_RELEASE_NULL(s_sharedSUBCKTManger);
}

SUBCKTManger::SUBCKTManger(void)
{
  
}


SUBCKTManger::~SUBCKTManger(void)
{
   
}


bool SUBCKTManger::init()
{
    bool bRet = false;
    do
    {
        
        
        bRet = true;
    }
    while (0);
    
    return bRet;
}
