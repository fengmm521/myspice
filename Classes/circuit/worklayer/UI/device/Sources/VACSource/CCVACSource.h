//
//  CCVACSource.h
//  myspice
//  精灵类，主要用来创建一些定制的精灵动画。
//  Created by woodcol on 15/9/27.
//
//

#ifndef __myspice__CCVACSource__
#define __myspice__CCVACSource__
#include "cocos2d.h"
#include "DeviceShowBase.h"
//交流电压源,也就是正弦波电压源


class CCVACSource:public DeviceShowBase
{
public:
     CREATE_FUNC(CCVACSource);
    virtual bool init();
};


#endif /* defined(__myspice__CCVACSource__) */
