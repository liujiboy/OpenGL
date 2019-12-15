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
#include <GLUT/GLUT.h>
using namespace std;
//创建VAO
void createVAO(const vector<Vertex>& vertices, GLuint vertex_position_loc,GLuint vertex_texture_coord_loc,GLuint vertex_normal_loc,GLuint&vao_id)
{
    GLuint vbo_id;
    glGenBuffers(1, &vbo_id);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    //传输数据
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices.size(), &vertices[0],
                 GL_STATIC_DRAW);
    //创建VAO，非Mac电脑请使用glGenVertexArrays和glBindVertexArray
    glGenVertexArraysAPPLE( 1, &vao_id );
    glBindVertexArrayAPPLE(vao_id);
    //启用顶点属性
    glEnableVertexAttribArray(vertex_position_loc);
    glEnableVertexAttribArray(vertex_texture_coord_loc);
    glEnableVertexAttribArray(vertex_normal_loc);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_id);
    //vertex_position_loc与顶点数据映射
    glVertexAttribPointer( vertex_position_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),0);
    //vertex_texture_coord_loc与纹理数据映射
    glVertexAttribPointer( vertex_texture_coord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),(GLvoid*)(0+sizeof(Vec3)));
    //vertex_normal_loc与法线数据映射
    glVertexAttribPointer( vertex_normal_loc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),(GLvoid*)(0+sizeof(Vec3)+sizeof(Vec2)));

}
void loadModel(const char* filename,GLuint vertex_position_loc,GLuint vertex_texture_coord_loc,GLuint vertex_normal_loc,GLuint&vao_id,GLsizei&size)
{
    vector<Vertex> vertices;
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
    size=(GLsizei)vertices.size();
    //创建VBO
    createVAO(vertices,vertex_position_loc,vertex_texture_coord_loc,vertex_normal_loc,vao_id);
   
}
