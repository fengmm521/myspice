//
//  UIBGLine.h
//  myspice
//
//  Created by woodcol on 15/9/26.
//
//
//资源加载适合cocos2d-x 3.2版本的cocostudio for mac 1.0.0
//

#ifndef __myspice__UIBGLine__
#define __myspice__UIBGLine__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "editor-support/cocostudio/CocoStudio.h"
USING_NS_CC;
using namespace ui;
using namespace cocostudio;
class UIBGLine : public cocos2d::Layer
{
public:
    virtual bool init();
    // a selector callback
    static UIBGLine* createBGLine();
    
    int m_widget;
    int m_height;
    
    int m_netSize;
    
    int m_wlineCount;
    int m_vlineCount;
    
    ui::ImageView* m_wLineBase; //横向线条
    ui::ImageView* m_vLineBase; //纵向线条
    
    //button event
    void initBGLine();
    void setAllLine();
    
    void moveTo(Point po);
};


#endif /* defined(__myspice__UIBGLine__) */
