
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
#include<cmath>
#include "utils.h"
GLuint pid;
//偏移量
float tx=0.0f;
float ty=0.0f;
//缩放比例
float sx=1.0f;
float sy=1.0f;
//旋转角度
float degree=0.0f;
//键盘事件
void keyboard(unsigned char key,int x,int y)
{
    switch (key) {
        case 'a':
            tx-=0.1f;
            break;
        case 'd':
            tx+=0.1f;
            break;
        case 'w':
            ty+=0.1f;
            break;
        case 's':
            ty-=0.1f;
            break;
        case 'z':
            sx+=0.1f;
            break;
        case 'x':
            sx-=0.1f;
            break;
        case 'c':
            sy+=0.1f;
            break;
        case 'v':
            sy-=0.1f;
            break;
        case  '1':
            degree+=1.0f;
            break;
        case '2':
            degree-=1.0f;
            break;
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
}
//绘制图形
void display()
{
    //用颜色RGBA(0,0,0,1)清除整个窗口
    glClearColor(0.0f, 0.0f, 0.0f,1.0f);
    //清除颜色缓冲区
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    //平移矩阵tm
    Matrix tm=translate(tx, ty, 0.0f);
    //缩放矩阵sm
    Matrix sm=scale(sx, sy, 1.0f);
    //旋转矩阵rm
    Matrix rm=rotate(0.0f,0.0f,1.0f,degree);
    //M=tm*rm*sm
    Matrix M=tm*rm*sm;
    Matrix P=perspective(120.0f,10.0f,10.0f,0.0f,10.0f);
    //使用Shader程序
    glUseProgram(pid);
    //获取vertex shader中全局变量m的位置
    GLuint m=glGetUniformLocation(pid, "m");
    //设置vertex shader中全局变量m的值
    glUniformMatrix4fv(m, 1, false, M.m);
    //获取vertex shader中全局变量p的位置
    GLuint p=glGetUniformLocation(pid, "p");
    //设置vertex shader中全局变量m的值
    glUniformMatrix4fv(p, 1, false, P.m);
    //绘制正方形
    glBegin(GL_QUADS);//设置图形拓扑为四边形QUADS
        //给出四边形的4个顶点坐标
        glVertex3f(-0.5f,0.5f,-0.5f);
        glVertex3f(0.0f,0.5f,-0.5f);
        glVertex3f(0.0f,0.0f,-0.5f);
        glVertex3f(-0.5f,0.0f,-0.5f);
        //再绘制一个四边形
        glVertex3f(0.2f,0.5f,-1.0f);
        glVertex3f(0.7f,0.5f,-1.0f);
        glVertex3f(0.7f,0.0f,-1.0f);
        glVertex3f(0.2f,0.0f,-1.0f);
    glEnd();//绘图结束
    //输出图形
    glutSwapBuffers();
}
//初始化
void init()
{
    //创建shader程序
    pid=glCreateProgram();
    //创建vertex shader
    GLuint vid=glCreateShader(GL_VERTEX_SHADER);
    //从文件中读取vertex shader程序
    const char* vertexShader=get_file_contents("vertex.glsl");
    //指定vertex shader的源程序
    glShaderSource(vid, 1,&vertexShader,NULL);
    //编译vertex shader
    glCompileShader(vid);
    //将vertex shader与shader程序关联
    glAttachShader(pid, vid);
    delete[] vertexShader;
    //创建fragment shader
    GLuint fid=glCreateShader(GL_FRAGMENT_SHADER);
    //从文件中读取fragment shader程序
    const char* fragmentShader=get_file_contents("fragment.glsl");
    //指定fragment shader的源程序
    glShaderSource(fid, 1,&fragmentShader,NULL);
    //编译fragment shader
    glCompileShader(fid);
    //将fragment shader与shader程序关联
    glAttachShader(pid, fid);
    delete[] fragmentShader;
    //链接shader程序
    glLinkProgram(pid);
    
}
int main(int argc, char * argv[])
{
    //初始化glut
    glutInit(&argc,argv);
    //初始化OpenGL显示模式
    glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
    //初始化窗口大小
    glutInitWindowSize(320,320);
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

