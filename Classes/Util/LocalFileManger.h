//
//  LocalFileManger.h
//  game1
//  C++单例类模板，非线程安全单例类，并且实例化后会常注内存
//
//  Created by 俊盟科技1 on 9/4/14.
//
//

#ifndef __game1__LocalFileManger__
#define __game1__LocalFileManger__

#include "cocos2d.h"
class LocalFileManger:public cocos2d::Ref
{
public:
    static LocalFileManger* sharedLocalFileManger();
    static void destroyInstance();
private:
	/**
     * @js ctor
     */
    LocalFileManger(void);
    /**
     * @js NA
     * @lua NA
     */
    ~LocalFileManger(void);
    
public:
    
    /**
     * Init LocalFileManger
     */
    virtual bool init();
    std::string getFileByName(std::string pFileName);
    std::string getFileNameFromFilePath(std::string pFilePathName);
    bool mkDirWithFilePath(std::string pFilePahtName);
    bool saveDataToFile(char *pContent,std::string pFileName);
    bool saveDataToFile(std::string pContent,std::string pFileName);
};

#endif /* defined(__game1__LocalFileManger__) */
