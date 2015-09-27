//
//  ModeManger.cpp
//  myspice
//
//  Created by woodcol on 15/9/19.
//  基础器件模型文件管理器
//

#include "ModeManger.h"

static ModeManger *s_sharedModeManger = nullptr;

ModeManger *ModeManger::sharedModeManger()
{
    if (s_sharedModeManger == nullptr)
    {
        s_sharedModeManger = new ModeManger();
        if (!s_sharedModeManger || !s_sharedModeManger->init())
        {
            CC_SAFE_DELETE(s_sharedModeManger);
        }
    }
    return s_sharedModeManger;
}

void ModeManger::destroyInstance()
{
    
    CC_SAFE_RELEASE_NULL(s_sharedModeManger);
}

ModeManger::ModeManger(void)
{
  
}


ModeManger::~ModeManger(void)
{
   
}


bool ModeManger::init()
{
    bool bRet = false;
    do
    {
        
        
        bRet = true;
    }
    while (0);
    
    return bRet;
}
