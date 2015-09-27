//
//  CCLED.h
//  myspice
//  精灵类，主要用来创建一些定制的精灵动画。
//  Created by woodcol on 15/9/27.
//
//

#ifndef __myspice__CCLED__
#define __myspice__CCLED__
#include "cocos2d.h"
#include "DeviceShowBase.h"
//发光二极管,默认为三色发光二极管

class CCLED:public DeviceShowBase
{
public:
     CREATE_FUNC(CCLED);
    virtual bool init();
};


#endif /* defined(__myspice__CCLED__) */
