//
//  main.cpp
//  spiceCpp
//
//  Created by woodcol on 15/9/15.
//  Copyright (c) 2015年 woodcol. All rights reserved.
//

#include <iostream>
#include <stdio.h>

extern "C" {
//#include "mainc.h"
//    int mainc(int argc, char **argv);
#include "testc1.h"
    int c1fun(int argc, const char * argv[]);
    void setCP_nIN(FILE* nin);
    void setCP_nOUT(FILE* nout);
    void setCP_nErr(FILE* nerr);
}

void maintool2(int argc, const char * argv[]) {
    //mainc(argc,argv);
    c1fun(argc,argv);
    
}
void mainInFile2(FILE* nin)
{
    setCP_nIN(nin);
}

void mainOutFile2(FILE* nout)
{
    setCP_nOUT(nout);
}

void mainErrFile2(FILE* nerr)
{
    setCP_nErr(nerr);
}