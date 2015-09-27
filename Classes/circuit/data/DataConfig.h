//
//  DataConfig.h
//  myspice
//  使用结构体来进行游戏属性存储会比较方便快捷
//
//  Created by woodcol on 15/9/26.
//
//

#ifndef myspice_DataConfig_h
#define myspice_DataConfig_h
#include "cocos2d.h"
using namespace std;
using namespace cocos2d;


#define kWidget 2048   //编辑背景宽
#define kHeight 1536   //编辑背景高
#define kNetSize 64    //器件引脚间距

//器件引脚
typedef struct ModePins_ST
{
    std::string pinName = "1";              //引脚名
    std::string pinModeName = "modename";   //引脚所在模型名称
    int modePinNumber = 0;                  //引脚所在模型引脚号
    int modePinCount = 1;                   //模型的最大引脚数量
    int PinTarget = 1;                      //引脚在模型的方向,1:左边，2：下边，3：右边，4：上边
    Point pinPoint = Point(-1,-1);          //引脚相对模型原点的坐标位置
}ModePinsData;


//线断
typedef struct LineDoublePoints_ST
{
    Point point0;
    Point point1;
}LineDoublePoints;

//连线的端点
typedef struct LinePort_ST
{
    Point portPo; //端点坐标
    //连线端点连接节点类型
    int portType = 0; //0:悬空，1:连接器件或电路引脚，2:连接连线节点
    std::string netName = ""; //连线所接网表名
    ModePinsData pinData; //端点所接器件引脚,无引脚时
    
}LinePort;


//器件
//BJT
//NPN 2n3904
typedef struct BJTDATA_ST
{
    std::string BJTModeName = "2n3904";
    std::string BJTType = "npn";
    std::string BJTModeParameter = "Is=6.734f Xti=3 Eg=1.11 Vaf=74.03 Bf=416.4 Ne=1.259 Ise=6.734f Ikf=66.78m Xtb=1.5 Br=.7371 Nc=2 Isc=0 Ikr=0 Rc=1 Cjc=3.638p Mjc=.3085 Vjc=.75 Fc=.5 Cje=4.493p Mje=.2593 Vje=.75 Tr=239.5n Tf=301.2p Itf=.4 Vtf=4 Xtf=2 Rb=10";
}ConfigBJTMode;


#endif
