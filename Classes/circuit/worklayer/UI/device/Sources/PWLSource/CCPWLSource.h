//
//  CCPWLSource.h
//  myspice
//  精灵类，主要用来创建一些定制的精灵动画。
//  Created by woodcol on 15/9/27.
//
//

#ifndef __myspice__CCPWLSource__
#define __myspice__CCPWLSource__
#include "cocos2d.h"
#include "DeviceShowBase.h"
//线性调制电压源,可以调制成三角波，等

class CCPWLSource:public cocos2d::Sprite
{
public:
     CREATE_FUNC(CCPWLSource);
    virtual bool init();
};


#endif /* defined(__myspice__CCPWLSource__) */
