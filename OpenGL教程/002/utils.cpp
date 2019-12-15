//
//  utils.cpp
//  OpenGL
//
//  Created by  刘骥 on 13-8-11.
//  Copyright (c) 2013年  刘骥. All rights reserved.
//

#include "utils.h"
#include <cmath>
#include <fstream>
using namespace std;
/*
    创建单位矩阵
    1   0   0   0
    0   1   0   0
    0   0   1   0
    0   0   0   1
*/
Matrix identity()
{
    Matrix mat;
    for(int i=0;i<16;i++)
        mat.m[i]=0.0f;
    mat.m[0] = mat.m[5] = mat.m[10] = mat.m[15] = 1.0f;
    return mat;
}
/*
    创建平移矩阵
    1   0   0   tx
    0   1   0   ty
    0   0   1   tz
    0   0   0   1
*/
Matrix translate(float tx, float ty, float tz)
{
    Matrix mat=identity();
    mat.m[12] = tx;
    mat.m[13] = ty;
    mat.m[14] = tz;
    return mat;
}
/*
    创建绕Z轴旋转的矩阵
    cos -sin    0   0
    sin cos     0   0
    0   0       1   0
    0   0       0   1
*/
Matrix rotationZ(float degree)
{
    Matrix mat=identity();
    
    mat.m[0] = cos(degree);
    mat.m[1] = sin(degree);
    mat.m[4] = -sin(degree);
    mat.m[5] = cos(degree);
    return mat;
}
/*
    创建缩放矩阵
    sx  0   0   0
    0   sy  0   0
    0   0   sz  0
    0   0   0   1
 */
Matrix scale(float sx,float sy,float sz)
{
    Matrix mat=identity();
    mat.m[0] = sx;
    mat.m[5] = sy;
    mat.m[10] = sz;
    return mat;
}
/*
    绕任意轴旋转
*/
Matrix rotate(float rx,float ry,float rz,float degree)
{
    Matrix mat=identity();
    //化为单位向量
    float length=sqrt(rx*rx+ry*ry+rz*rz);
    rx=rx/length;
    ry=ry/length;
    rz=rz/length;
    //一点小技巧免得反复计算cos和sin
    float a=cos(degree);
    float b=sin(degree);
    mat.m[0]=a+(1-a)*rx*rx;
    mat.m[1]=(1-a)*rx*ry+rz*b;
    mat.m[2]=(1-a)*rx*rz-ry*b;
    mat.m[4]=(1-a)*rx*ry-rz*b;
    mat.m[5]=a+(1-a)*ry*ry;
    mat.m[6]=(1-a)*ry*rz+rx*b;
    mat.m[8]=(1-a)*rx*rz+ry*b;
    mat.m[9]=(1-a)*ry*rz-rx*b;
    mat.m[10]=a+(1-a)*rz*rz;
    return mat;
}
/*
    矩阵乘法
 */
Matrix multiply(const Matrix&m0, const Matrix&m1)
{
    Matrix mat;
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
        {
            mat.m[i+j*4]=0;
            for(int k=0;k<4;k++)
                mat.m[i+j*4]+=m0.m[i+k*4]*m1.m[k+j*4];
        }
    return mat;
}
/*
 矩阵乘法（运算符重载的方法）
 */
Matrix operator*(const Matrix&m0, const Matrix&m1)
{
    Matrix mat;
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
        {
            mat.m[i+j*4]=0;
            for(int k=0;k<4;k++)
                mat.m[i+j*4]+=m0.m[i+k*4]*m1.m[k+j*4];
        }
    return mat;
}
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
/*
 求解透视投影矩阵
 */
Matrix  perspective(float fovy, float w,float h, float near, float far)
{
    //角度化幅度
    float radians = fovy / 2.0f*3.1415926f/180.0f;
    float deltaZ = far - near;
    float sine = sin(radians);
    float aspect=w/h;
    if ((deltaZ == 0) || (sine == 0) || (aspect == 0))
    {
        return identity();
    }
    float cotangent = cos(radians) / sine;
    Matrix mat=identity();
    mat.m[0] = cotangent / aspect;
    mat.m[5] = cotangent;
    mat.m[10] = -(far + near) / deltaZ;
    mat.m[11] = -1;
    mat.m[14] = -2 * near * far / deltaZ;
    mat.m[15] = 0;
    return mat;
}
