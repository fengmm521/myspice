//
//  SUBCKTManger.h
//  myspice
//  C++单例类模板，非线程安全单例类，并且实例化后会常注内存
//
//  Created by woodcol on 15/9/19.
//
//

#ifndef __myspice__SUBCKTManger__
#define __myspice__SUBCKTManger__

#include <iostream>
#include "cocos2d.h"

typedef struct SUBCKT_ST
{
    std::string subcktName;        //子电路名称
    std::string subcktFilePath;    //子电路文件保存路径
    std::string subcktData;        //子电路文件内容
    std::string subcktCircuitMode; //子电路电路模型文件名
}SUBCKT;


class SUBCKTManger:public cocos2d::Ref
{
public:
    static SUBCKTManger* sharedSUBCKTManger();
    static void destroyInstance();
private:
	/**
     * @js ctor
     */
    SUBCKTManger(void);
    /**
     * @js NA
     * @lua NA
     */
    ~SUBCKTManger(void);
    
public:
    
    /**
     * Init SUBCKTManger
     */
    virtual bool init();
};

#endif /* defined(__myspice__SUBCKTManger__) */
