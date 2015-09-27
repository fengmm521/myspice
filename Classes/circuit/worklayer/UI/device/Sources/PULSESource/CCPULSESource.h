//
//  CCPULSESource.h
//  myspice
//  精灵类，主要用来创建一些定制的精灵动画。
//  Created by woodcol on 15/9/27.
//
//

#ifndef __myspice__CCPULSESource__
#define __myspice__CCPULSESource__
#include "cocos2d.h"
#include "DeviceShowBase.h"
//脉冲源

class CCPULSESource:public DeviceShowBase
{
public:
     CREATE_FUNC(CCPULSESource);
    virtual bool init();
};


#endif /* defined(__myspice__CCPULSESource__) */
