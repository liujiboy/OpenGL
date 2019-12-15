
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
#include "model.h"
#include <vector>
vector<Vertex> vertices;
GLuint pid;
//缩放比例
float s=1.0f;
//x旋转角度
float xAngle=0.0f;
//y旋转角度
float yAngle=0.0f;
//键盘事件
void keyboard(unsigned char key,int x,int y)
{
    switch (key) {
        case 'a':
            yAngle-=0.01f;
            break;
        case 'd':
            yAngle+=0.01f;
            break;
        case 'w':
            xAngle-=0.01f;
            break;
        case 's':
            xAngle+=0.01f;
            break;
        case 'z':
            s+=0.01f;
            break;
        case 'x':
            s-=0.01f;
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
    //启动深度测试
    glEnable(GL_DEPTH_TEST);
    //平移矩阵tm
    Matrix tm=translate(0.0f, 0.0f, -2.0f);
    //缩放矩阵sm
    Matrix sm=scale(s, s, s);
    //旋转矩阵xrm
    Matrix xrm=rotate(1.0f,0.0f,0.0f,xAngle);
    //旋转矩阵yrm
    Matrix yrm=rotate(0.0f,1.0f,0.0f,yAngle);
    //变换矩阵M
    Matrix M=tm*yrm*xrm*sm;
    //投影矩阵P
    Matrix P=perspective(120.0f,1.0f,1.0f,10.0f);
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
    //绘制模型
    glBegin(GL_QUADS);//设置图形拓扑为四边形QUADS
    for(int i=0;i<vertices.size();i++)
    {
        //给出四边形的4个顶点坐标
        Vertex vertex=vertices[i];
        glVertex3f(vertex.v.x,vertex.v.y,vertex.v.z);
    }
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
    loadModel("cube.obj", vertices);
    //
    
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

