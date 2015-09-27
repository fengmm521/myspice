//
//  CCDiode.h
//  myspice
//  精灵类，主要用来创建一些定制的精灵动画。
//  Created by woodcol on 15/9/27.
//
//

#ifndef __myspice__CCDiode__
#define __myspice__CCDiode__
#include "cocos2d.h"
#include "DeviceShowBase.h"
//二极管

class CCDiode:public DeviceShowBase
{
public:
     CREATE_FUNC(CCDiode);
    virtual bool init();
};


#endif /* defined(__myspice__CCDiode__) */
