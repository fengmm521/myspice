//
//  LocalFileManger.cpp
//  game1
//
//  Created by 俊盟科技1 on 9/4/14.
//
//
#include <sys/stat.h>
#include <errno.h>
#include "LocalFileManger.h"
using namespace cocos2d;
static LocalFileManger *s_sharedLocalFileManger = nullptr;

LocalFileManger *LocalFileManger::sharedLocalFileManger()
{
    if (s_sharedLocalFileManger == nullptr)
    {
        s_sharedLocalFileManger = new LocalFileManger();
        if (!s_sharedLocalFileManger || !s_sharedLocalFileManger->init())
        {
            CC_SAFE_DELETE(s_sharedLocalFileManger);
        }
    }
    return s_sharedLocalFileManger;
}

void LocalFileManger::destroyInstance()
{
    
    CC_SAFE_RELEASE_NULL(s_sharedLocalFileManger);
}

LocalFileManger::LocalFileManger(void)
{
  
}


LocalFileManger::~LocalFileManger(void)
{
   
}


bool LocalFileManger::init()
{
    bool bRet = false;
    do
    {
        
        
        bRet = true;
    }
    while (0);
    
    return bRet;
}

std::string LocalFileManger::getFileByName(std::string pFileName)
{
    
    //µ⁄“ªœ»ªÒ»°Œƒº˛µƒ¬∑æ∂
    std::string path = FileUtils::getInstance()->getWritablePath() + pFileName;
    CCLOG("path = %s",path.c_str());
    
    if (FileUtils::getInstance()->isFileExist(FileUtils::getInstance()->fullPathForFilename(path.c_str())))
    {
        //¥¥Ω®“ª∏ˆŒƒº˛÷∏’Î
        FILE* file = fopen(path.c_str(), "r");
        
        if (file) {
            char* buf;  //“™ªÒ»°µƒ◊÷∑˚¥Æ
            long len;    //ªÒ»°µƒ≥§∂»
            /*ªÒ»°≥§∂»*/
            fseek(file, 0, SEEK_END);   //“∆µΩŒ≤≤ø
            len = ftell(file);          //Ã·»°≥§∂»
            rewind(file);               //ªÿπÈ‘≠Œª
            CCLOG("count the file content len = %ld",len);
            //∑÷≈‰bufø’º‰
            buf = (char*)malloc(sizeof(char) * len + 1);
            if (!buf) {
                CCLOG("malloc space is not enough.");
                return NULL;
            }
            //∂¡»°Œƒº˛
            //∂¡»°Ω¯µƒbuf£¨µ•Œª¥Û–°£¨≥§∂»£¨Œƒº˛÷∏’Î
            int rLen = (int)fread(buf, sizeof(char), len, file);
            buf[rLen] = '\0';
            CCLOG("has read Length = %d",rLen);
            //CCLOG("has read content = %s",buf);
            
            std::string result = buf;
            fclose(file);
            free(buf);
            return result;
        }
        else
            CCLOG("open file error.");
        
        return "";
    }else{
        return "";
    }
    
    
    
}
std::string LocalFileManger::getFileNameFromFilePath(std::string pFilePathName)
{
	std::string heroIDAndLeve = pFilePathName; //’‚¿Ô”–Œƒº˛¬∑æ∂∫ÕŒƒº˛º–√˚
    
	const char *expr = heroIDAndLeve.c_str();
    
	char *buf = new char[strlen(expr)+1];
	strcpy(buf, expr);
    
	char* token = strtok( buf, "/\\");
	std::vector<std::string> strtmpID;
    
	while( token != NULL )
	{
		/* While there are tokens in "string" */
		//printf( "%s ", token );
		std::string tmpx = token;
		strtmpID.push_back(tmpx);
		/* Get next token: */
		token = strtok( NULL, "/\\");
	}
	
	log("back is %s",strtmpID.back().c_str());
    
	return strtmpID.back();
    
}
bool LocalFileManger::mkDirWithFilePath(std::string pFilePahtName)
{
	std::string heroIDAndLeve = pFilePahtName; //’‚¿Ô”–Œƒº˛¬∑æ∂∫ÕŒƒº˛º–√˚
    
	const char *expr = heroIDAndLeve.c_str();
    
	char *buf = new char[strlen(expr)+1];
	strcpy(buf, expr);
    
	char* token = strtok( buf, "/\\");
	std::vector<std::string> strtmpID;
    
	while( token != NULL )
	{
		/* While there are tokens in "string" */
		//printf( "%s ", token );
		std::string tmpx = token;
		strtmpID.push_back(tmpx);
		/* Get next token: */
		token = strtok( NULL, "/\\");
	}
    
	std::string pathroot = FileUtils::getInstance()->getWritablePath();
    
	int counttmp = (int)strtmpID.size();
	for (int i = 0; i < counttmp-1;i++ )
	{
		pathroot += strtmpID[i];
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
		int ret = mkdir(pathroot.c_str(), S_IRWXU | S_IRWXG |  S_IRWXO);
        
 		if (ret != 0 && (errno != EEXIST))
 		{
 			log("create dir eorr...");
 			CCAssert(ret != 0, "mkdir erro...");
 			return false;
 		}
#else
        
		BOOL ret = CreateDirectoryA(pathroot.c_str(), NULL);
        
		if (!ret && ERROR_ALREADY_EXISTS != GetLastError())
            
		{
			CCLog("create dir eorr...");
			return false;
		}
        
        
        
#endif
	}
	return true;
}
bool LocalFileManger::saveDataToFile(char *pContent,std::string pFileName)
{
	//µ⁄“ªªÒ»°¥¢¥ÊµƒŒƒº˛¬∑æ∂
	std::string path = FileUtils::getInstance()->getWritablePath() + pFileName;
    
	this->mkDirWithFilePath(pFileName);
    
	//¥¥Ω®“ª∏ˆŒƒº˛÷∏’Î
	//¬∑æ∂°¢ƒ£ Ω
	FILE* file = fopen(path.c_str(), "w+");
	if (file) {
		fputs(pContent, file);
		fclose(file);
	}
	else
		CCLOG("save file error.");
    
	return false;
}
bool LocalFileManger::saveDataToFile(std::string pContent,std::string pFileName)
{
	char *c = const_cast<char*>(pContent.c_str());
	bool xx = this->saveDataToFile(c,pFileName);
	return xx;
}

