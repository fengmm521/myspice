//
//  SpiceToolManger.cpp
//  myspice
//
//  Created by woodcol on 15/9/15.
//
//

#include "SpiceToolManger.h"
#include <iostream>
#include <istream>
#include <stdio.h>
#include <errno.h>
#include "maintool.h"
#include "LocalFileManger.h"
#include <unistd.h>
#include "Lock.h"
#include "maintool.h"
static SpiceToolManger *s_sharedSpiceToolManger = nullptr;
extern CEvent *m_sendEvent;

SpiceToolManger *SpiceToolManger::sharedSpiceToolManger()
{
    if (s_sharedSpiceToolManger == nullptr)
    {
        s_sharedSpiceToolManger = new SpiceToolManger();
        if (!s_sharedSpiceToolManger || !s_sharedSpiceToolManger->init())
        {
            CC_SAFE_DELETE(s_sharedSpiceToolManger);
        }
    }
    return s_sharedSpiceToolManger;
}

void SpiceToolManger::destroyInstance()
{
    
    CC_SAFE_RELEASE_NULL(s_sharedSpiceToolManger);
}

SpiceToolManger::SpiceToolManger(void)
{
  
}


SpiceToolManger::~SpiceToolManger(void)
{
   
}


bool SpiceToolManger::init()
{
    bool bRet = false;
    do
    {
        s_printCallBack = nullptr;
        std::string ext = "EX\nVS 1 0 10\nR1 1 2 20\nR2 2 0 20\nR3 3 0 4K\nL 2 3 0.2 IC=0.2\nC 3 0 0.5U IC=2\n.END\n";
        std::string paths = cocos2d::FileUtils::getInstance()->getWritablePath() + "ext.cir";
        cocos2d::log("%s",paths.c_str());
        LocalFileManger::sharedLocalFileManger()->saveDataToFile(ext, "ext.cir");
        std::string cmd = "source "+paths + "\n";
        cmd += "tran 0.1m 5m uic\n";
        cmd += "print v(3)\n";
//        std::string cmdpath = cocos2d::FileUtils::getInstance()->getWritablePath() + "cmd.txt";
//        LocalFileManger::sharedLocalFileManger()->saveDataToFile(cmd, cmdpath);
        inPutStream = tmpfile();
        fprintf(inPutStream, "%s",cmd.c_str());
        FILE *fp = inPutStream;
        char c;
        ssize_t i;
        int fse = fseek(fp, 0,0);
        //参考：http://baike.baidu.com/link?url=x0dj7iFyKCBdcg5UfrNK6_F2Q6IdzOtE_8nOxrs-7tDNSJ1WAZ6fcGUkQAMYy1mfVqSyXqZR19y7BJbQlVgyFq
        //fseek()设计文件当前读写指针
        //ftell() 获取文件读写当前指针
        printf("fseek:%d",fse);
        do
            i = read(fileno(fp), &c, 1);
        while (i == -1 && errno == EINTR);
        fse = fseek(fp, 0,0);
        printf("c=%c\n",c);
        outPutStream = tmpfile();
        errPutStream = tmpfile();
        
        this->clearPrintData();
        mainInFile(inPutStream);
        
        cocos2d::Director::getInstance()->getScheduler()->schedule(schedule_selector(SpiceToolManger::updateSpice), this, 0.0f, false);
        bRet = true;
    }
    while (0);
    
    return bRet;
}

//start spice
void SpiceToolManger::startSpice()
{
    m_isSpiceOk = false;
    threadSpice = std::thread(&SpiceToolManger::spiceThread,this);//创建一个分支线程，回调到sendThread函数里
   //启动仿真器线程
    m_sendEvent = new CEvent();
    threadSpice.detach();
    
}
void SpiceToolManger::spiceThread()
{
    const char *sstr = cocos2d::FileUtils::getInstance()->getWritablePath().c_str();
    //    const char **s;
    //    (*s)= const_cast<const char*>(*argv);
    
    const char *s[6];
    s[0] = sstr;
    s[1] = "-p";
    s[3] = (const char *)inPutStream;
    s[4] = (const char *)outPutStream;
    s[5] = (const char *)errPutStream;
    m_sendEvent->Lock();
    maintool(2, s);
    m_isSpiceOk = true;
    
}
void SpiceToolManger::updateSpice(float dt)
{
    if (m_isSpiceOk) {
        cocos2d::Director::getInstance()->getScheduler()->unschedule(schedule_selector(SpiceToolManger::updateSpice), this);
        //std::string backs = this->getBackStr();
        //printf("back:\n%s\n",backs.c_str());
        if (s_printCallBack) {
            s_printCallBack(s_printDataVector);
        }
    }
}
std::vector<PrintDat> SpiceToolManger::getPrintData()
{
    if (m_isSpiceOk) {
        return s_printDataVector;
    }else{
        std::vector<PrintDat> tmp;
        return tmp;
    }
    
}
void SpiceToolManger::setPrintLineCount(int count)
{
    s_printLineCount = count;
}
void SpiceToolManger::setPrintData(std::vector<PrintDat> &datap)
{
    s_printDataVector = datap;
}
void SpiceToolManger::clearPrintData()
{
    s_printLineCount = 0;
    s_readType = 0;
    s_nowReadLine = 0;
    s_printDataVector.clear();
    s_tmpPrintData.lineID = -1;
    s_tmpPrintData.ptime = -1;
    s_tmpPrintData.pvalue = 0;
}
std::string SpiceToolManger::getBackStr()
{
    FILE* file = outPutStream;
    if (file) {
        char* buf;
        long len;
        fseek(file, 0, SEEK_END);
        len = ftell(file);
        rewind(file);
        buf = (char*)malloc(sizeof(char) * len + 1);
        if (!buf) {
            return NULL;
        }
        int rLen = (int)fread(buf, sizeof(char), len, file);
        buf[rLen] = '\0';
        //CCLOG("has read content = %s",buf);
        
        std::string result = buf;
        free(buf);
        return result;
    }
    return "";
}
//重设电路
void SpiceToolManger::resetSpiceCircuit(std::string &circuitpath,std::function<void()> callback)
{
    
}
//call spice cmd
bool SpiceToolManger::callSpicePrint(std::string val,std::function<void(std::vector<PrintDat>&)> callback)
{
    if(!m_isSpiceOk)
    {
        return false;//当前有电路仿真命令正在运行
    }
    this->clearPrintData();
    s_printCallBack = callback;
    fclose(inPutStream);
    inPutStream = tmpfile();
    std::string sendcmd = "print "+ val + "\n";
    fprintf(inPutStream,"%s",sendcmd.c_str());
    //参考：http://baike.baidu.com/link?url=x0dj7iFyKCBdcg5UfrNK6_F2Q6IdzOtE_8nOxrs-7tDNSJ1WAZ6fcGUkQAMYy1mfVqSyXqZR19y7BJbQlVgyFq
    mainInFile(inPutStream);
    printf("print cmd value:%s\n",val.c_str());
    fseek(inPutStream, 0,SEEK_SET);
    m_isSpiceOk = false;
    cocos2d::Director::getInstance()->getScheduler()->schedule(schedule_selector(SpiceToolManger::updateSpice), this, 0.0f, false);
    m_sendEvent->Post();
    return true;
}