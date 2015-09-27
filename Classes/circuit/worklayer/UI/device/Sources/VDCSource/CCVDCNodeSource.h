//
//  CCVDCNodeSource.h
//  myspice
//  精灵类，主要用来创建一些定制的精灵动画。
//  Created by woodcol on 15/9/27.
//
//

#ifndef __myspice__CCVDCNodeSource__
#define __myspice__CCVDCNodeSource__
#include "cocos2d.h"
#include "CCVDCSource.h"
//直流节点电位电压源

class CCVDCNodeSource:public CCVDCSource
{
public:
     CREATE_FUNC(CCVDCNodeSource);
    virtual bool init();
};


#endif /* defined(__myspice__CCVDCNodeSource__) */
