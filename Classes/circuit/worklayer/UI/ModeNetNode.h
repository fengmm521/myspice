//
//  ModeNetNode.h
//  myspice
//  精灵类，主要用来创建一些定制的精灵动画。
//  Created by woodcol on 15/9/27.
//
//

#ifndef __myspice__ModeNetNode__
#define __myspice__ModeNetNode__

#include <iostream>

#include "cocos2d.h"

class ModeNetLine;

class ModeNetNode:public cocos2d::Sprite
{
public:
    //在一条连线上使用另一线连线的端点创建一个节点，将第一条连线分成两个连线，一起连接入节点
    static ModeNetNode* createWithLinePoint(ModeNetLine* netLine1,ModeNetLine* netLine2);
    virtual bool init();
    
    
};


#endif /* defined(__myspice__ModeNetNode__) */
