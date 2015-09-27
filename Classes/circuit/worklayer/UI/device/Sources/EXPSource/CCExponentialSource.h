//
//  CCExponentialSource.h
//  myspice
//  精灵类，主要用来创建一些定制的精灵动画。
//  Created by woodcol on 15/9/27.
//
//

#ifndef __myspice__CCExponentialSource__
#define __myspice__CCExponentialSource__
#include "cocos2d.h"
#include "DeviceShowBase.h"
//指数源

class CCExponentialSource:public DeviceShowBase
{
public:
     CREATE_FUNC(CCExponentialSource);
    virtual bool init();
};


#endif /* defined(__myspice__CCExponentialSource__) */
