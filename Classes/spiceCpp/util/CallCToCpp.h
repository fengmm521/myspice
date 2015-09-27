//
//  CallCToCpp.h
//  spiceCpp
//
//  Created by woodcol on 15/9/15.
//  Copyright (c) 2015年 woodcol. All rights reserved.
//

#ifndef __spiceCpp__CallCToCpp__
#define __spiceCpp__CallCToCpp__

#include <stdio.h>
#include <iostream>
#include "SpiceToolManger.h"
extern "C" {
    void splitstr(char* pstr);
    void pauseThread();
    void setPintLineCount(int count);
}
void splitstr(char* pstr);
void setPintLineCount(int count);
void pauseThread();

class DataPrint
{
public:
    DataPrint(void){};
    ~DataPrint(void){};
    std::vector<PrintDat> m_Dataprint;
    PrintDat m_nowPrintDat;
    void addStr(std::string str,int ptype);
    int m_lineCount;
};



#endif /* defined(__spiceCpp__CallCToCpp__) */
