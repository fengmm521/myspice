//
//  CCNJEFT.h
//  myspice
//  精灵类，主要用来创建一些定制的精灵动画。
//  Created by woodcol on 15/9/27.
//
//

#ifndef __myspice__CCNJEFT__
#define __myspice__CCNJEFT__
#include "cocos2d.h"
#include "DeviceShowBase.h"
//N通道场效应管

class CCNJEFT:public DeviceShowBase
{
public:
     CREATE_FUNC(CCNJEFT);
    virtual bool init();
};


#endif /* defined(__myspice__CCNJEFT__) */
