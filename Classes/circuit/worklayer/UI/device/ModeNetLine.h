//
//  ModeNetLine.h
//  myspice
//  精灵类，器件的网络连线对象
//  Created by woodcol on 15/9/23.
//
//

#ifndef __myspice__ModeNetLine__
#define __myspice__ModeNetLine__

#include <iostream>

#include "cocos2d.h"
#include "DataConfig.h"
using namespace cocos2d;

class ModeNetLine:public cocos2d::Sprite
{
public:
    
    static ModeNetLine* createWithNetName(std::string pNetname,std::vector<Point> &pos);
    
    virtual bool init();
    std::vector<cocos2d::Point> m_linePoints; //连线所经过的点,两点间会组成线断，两点之间的距离只能是64
    
    std::vector<LineDoublePoints> m_linesDouble; //所有点组成的线断集合
    
    
    std::string m_lineNetName;  //连线网表名称
    LinePort m_portA;           //连线A端状态信息
    LinePort m_portB;           //连线B端状态信息
    
    
    void findLineNetNameFromManger(); //从电路管理类生成当前连线的网表名,所有没有互相连接的网表名不能重复。
    
    //选择连线
    void selectLinePoint(Point po);
    
    //托动以绘制连线
    void startDrawNewLine(Point toPo);
    
    //停止选择连线，或者绘制结束
    void endSelectLine();
    
    //连线被其他器件分割，或者被连线节点分割成两个连线,
    std::vector<ModeNetLine*> splitModeNetLineWithPoint(Point po);
    
    //将当前连线与另一条连线相连,两条连线必须要有一个相同的端点
    void connectOtherNetLine(ModeNetLine* otherLine);
    
};

#endif /* defined(__myspice__ModeNetLine__) */
