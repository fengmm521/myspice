//
//  CCPJEFT.h
//  myspice
//  精灵类，主要用来创建一些定制的精灵动画。
//  Created by woodcol on 15/9/27.
//
//

#ifndef __myspice__CCPJEFT__
#define __myspice__CCPJEFT__
#include "cocos2d.h"
#include "DeviceShowBase.h"
//P通道场效应管

class CCPJEFT:public DeviceShowBase
{
public:
     CREATE_FUNC(CCPJEFT);
    virtual bool init();
};


#endif /* defined(__myspice__CCPJEFT__) */
