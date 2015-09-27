//
//  CCELeCapacitor.h
//  myspice
//  精灵类，主要用来创建一些定制的精灵动画。
//  Created by woodcol on 15/9/27.
//
//

#ifndef __myspice__CCELeCapacitor__
#define __myspice__CCELeCapacitor__
#include "cocos2d.h"
#include "CCCapacitor.h"
//电解电容器

class CCELeCapacitor:public CCCapacitor
{
public:
     CREATE_FUNC(CCELeCapacitor);
    virtual bool init();
};


#endif /* defined(__myspice__CCELeCapacitor__) */
