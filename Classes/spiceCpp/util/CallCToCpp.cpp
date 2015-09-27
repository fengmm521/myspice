//
//  CallCToCpp.cpp
//  spiceCpp
//
//  Created by woodcol on 15/9/15.
//  Copyright (c) 2015年 woodcol. All rights reserved.
//

#include "CallCToCpp.h"
#include "StrSplit.h"
#include "Lock.h"
#include "LockUtil.h"
#include <string>

static DataPrint sticDataPrint;
static bool isBegin = false;
static bool isEnd = false;
static int typex = 0;
static int nlinecount = -1;
void splitstr(char* pstr)
{
    std::vector<std::string> strarr = StrSplit::split(pstr, "\t");
    //printf("xxxxx:%s",strarr[0].c_str());
    if (std::strcmp(strarr[0].c_str(),"0") == 0) {
        isBegin = true;
        isEnd = false;
        typex = 1;
        nlinecount = 0;
        sticDataPrint.addStr(strarr[0],1);
    }else if(std::strcmp(strarr[0].c_str(),"\n") != 0){
        if (isBegin) {
            switch (typex) {
                case 1://time
                {
                    typex = 2;
                   // printf("type1:%s\n",strarr[0].c_str());
                    sticDataPrint.addStr(strarr[0],2);
                    
                }
                    break;
                case 2://value
                {
                    typex = 3;
                    //printf("type2:%s\n",strarr[0].c_str());
                    sticDataPrint.addStr(strarr[0],3);
                    if (isEnd) {
                        typex = -1;
                        isBegin = false;
                        nlinecount = -1;
                        sticDataPrint.m_Dataprint.clear();
                    }
                }
                    break;
                case 3:
                {
                    typex = 1;
                    nlinecount++;
//                    if (nlinecount == 61) {
//                        printf("lineend:%s",strarr[0].c_str());
//                    }
//                    printf("type3:%s\n",strarr[0].c_str());
                    sticDataPrint.addStr(strarr[0],1);
                    
                }
                    break;
                default:
                    break;
            }
        }
    }
}
void setPintLineCount(int count)
{
    sticDataPrint.m_lineCount = count;
}

void pauseThread()
{
    m_sendEvent->Unlock();
    SpiceToolManger::sharedSpiceToolManger()->m_isSpiceOk = true;
    m_sendEvent->Wait();
}
void DataPrint::addStr(std::string str,int ptype)
{
    switch (ptype) {
        case 1:
        {
            m_nowPrintDat.lineID = atoi(str.c_str());
        }
            break;
        case 2:
        {
            m_nowPrintDat.ptime = atof(str.c_str());
        }
            break;
        case 3:
        {
            m_nowPrintDat.pvalue = atof(str.c_str());
            m_Dataprint.push_back(m_nowPrintDat);
            if (this->m_lineCount -1 == nlinecount) {
                SpiceToolManger::sharedSpiceToolManger()->setPrintData(m_Dataprint);
                isEnd = true;
            }
        }
            break;
            
        default:
            break;
    }
}