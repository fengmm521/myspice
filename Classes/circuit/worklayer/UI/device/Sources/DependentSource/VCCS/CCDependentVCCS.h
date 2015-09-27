//
//  CCDependentVCCS.h
//  myspice
//  精灵类，主要用来创建一些定制的精灵动画。
//  Created by woodcol on 15/9/27.
//
//

#ifndef __myspice__CCDependentVCCS__
#define __myspice__CCDependentVCCS__
#include "cocos2d.h"
#include "DeviceShowBase.h"
//压控电流源


class CCDependentVCCS:public DeviceShowBase
{
public:
     CREATE_FUNC(CCDependentVCCS);
    virtual bool init();
};


#endif /* defined(__myspice__CCDependentVCCS__) */
