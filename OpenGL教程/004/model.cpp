//
//  model.cpp
//  OpenGL
//
//  Created by  刘骥 on 13-8-13.
//  Copyright (c) 2013年  刘骥. All rights reserved.
//

#include "model.h"
#include <fstream>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
using namespace std;
void loadModel(const char* filename,vector<Vertex>& vertices)
{
    //临时存储解析的v、vt和vn数据
    vector<Vec3> vs;
    vector<Vec2> vts;
    vector<Vec3> vns;
    ifstream in(filename);
    string line;
    while(!in.eof())
    {
        getline(in, line);
        //将“/”替换为空格，便于解析f
        replace(line.begin(), line.end(), '/', ' ');
        //下面用istringstream来解析字符串
        istringstream iss(line);
        string sub;
        iss>>sub;
        if (sub=="v") {
            //解析v
            Vec3 v;
            iss>>v.x>>v.y>>v.z;
            vs.push_back(v);
        }
        else if (sub=="vt"){
            //解析vt
            Vec2 vt;
            iss>>vt.x>>vt.y;
            vts.push_back(vt);

        }
        else if (sub=="vn"){
            //解析vn
            Vec3 vn;
            iss>>vn.x>>vn.y>>vn.z;
            vns.push_back(vn);
        }
        else if (sub=="f"){
            //解析f
            for(int i=0;i<4;i++)
            {
                int vi,vti,vni;
                iss>>vi>>vti>>vni;
                Vertex vertex;
                vertex.v=vs[vi-1];
                vertex.vn=vns[vni-1];
                vertex.vt=vts[vti-1];
                vertices.push_back(vertex);
            }
        }
        
    }
}