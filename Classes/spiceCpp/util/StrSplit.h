//
//  StrSplit.h
//  game3
//  这个是创建一些基本的cocos2d类时会用到
//  Created by 俊盟科技1 on 9/20/14.
//
//

#ifndef __game3__StrSplit__
#define __game3__StrSplit__

#include <iostream>
#include <string>
#include <vector>

using namespace std;
typedef std::vector<std::string> strArray;


class StrSplit
{
    
public:
    ~StrSplit(){};
    
    static inline strArray split(const std::string& src,const std::string& tokenx)
    {
        
        //字符串分割,把多张图片名，分开存入map
        const char *expr = src.c_str();
        char *buf = new char[strlen(expr)+1];
        strcpy(buf, expr);
        char* token = strtok( buf, tokenx.c_str());
        std::vector<std::string> strtmpID;
        while( token != NULL )
        {
            /* While there are tokens in "string" */
            //printf( "%s ", token );
            std::string tmpx = token;
            strtmpID.push_back(tmpx);
            /* Get next token: */
            token = strtok( NULL, tokenx.c_str());
        }
        return strtmpID;
    }
    static void split(const std::string& src, const std::string& token, strArray& vect)
    {
        size_t nend = 0;
        size_t nbegin = 0;
        size_t tokenSize = token.size();
        while(nend != std::string::npos)
        {
            nend = src.find(token, nbegin);
            if(nend == std::string::npos)
                vect.push_back(src.substr(nbegin, src.length()-nbegin));
            else
                vect.push_back(src.substr(nbegin, nend-nbegin));
            nbegin = nend + tokenSize;
        }
    }
};





#endif /* defined(__game3__StrSplit__) */
