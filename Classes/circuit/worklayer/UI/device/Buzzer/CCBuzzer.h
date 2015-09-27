//
//  CCBuzzer.h
//  myspice
//  精灵类，主要用来创建一些定制的精灵动画。
//  Created by woodcol on 15/9/27.
//
//

#ifndef __myspice__CCBuzzer__
#define __myspice__CCBuzzer__
#include "cocos2d.h"
#include "DeviceShowBase.h"
//蜂鸣器

class CCBuzzer:public DeviceShowBase
{
public:
     CREATE_FUNC(CCBuzzer);
    virtual bool init();
};


#endif /* defined(__myspice__CCBuzzer__) */
