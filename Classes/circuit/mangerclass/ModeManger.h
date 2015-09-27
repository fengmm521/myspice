//
//  ModeManger.h
//  myspice
//  C++单例类模板，非线程安全单例类，并且实例化后会常注内存
//
//  Created by woodcol on 15/9/19.
//基础器件包括以下器件:
//
//电阻，电感，电容，二极管，三极管，双极晶体管，结型场效应管，mos场效应晶体管,
//直流电压电流源，交流电压电流源，脉冲电压源，分段线性电源（锯齿波，三角波，方波等），正弦波源，指数波源
//压控电压源，压控电流源，流控电压源，流控电流源
//麦克风，喇叭，蜂鸣器
//子电路
//

#ifndef __myspice__ModeManger__
#define __myspice__ModeManger__

#include <iostream>
#include "cocos2d.h"
class DeviceShowBase;
class ModeManger:public cocos2d::Ref
{
public:
    static ModeManger* sharedModeManger();
    static void destroyInstance();
private:
	/**
     * @js ctor
     */
    ModeManger(void);
    /**
     * @js NA
     * @lua NA
     */
    ~ModeManger(void);
    
public:
    
    /**
     * Init ModeManger
     */
    virtual bool init();
    
    //所有器件显示模型
    std::map<std::string,DeviceShowBase*> m_showModes;
    std::map<std::string,std::string> m_spiceDataModes; //器件的spice模型设置
};

#endif /* defined(__myspice__ModeManger__) */
