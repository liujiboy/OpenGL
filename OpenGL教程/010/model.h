//
//  model.h
//  OpenGL
//
//  Created by  刘骥 on 13-8-13.
//  Copyright (c) 2013年  刘骥. All rights reserved.
//

#ifndef OpenGL_model_h
#define OpenGL_model_h
#include <GLUT/GLUT.h>
#include <vector>
using namespace std;
//三维向量
typedef struct {
    float x;
    float y;
    float z;
}Vec3;
//二维向量
typedef struct {
    float x;
    float y;
}Vec2;
//顶点
typedef struct{
    Vec3 v; //坐标
    Vec2 vt;//纹理
    Vec3 vn;//法线
}Vertex;
//加载模型
void loadModel(const char* filename,GLuint vertex_position_loc,GLuint vertex_texture_coord_loc,GLuint vertex_normal_loc,GLuint&vao_id,GLsizei&size);

#endif
