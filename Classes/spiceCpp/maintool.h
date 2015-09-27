//
//  maintool.h
//  myspice
//  使用结构体来进行游戏属性存储会比较方便快捷
//
//  Created by woodcol on 15/9/17.
//
//

#ifndef myspice_maintool_h
#define myspice_maintool_h

void maintool2(int argc, const char * argv[]);
void mainInFile2(FILE* nin);
void mainOutFile2(FILE* nout);
void mainErrFile2(FILE* nerr);
void maintool(int argc,const char **argv)
{
    //mainc(argc,argv);
    maintool2(argc,argv);
    
}

void mainInFile(FILE* nin)
{
    mainInFile2(nin);
}

void mainOutFile(FILE* nout)
{
    mainOutFile2(nout);
}

void mainErrFile(FILE* nerr)
{
    mainErrFile2(nerr);
}

#endif
