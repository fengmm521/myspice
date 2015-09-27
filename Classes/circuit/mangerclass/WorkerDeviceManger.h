//
//  WorkerDeviceManger.h
//  myspice
//  C++单例类模板，非线程安全单例类，并且实例化后会常注内存
//
//  Created by woodcol on 15/9/26.
//
//

#ifndef __myspice__WorkerDeviceManger__
#define __myspice__WorkerDeviceManger__

#include <iostream>
#include "cocos2d.h"

class DeviceShowBase;
class ModeNetLine;
class ModeNetNode;
class WorkerMainLayer;

class WorkerDeviceManger:public cocos2d::Ref
{
public:
    static WorkerDeviceManger* sharedWorkerDeviceManger();
    static void destroyInstance();
private:
	/**
     * @js ctor
     */
    WorkerDeviceManger(void);
    /**
     * @js NA
     * @lua NA
     */
    ~WorkerDeviceManger(void);
    
public:
    
    /**
     * Init WorkerDeviceManger
     */
    virtual bool init();
    
    
    WorkerMainLayer* m_workerlayer;
    
    std::vector<DeviceShowBase*> m_nowDevices; //当前的所有器件
    std::vector<ModeNetLine*> m_ModeNetLines;  //当前的所有连接线
    std::vector<ModeNetNode*> m_ModeNetNode;   //所有连线上的节点
    
    DeviceShowBase* createOneDeviceWithName(std::string devname);
    
    DeviceShowBase* m_selectDevice; //当前选择的器件
    DeviceShowBase* getSelectDevice();
    //当器件被选中时，本方法由器件自已调用
    void selectDevice(DeviceShowBase* selDevice);
    //取消选择当前选择的器件
    void unselectDevice(DeviceShowBase* unselDevice);
    
    //连接线,增加一个新连接线
    void addNewNetLine(ModeNetLine* nline);
    //增加一个新连接节点
    void addNewNetNode(ModeNetNode* nnode);
    //增加一个新的器件,通过模型管理类获取新器件参数
    void addNewDeviceWithModeName(std::string modeName);
};

#endif /* defined(__myspice__WorkerDeviceManger__) */
