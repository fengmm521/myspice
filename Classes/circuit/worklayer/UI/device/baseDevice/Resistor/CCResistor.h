//
//  CCResistor.h
//  myspice
//  精灵类，主要用来创建一些定制的精灵动画。
//  Created by woodcol on 15/9/27.
//
//

#ifndef __myspice__CCResistor__
#define __myspice__CCResistor__
#include "cocos2d.h"
#include "DeviceShowBase.h"
//电阻器

class CCResistor:public DeviceShowBase
{
public:
     CREATE_FUNC(CCResistor);
    virtual bool init();
};


#endif /* defined(__myspice__CCResistor__) */
