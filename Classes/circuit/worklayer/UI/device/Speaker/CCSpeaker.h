//
//  CCSpeaker.h
//  myspice
//  精灵类，主要用来创建一些定制的精灵动画。
//  Created by woodcol on 15/9/27.
//
//

#ifndef __myspice__CCSpeaker__
#define __myspice__CCSpeaker__
#include "cocos2d.h"
#include "DeviceShowBase.h"
//音频输出喇叭

class CCSpeaker:public DeviceShowBase
{
public:
     CREATE_FUNC(CCSpeaker);
    virtual bool init();
};


#endif /* defined(__myspice__CCSpeaker__) */
