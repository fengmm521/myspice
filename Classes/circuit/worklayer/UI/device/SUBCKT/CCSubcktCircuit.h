//
//  CCSubcktCircuit.h
//  myspice
//  精灵类，主要用来创建一些定制的精灵动画。
//  Created by woodcol on 15/9/27.
//
//

#ifndef __myspice__CCSubcktCircuit__
#define __myspice__CCSubcktCircuit__
#include "cocos2d.h"
#include "DeviceShowBase.h"
//子电源,或者是用户自定义的器件

class CCSubcktCircuit:public DeviceShowBase
{
public:
     CREATE_FUNC(CCSubcktCircuit);
    virtual bool init();
};


#endif /* defined(__myspice__CCSubcktCircuit__) */
