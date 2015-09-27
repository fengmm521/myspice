//
//  CCPMOSFET.h
//  myspice
//  精灵类，主要用来创建一些定制的精灵动画。
//  Created by woodcol on 15/9/27.
//
//

#ifndef __myspice__CCPMOSFET__
#define __myspice__CCPMOSFET__
#include "cocos2d.h"
#include "DeviceShowBase.h"
//PMOSFET

class CCPMOSFET:public DeviceShowBase
{
public:
     CREATE_FUNC(CCPMOSFET);
    virtual bool init();
};


#endif /* defined(__myspice__CCPMOSFET__) */
