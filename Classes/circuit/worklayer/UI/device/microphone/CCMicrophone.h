//
//  CCMicrophone.h
//  myspice
//  精灵类，主要用来创建一些定制的精灵动画。
//  Created by woodcol on 15/9/27.
//
//

#ifndef __myspice__CCMicrophone__
#define __myspice__CCMicrophone__
#include "cocos2d.h"
#include "DeviceShowBase.h"
//麦克风

class CCMicrophone:public DeviceShowBase
{
public:
     CREATE_FUNC(CCMicrophone);
    virtual bool init();
};


#endif /* defined(__myspice__CCMicrophone__) */
