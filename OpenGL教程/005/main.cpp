
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
    //计算Normal Matrix
    glm::mat3 normal_matrix=glm::transpose(glm::inverse(glm::mat3(model_view_matrix)));
    //投影矩阵P
    glm::mat4 projection_matrix=glm::perspective(120.f, 1.0f, 1.0f, 10.0f);
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
    //获取vertex shader中全局变量normal_matrix的位置
    GLuint n=glGetUniformLocation(pid, "normal_matrix");
    //设置vertex shader中全局变量normal_matrix的值
    glUniformMatrix3fv(n, 1, false, &normal_matrix[0][0]);
    //绘制模型
    glBegin(GL_QUADS);//设置图形拓扑为四边形QUADS
    for(int i=0;i<vertices.size();i++)
    {
        //给出四边形的4个顶点坐标
        Vertex vertex=vertices[i];
        glNormal3f(vertex.vn.x,vertex.vn.y,vertex.vn.z);
        glVertex3f(vertex.v.x,vertex.v.y,vertex.v.z);
    }
    glEnd();//绘图结束
    //输出图形
    glutSwapBuffers();
}
//打印shader程序的错误
void printShaderLog(GLuint id)
{
    GLint logLength;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);
    
    if (logLength> 0) {
        GLchar* buf=new GLchar[logLength];
        glGetShaderInfoLog(id, logLength, &logLength, buf);
        cout<<buf<<endl;
        
    }
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
    //打印shader错误
    printShaderLog(vid);
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
    //打印shader错误
    printShaderLog(fid);
    //将fragment shader与shader程序关联
    glAttachShader(pid, fid);
    delete[] fragmentShader;
    //链接shader程序
    glLinkProgram(pid);
    loadModel("cube.obj", vertices);
    
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

