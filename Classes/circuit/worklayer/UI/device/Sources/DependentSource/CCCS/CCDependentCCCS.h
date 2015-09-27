//
//  CCDependentCCCS.h
//  myspice
//  精灵类，主要用来创建一些定制的精灵动画。
//  Created by woodcol on 15/9/27.
//
//

#ifndef __myspice__CCDependentCCCS__
#define __myspice__CCDependentCCCS__
#include "cocos2d.h"
#include "DeviceShowBase.h"

class CCDependentCCCS:public DeviceShowBase
{
public:
     CREATE_FUNC(CCDependentCCCS);
    virtual bool init();
};


#endif /* defined(__myspice__CCDependentCCCS__) */
