//
//  CCCapacitor.h
//  myspice
//  精灵类，主要用来创建一些定制的精灵动画。
//  Created by woodcol on 15/9/27.
//
//

#ifndef __myspice__CCCapacitor__
#define __myspice__CCCapacitor__
#include "cocos2d.h"
#include "DeviceShowBase.h"

//电容器

class CCCapacitor:public DeviceShowBase
{
public:
     CREATE_FUNC(CCCapacitor);
    virtual bool init();
};


#endif /* defined(__myspice__CCCapacitor__) */
