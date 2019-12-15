#include <iostream>
#include <fstream>
#include <string>
//注意头文件的位置可能不同
//使用glew的应该#include<glew.h>
//使用freeglut的应该#include<freeglut.h>
//glew的头文件必须在freeglut之前被包含
//Mac电脑很简单只需要#include <GLUT/glut.h>
#include <GLUT/glut.h>
using namespace std;
#include "utils.h"
#include "model.h"
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
//模型顶点
vector<Vertex> vertices;
//Shader程序id
GLuint pid;
//纹理id
GLuint texture_id;
GLuint texture2_id;
//缩放比例
float s=1.0f;
//x旋转角度
float xAngle=0.0f;
//y旋转角度
float yAngle=0.0f;
//宽高比
float aspect=1.0f;
//键盘事件
void keyboard(unsigned char key,int x,int y)
{
    switch (key) {
        case 'a':
            yAngle-=1.0f;
            break;
        case 'd':
            yAngle+=1.0f;
            break;
        case 'w':
            xAngle-=1.0f;
            break;
        case 's':
            xAngle+=1.0f;
            break;
        case 'z':
            s+=0.01f;
            break;
        case 'x':
            s-=0.01f;
            break;
        case 27:
            exit(0);
        default:
            break;
    }
    //重绘图形
    glutPostRedisplay();
}
//鼠标事件
void mouse(int button,int state,int x,int y)
{
}
//空闲事件
void idle()
{
}
//窗口大小改变事件
void reshape(int w,int h)
{
    //设置图形在窗体中的显示范围
    glViewport(0, 0, w, h);
    //每次窗体大小变化重新计算宽高比
    aspect=(float)w/(float)h;
}
//绘制图形
void display()
{
    //用颜色RGBA(0,0,0,1)清除整个窗口
    glClearColor(0.0f, 0.0f, 0.0f,1.0f);
    //清除颜色缓冲区
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    //启动深度测试
    glEnable(GL_DEPTH_TEST);
    //平移矩阵tm
    glm::mat4 tm=glm::translate(0.0f, 0.0f, -2.0f);
    //缩放矩阵sm
    glm::mat4 sm=glm::scale(s, s,s);
    //旋转矩阵xrm
    glm::mat4 xrm=glm::rotate(xAngle, 1.0f, 0.0f, 0.0f);
    //旋转矩阵yrm
    glm::mat4 yrm=glm::rotate(yAngle, 0.0f, 1.0f,0.0f);
    //变换矩阵M
    glm::mat4 model_view_matrix=tm*xrm*yrm*sm;
    //投影矩阵P
    glm::mat4 projection_matrix=glm::perspective(100.f, aspect, 1.0f, 10.0f);
    //使用Shader程序
    glUseProgram(pid);
    //获取vertex shader中全局变量model_view_matrix的位置
    GLuint m=glGetUniformLocation(pid, "model_view_matrix");
    //设置vertex shader中全局变量model_view_matrix的值
    glUniformMatrix4fv(m, 1, false, &model_view_matrix[0][0]);
    //获取vertex shader中全局变量projection_matrix的位置
    GLuint p=glGetUniformLocation(pid, "projection_matrix");
    //设置vertex shader中全局变量projection_matrix的值
    glUniformMatrix4fv(p, 1, false, &projection_matrix[0][0]);
    //使用纹理，将纹理绑定到GL_TEXTURE0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    //将纹理与fragment shader中的texture变量关联
    GLuint tex_loc = glGetUniformLocation(pid, "texture");
    glUniform1i(tex_loc, 0);
    //使用纹理，将纹理绑定到GL_TEXTURE1
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2_id);
    //将纹理与fragment shader中的texture变量关联
    GLuint tex_loc2 = glGetUniformLocation(pid, "texture2");
    glUniform1i(tex_loc2, 1);
    //绘制模型
    glBegin(GL_QUADS);//设置图形拓扑为四边形QUADS
    for(int i=0;i<vertices.size();i++)
    {
        //给出四边形的4个顶点坐标
        Vertex vertex=vertices[i];
        glTexCoord2f(vertex.vt.x, vertex.vt.y);
        glNormal3f(vertex.vn.x,vertex.vn.y,vertex.vn.z);
        glVertex3f(vertex.v.x,vertex.v.y,vertex.v.z);
    }
    glEnd();//绘图结束
    //输出图形
    glutSwapBuffers();
}



//初始化
void init()
{
    pid=load_shader_program("vertex.glsl", "fragment.glsl");
    texture_id=create_texture_2d("texture.bmp");
    texture2_id=create_texture_2d("texture2.bmp");
    loadModel("cube.obj", vertices);
    
}

int main(int argc, char * argv[])
{
    //初始化glut
    glutInit(&argc,argv);
    //初始化OpenGL显示模式
    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
    //初始化窗口大小
    glutInitWindowSize(800,800);
    //创建窗口
    glutCreateWindow("OpenGL教程");
    //glewInit(); 使用glew必须添加
    //初始化
    init();
    //注册窗口大小改变事件处理函数
    glutReshapeFunc(reshape);
    //注册绘制图形函数
    glutDisplayFunc(display);
    //注册键盘事件函数
    glutKeyboardFunc(keyboard);
    //注册鼠标事件函数
    glutMouseFunc(mouse);
    //注册空闲事件函数
    glutIdleFunc(idle);
    //开始事件循环
    glutMainLoop();
    return 0;
}

