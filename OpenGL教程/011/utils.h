#ifndef OpenGL_utils_h
#define OpenGL_utils_h
#include<GLUT/GLUT.h>
#include <iostream>
#include <fstream>
#include "EasyBMP.h"
/*
 读取文件的所有内容
 */
char* get_file_contents(const char *filename);
//打印shader程序的错误
void print_shader_log(GLuint id);
//创建Shader
GLuint create_shader(GLenum type,const char* shader_file);
//加载Shader程序
GLuint load_shader_program(const char* vertex_shader_file,const char* fragment_shader_file);
//加载纹理
GLuint create_texture_2d(const char* texture_file);
//创建纹理
GLuint create_texture_2d(int width,int height);
#endif
