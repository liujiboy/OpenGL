//
//  utils.cpp
//  OpenGL
//
//  Created by  刘骥 on 13-8-11.
//  Copyright (c) 2013年  刘骥. All rights reserved.
//

#include "utils.h"

#include <fstream>
using namespace std;
/*
 读取文件的所有内容
 */
char* get_file_contents(const char *filename)
{
    ifstream in(filename, ios::in | ios::binary);
   
        in.seekg(0, ios::end);
        long size=in.tellg();
        char* contents=new char[size+1];
        in.seekg(0, ios::beg);
        in.read(contents, size);
        in.close();
        contents[size]=NULL;
        return contents;
  
}

