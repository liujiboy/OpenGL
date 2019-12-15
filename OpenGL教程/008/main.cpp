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
#include "camera.h"
Camera camera(100.0f,1.0f,1.0f,1000.0f);
//模型顶点
vector<Vertex> vertices;
//Shader程序id
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
            camera.move_right(-0.1f);
            break;
        case 'd':
            camera.move_right(0.1f);
            break;
        case 'w':
            camera.move_forward(0.1f);
            break;
        case 's':
            camera.move_forward(-0.1f);
            break;
        case '1':
            camera.rotate_up(1.0f);
            break;
        case '2':
            camera.rotate_up(-1.0f);
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
    camera.set_aspect((float)w/(float)h);
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
    //模型矩阵
    glm::mat4 model_view_matrix=camera.get_model_view_matrix();
    //投影矩阵
    glm::mat4 projection_matrix=camera.get_projection_matrix();
    //获取normal_matrix
    glm::mat3 normal_matrix=glm::transpose(glm::inverse(glm::mat3(model_view_matrix)));
    //使用Shader程序
    glUseProgram(pid);
    //获取vertex shader中全局变量model_view_matrix的位置
    GLuint model_view_matrix_loc=glGetUniformLocation(pid, "model_view_matrix");
    //设置vertex shader中全局变量model_view_matrix的值
    glUniformMatrix4fv(model_view_matrix_loc, 1, false, &model_view_matrix[0][0]);
    //获取vertex shader中全局变量projection_matrix的位置
    GLuint projection_matrix_loc=glGetUniformLocation(pid, "projection_matrix");
    //设置vertex shader中全局变量projection_matrix的值
    glUniformMatrix4fv(projection_matrix_loc, 1, false, &projection_matrix[0][0]);
    //获取vertex shader中全局变量normal_matrix的位置
    GLuint normal_matrix_loc=glGetUniformLocation(pid, "normal_matrix");
    //设置vertex shader中全局变量normal_matrix的值
    glUniformMatrix3fv(normal_matrix_loc, 1, false, &normal_matrix[0][0]);
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



//初始化
void init()
{
    pid=load_shader_program("vertex.glsl", "fragment.glsl");
    loadModel("cube.obj", vertices);
    //设置相机初始位置
    camera.set_position(0.0f,0.0f,2.0f);
    
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

