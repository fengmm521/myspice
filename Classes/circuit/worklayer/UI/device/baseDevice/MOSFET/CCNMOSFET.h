//
//  CCNMOSFET.h
//  myspice
//  精灵类，主要用来创建一些定制的精灵动画。
//  Created by woodcol on 15/9/27.
//
//

#ifndef __myspice__CCNMOSFET__
#define __myspice__CCNMOSFET__
#include "cocos2d.h"
#include "DeviceShowBase.h"
//NMOS管
class CCNMOSFET:public DeviceShowBase
{
public:
     CREATE_FUNC(CCNMOSFET);
    virtual bool init();
};


#endif /* defined(__myspice__CCNMOSFET__) */
