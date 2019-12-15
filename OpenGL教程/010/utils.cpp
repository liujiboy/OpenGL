//
//  utils.cpp
//  OpenGL
//
//  Created by  刘骥 on 13-8-11.
//  Copyright (c) 2013年  刘骥. All rights reserved.
//

#include "utils.h"

/*
 读取文件的所有内容
 */
char* get_file_contents(const char *filename)
{
    std::ifstream in(filename, std::ios::in | std::ios::binary);
   
        in.seekg(0, std::ios::end);
        long size=in.tellg();
        char* contents=new char[size+1];
        in.seekg(0, std::ios::beg);
        in.read(contents, size);
        in.close();
        contents[size]=NULL;
        return contents;
  
}
//打印shader程序的错误
void print_shader_log(GLuint id)
{
    GLint log_length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &log_length);
    
    if (log_length> 0) {
        GLchar* buf=new GLchar[log_length];
        glGetShaderInfoLog(id, log_length, &log_length, buf);
        std::cout<<buf<<std::endl;
        
    }
}
//创建Shader
GLuint create_shader(GLenum type,const char* shader_file)
{
    //创建shader
    GLuint shader_id=glCreateShader(type);
    //从文件中读取shader程序
    const char* source=get_file_contents(shader_file);
    //指定shader的源程序
    glShaderSource(shader_id, 1,&source,NULL);
    //编译shader
    glCompileShader(shader_id);
    //打印shader错误
    print_shader_log(shader_id);
    delete []source;
    return shader_id;
}
//加载Shader程序
GLuint load_shader_program(const char* vertex_shader_file,const char* fragment_shader_file)
{
    //创建shader程序
    GLuint prog_id=glCreateProgram();
    //创建vertex shader
    GLuint vertex_shader_id=create_shader(GL_VERTEX_SHADER, vertex_shader_file);
    
    //创建fragment shader
    GLuint fragment_shader_id=create_shader(GL_FRAGMENT_SHADER,fragment_shader_file);
    //将shader程序关联
    glAttachShader(prog_id, vertex_shader_id);
    glAttachShader(prog_id, fragment_shader_id);
    glLinkProgram(prog_id);
    return prog_id;
}
//加载纹理
GLuint create_texture_2d(const char* texture_file)
{
    //使用EasyBMP加载纹理图片
    //使用什么库没有关系，最终纹理需要生成一个数组，数组的格式如下：
    //{r1,g1,b1,r2,g2,b2,...,rn,gn,bn}，其中ri，gi，bi表示i位置的
    //像素点的rgb值。如果图像由alpha值，数组的格式如下：
    //{r1,g1,b1,a1,r2,g2,b2,a2,...,rn,gn,bn,an}
    BMP texture;
    texture.ReadFromFile(texture_file);
    int texture_width=texture.TellWidth();
    int texture_height=texture.TellHeight();
    unsigned char* texture_data=new unsigned char[texture_width*texture_height*3];
    int i=0;
    for(int row=0;row<texture_height;row++)
        for(int col=0;col<texture_width;col++)
        {
            texture_data[i++]=texture(row,col)->Red;
            texture_data[i++]=texture(row,col)->Green;
            texture_data[i++]=texture(row,col)->Blue;
        }
    //创建纹理，并将纹理数据传递给OpenGL
    GLuint textures[1];
    glGenTextures(1, textures);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    //设置纹理参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //传输数据
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_width,texture_height, 0,GL_RGB, GL_UNSIGNED_BYTE, texture_data);
    delete []texture_data;
    return textures[0];
}
//创建纹理
GLuint create_texture_2d(int width,int height)
{    //创建纹理，并将纹理数据传递给OpenGL
    GLuint textures[1];
    glGenTextures(1, textures);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    //设置纹理参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //传输数据
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width,height, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);
    return textures[0];
}
