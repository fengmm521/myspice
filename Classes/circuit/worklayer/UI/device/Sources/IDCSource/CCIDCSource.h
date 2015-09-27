//
//  CCIDCSource.h
//  myspice
//  精灵类，主要用来创建一些定制的精灵动画。
//  Created by woodcol on 15/9/27.
//
//

#ifndef __myspice__CCIDCSource__
#define __myspice__CCIDCSource__
#include "cocos2d.h"
#include "DeviceShowBase.h"
//直流电流源

class CCIDCSource:public DeviceShowBase
{
public:
     CREATE_FUNC(CCIDCSource);
    virtual bool init();
};


#endif /* defined(__myspice__CCIDCSource__) */
