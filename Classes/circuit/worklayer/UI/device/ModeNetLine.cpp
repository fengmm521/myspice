//
//  ModeNetLine.cpp
//  myspice
//
//  Created by woodcol on 15/9/23.
//
//

#include "ModeNetLine.h"

bool ModeNetLine::init()
{
    //////////////////////////////
    // 1. super init first
    if (!cocos2d::Sprite::init()) {
        return false;
    }
    
    
    bool bRet = false;
    do
    {
        
        m_lineNetName = "";
        
        
        
        
        bRet = true;
    }
    while (0);
    
    return bRet;
}