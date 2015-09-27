//
//  CCInductor.h
//  myspice
//  精灵类，主要用来创建一些定制的精灵动画。
//  Created by woodcol on 15/9/27.
//
//

#ifndef __myspice__CCInductor__
#define __myspice__CCInductor__
#include "cocos2d.h"
#include "DeviceShowBase.h"
//电感


class CCInductor:public DeviceShowBase
{
public:
     CREATE_FUNC(CCInductor);
    virtual bool init();
};


#endif /* defined(__myspice__CCInductor__) */
