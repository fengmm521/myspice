//
//  CCIACSource.h
//  myspice
//  精灵类，主要用来创建一些定制的精灵动画。
//  Created by woodcol on 15/9/27.
//
//

#ifndef __myspice__CCIACSource__
#define __myspice__CCIACSource__
#include "cocos2d.h"
#include "DeviceShowBase.h"
//交流电流源


class CCIACSource:public DeviceShowBase
{
public:
     CREATE_FUNC(CCIACSource);
    virtual bool init();
};


#endif /* defined(__myspice__CCIACSource__) */
