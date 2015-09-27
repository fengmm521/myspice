//
//  DeviceShowBase.h
//  myspice
//  精灵类，主要用来创建一些定制的精灵动画。
//  Created by woodcol on 15/9/19.
//
//

#ifndef __myspice__DeviceShowBase__
#define __myspice__DeviceShowBase__

#include <iostream>

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "DataConfig.h"
USING_NS_CC;
using namespace ui;
using namespace cocostudio;



class WorkerMainLayer;

class DeviceShowBase:public cocos2d::Sprite
{
public:
    virtual bool init();
    
    virtual void initBase();
    // 初始化模型显示数据
    static DeviceShowBase* createWithNameAndPoint(WorkerMainLayer* worklayer,std::string modeName,Point pini = Point(0,0));
    //button event
    std::string m_modeImageName;//模型图形名称
    std::string m_modeName;     //模型名称
    std::string m_modeTypeName; //器件类型名称
    std::vector<ModePinsData> m_pins; //器件所有引脚
    
    virtual void setModeDateWithModeName();
    
    //器件所在电路图图层
    WorkerMainLayer* m_worklayer;
    //设置模型到一个新坐标
    void moveModeTo(Point* po);
    //获取模型所有坐标在电路设置界面中的坐标数据，当界面有缩放时，器件的实际坐标要根据缩放换算
    //计算引脚位置时通过当前器件旋转角色确定旋转方向
    std::vector<ModePinsData> getPinsDataForMainLayer();
    
    //设置器件被选择状态
    bool m_isSelect;
    void setSelect(bool isSelected = true);
    Sprite* m_selectBG;//器件被选择时的背影图像,通过器件的初始化引脚坐标
    
    //为引脚设置连接网表名
    virtual void setPinConnectNetLabe(int pintNum,std::string PinNetName);
};


#endif /* defined(__myspice__DeviceShowBase__) */
