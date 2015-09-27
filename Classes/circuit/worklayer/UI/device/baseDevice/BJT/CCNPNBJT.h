//
//  CCNPNBJT.h
//  myspice
//  精灵类，主要用来创建一些定制的精灵动画。
//  Created by woodcol on 15/9/27.
//
//

#ifndef __myspice__CCNPNBJT__
#define __myspice__CCNPNBJT__
#include "cocos2d.h"
#include "DataConfig.h"
#include "DeviceShowBase.h"
//npn三极管

class CCNPNBJT:public DeviceShowBase
{
public:
     CREATE_FUNC(CCNPNBJT);
    virtual bool init();
    ConfigBJTMode m_bjtModeP;
};


#endif /* defined(__myspice__CCNPNBJT__) */
