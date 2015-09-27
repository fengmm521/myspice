//
//  SpiceToolManger.h
//  myspice
//  C++单例类模板，非线程安全单例类，并且实例化后会常注内存
//
//  Created by woodcol on 15/9/15.
//
//

#ifndef __myspice__SpiceToolManger__
#define __myspice__SpiceToolManger__

#include <iostream>
#include "cocos2d.h"
#include <thread>
#include <mutex>

typedef enum comd_enum
{
    cmd_op = 1, //计算电路的直流偏置状态
    cmd_dc,     //当电路中某一参数在一定范围内变化时计算相对应的电路直流偏置特性
    cmd_ac,     //计算电路的交流小信号频率响应特性
    cmd_tran,   //瞬态分析,在给定激励信号的作用下,计算电路输出端的瞬态响应
    cmd_noise,  //噪声分析
    cmd_pz,     //零极点分析
    cmd_disto,  //失真分析
    cmd_tf      //直流小信号传递函数分析
}cmd_Type;

//瞬态分析参数
typedef struct TranValue_ST
{
    std::string minValue = "0.1m"; //瞬态分析数值分辨率
    std::string maxValue = "50m";  //瞬态分析时间长度
    std::string trantype = "";     //瞬态分析类型参数
}TranValueST;

//直流状态分分参数
typedef struct DCValue_ST
{
    std::string vNode = "";         //电压源或者电流源名称
    std::string minValue = "0";     //起始值
    std::string maxValue = "5";     //结束值
    std::string stepValue = "0.1";  //改变步长
}DCValueST;

//交流分析参数数据结构
typedef struct ACValue_ST
{
    //交流信号按数量级变化，
    //numsteps:步进数量方式,
    //dec:起始与结束的数量级之差，每一级中间有多少个分段，
    //oct：倍频
    //lin:线性
    std::string vNode = "dec";
    std::string decNumber = "20";   //表示在一个数量级内取多少个点
    std::string minValue = "1";     //频率起始值,start
    std::string maxValue = "10k";   //频率结束值,stop
}ACValueST;

//输出的列表数据
typedef struct PrintDat_ST
{
    int lineID;
    double ptime;
    double pvalue;
}PrintDat;

class SpiceToolManger:public cocos2d::Ref
{
public:
    static SpiceToolManger* sharedSpiceToolManger();
    static void destroyInstance();
private:
	/**
     * @js ctor
     */
    SpiceToolManger(void);
    /**
     * @js NA
     * @lua NA
     */
    ~SpiceToolManger(void);
    
public:
    
    /**
     * Init SpiceToolManger
     */
    virtual bool init();
    
    //start spice
    void startSpice();
    //重新设计电路图，当电路中元件值改变，或添加删除新元件时可调用当前方法
    void resetSpiceCircuit(std::string &circuitpath,std::function<void()> callback);
    //调用print命令获取电路仿真后参数列表。
    bool callSpicePrint(std::string val,std::function<void(std::vector<PrintDat>&)> callback);
    
    bool m_isSpiceOk;
    std::vector<PrintDat> getPrintData();
    void setPrintLineCount(int count);
    void setPrintData(std::vector<PrintDat> &datap);
    
private:
    //spice仿真器运行线程
    int s_printLineCount;
    int s_nowReadLine;
    int s_readType;//0:读取结束1:lineNumber,2:time,3:value
    
    std::function<void(std::vector<PrintDat>&)> s_printCallBack;
    
    PrintDat s_tmpPrintData;
    std::vector<PrintDat> s_printDataVector;
    std::thread threadSpice;
    void spiceThread();
    void updateSpice(float dt);
    
    std::string getBackStr();
    void clearPrintData();
    
    FILE *inPutStream;
    FILE *outPutStream;
    FILE *errPutStream;
};

#endif /* defined(__myspice__SpiceToolManger__) */
