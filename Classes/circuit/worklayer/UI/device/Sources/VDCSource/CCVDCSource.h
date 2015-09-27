//
//  CCVDCSource.h
//  myspice
//  精灵类，主要用来创建一些定制的精灵动画。
//  Created by woodcol on 15/9/27.
//
//

#ifndef __myspice__CCVDCSource__
#define __myspice__CCVDCSource__
#include "cocos2d.h"
#include "DeviceShowBase.h"
//直流电压源

class CCVDCSource:public DeviceShowBase
{
public:
     CREATE_FUNC(CCVDCSource);
    virtual bool init();
};


#endif /* defined(__myspice__CCVDCSource__) */
