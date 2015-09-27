//
//  CCZenerDiode.h
//  myspice
//  精灵类，主要用来创建一些定制的精灵动画。
//  Created by woodcol on 15/9/27.
//
//

#ifndef __myspice__CCZenerDiode__
#define __myspice__CCZenerDiode__
#include "cocos2d.h"
#include "CCDiode.h"

//稳压二级管

class CCZenerDiode:public CCDiode
{
public:
     CREATE_FUNC(CCZenerDiode);
    virtual bool init();
};


#endif /* defined(__myspice__CCZenerDiode__) */
