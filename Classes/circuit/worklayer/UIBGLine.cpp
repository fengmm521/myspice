//
//  UIBGLine.cpp
//  myspice
//
//  Created by woodcol on 15/9/26.
//
//资源加载适合cocos2d-x 3.2版本的cocostudio for mac 1.0.0
//

#include "UIBGLine.h"
#include "strHash.h"
#include "DataConfig.h"
USING_NS_CC;
using namespace ui;
using namespace std;
// on "init" you need to initialize your instance

UIBGLine* UIBGLine::createBGLine()
{
    UIBGLine* tmp = new UIBGLine();
    if(tmp){
        tmp->autorelease();
        tmp->init();
        return tmp;
    }else{
        return nullptr;
    }
}

bool UIBGLine::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    m_netSize = kNetSize;
    m_widget = kWidget+4*m_netSize;
    m_height = kHeight+4*m_netSize;
    
    this->initBGLine();
    
    
    return true;
}
void UIBGLine::initBGLine()
{
    m_wLineBase = ui::ImageView::create("bgline.png",Widget::TextureResType::PLIST);
    m_wLineBase->setScale9Enabled(true);
    m_wLineBase->setCapInsets(Rect(0,0,1,1));
    m_wLineBase->setContentSize(Size(m_widget,2));
    m_wLineBase->setPosition(Point(-m_netSize,-m_netSize));
    m_wLineBase->setAnchorPoint(Point(0.0f,0.5));
    m_wLineBase->setOpacity(100);
    this->addChild(m_wLineBase);
    
    m_vLineBase = ui::ImageView::create("bgline.png",Widget::TextureResType::PLIST);
    m_vLineBase->setScale9Enabled(true);
    m_vLineBase->setCapInsets(Rect(0,0,1,1));
    m_vLineBase->setContentSize(Size(2,m_height));
    m_vLineBase->setPosition(Point(-m_netSize,-m_netSize));
    m_vLineBase->setOpacity(100);
    m_vLineBase->setAnchorPoint(Point(0.5,0));
    this->addChild(m_vLineBase);
    
    this->setAllLine();
    
}
void UIBGLine::setAllLine()
{
    m_wlineCount = m_widget/m_netSize;
    m_vlineCount = m_height/m_netSize;
    
    for (int i = 0; i < m_wlineCount; i++) {
        ImageView* tmpwline = dynamic_cast<ImageView*>(m_vLineBase->clone());
        tmpwline->setPosition(Point(i*m_netSize,-m_netSize));
        this->addChild(tmpwline);
    }
    
    for (int i = 0; i < m_vlineCount; i++) {
        ImageView* tmpvline = dynamic_cast<ImageView*>(m_wLineBase->clone());
        tmpvline->setPosition(Point(-m_netSize,i*m_netSize));
        this->addChild(tmpvline);
    }
}
void UIBGLine::moveTo(Point po)
{
    int offsetx = (int(po.x))%64;
    int offsety = (int(po.y))%64;
    this->setPosition(offsetx,offsety);
}

