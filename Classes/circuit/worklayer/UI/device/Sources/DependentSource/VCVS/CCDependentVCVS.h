//
//  CCDependentVCVS.h
//  myspice
//  精灵类，主要用来创建一些定制的精灵动画。
//  Created by woodcol on 15/9/27.
//
//

#ifndef __myspice__CCDependentVCVS__
#define __myspice__CCDependentVCVS__
#include "cocos2d.h"
#include "DeviceShowBase.h"
//压控电压源


class CCDependentVCVS:public DeviceShowBase
{
public:
     CREATE_FUNC(CCDependentVCVS);
    virtual bool init();
};


#endif /* defined(__myspice__CCDependentVCVS__) */
