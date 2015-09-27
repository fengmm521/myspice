//
//  CCDependentCCVS.h
//  myspice
//  精灵类，主要用来创建一些定制的精灵动画。
//  Created by woodcol on 15/9/27.
//
//

#ifndef __myspice__CCDependentCCVS__
#define __myspice__CCDependentCCVS__
#include "cocos2d.h"
#include "DeviceShowBase.h"
//流控电压源

class CCDependentCCVS:public DeviceShowBase
{
public:
     CREATE_FUNC(CCDependentCCVS);
    virtual bool init();
};


#endif /* defined(__myspice__CCDependentCCVS__) */
