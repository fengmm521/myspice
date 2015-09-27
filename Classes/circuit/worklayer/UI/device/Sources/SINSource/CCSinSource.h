//
//  CCSinSource.h
//  myspice
//  精灵类，主要用来创建一些定制的精灵动画。
//  Created by woodcol on 15/9/27.
//
//

#ifndef __myspice__CCSinSource__
#define __myspice__CCSinSource__
#include "cocos2d.h"
#include "DeviceShowBase.h"
//正弦电压源

class CCSinSource:public DeviceShowBase
{
public:
     CREATE_FUNC(CCSinSource);
    virtual bool init();
};


#endif /* defined(__myspice__CCSinSource__) */
