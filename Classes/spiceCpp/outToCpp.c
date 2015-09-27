//
//  outToCpp.c
//  spiceCpp
//
//  Created by woodcol on 15/9/15.
//  Copyright (c) 2015年 woodcol. All rights reserved.
//

#include "outToCpp.h"
#include <string.h>

extern void splitstr(char* pstr);
extern void setPintLineCount(int count);
void callClasscpp(char* str)
{
    splitstr(str);
}

void callClassCppLineCount(int count)
{
    setPintLineCount(count);
}
void outToCppClass(char* str)
{
    callClasscpp(str);
}
void outToCppClassLineCount(int count)
{
    callClassCppLineCount(count);
}