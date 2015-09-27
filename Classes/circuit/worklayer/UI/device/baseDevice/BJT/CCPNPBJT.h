//
//  CCPNPBJT.h
//  myspice
//  精灵类，主要用来创建一些定制的精灵动画。
//  Created by woodcol on 15/9/27.
//
//

#ifndef __myspice__CCPNPBJT__
#define __myspice__CCPNPBJT__
#include "cocos2d.h"
#include "DataConfig.h"
#include "DeviceShowBase.h"
//pnp三级管
class CCPNPBJT:public DeviceShowBase
{
public:
     CREATE_FUNC(CCPNPBJT);
    virtual bool init();
    ConfigBJTMode m_bjtModeP;
};


#endif /* defined(__myspice__CCPNPBJT__) */
