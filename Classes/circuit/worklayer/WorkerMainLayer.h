//
//  WorkerMainLayer.h
//  myspice
//
//  Created by woodcol on 15/9/19.
//
//
//资源加载适合cocos2d-x 3.2版本的cocostudio for mac 1.0.0
//

#ifndef __myspice__WorkerMainLayer__
#define __myspice__WorkerMainLayer__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "editor-support/cocostudio/CocoStudio.h"
USING_NS_CC;
using namespace ui;
using namespace cocostudio;
class WorkerMainLayer : public cocos2d::Layer
{
public:
    virtual bool init();
    // a selector callback
    
    static WorkerMainLayer* createWithCircuitName(std::string pCircuitName);
    
    
    void initBGLine();
    
    
    void menuTouchCallback(cocos2d::Ref* pSender);
    // implement the "static create()" method manually
    CREATE_FUNC(WorkerMainLayer);
    //button event
    void touchEvent(cocos2d::Ref *pSender, Widget::TouchEventType type);
    
    
    void setDeviceTo(Point po);
    
};

#endif /* defined(__myspice__WorkerMainLayer__) */
