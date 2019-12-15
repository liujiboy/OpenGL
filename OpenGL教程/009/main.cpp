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
//Vertex Array Object id
GLuint vao_id;
//相机
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
//纹理id
GLuint texture_id;
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
    //使用纹理，将纹理绑定到GL_TEXTURE0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    //将纹理与fragment shader中的texture变量关联
    GLuint tex_loc = glGetUniformLocation(pid, "texture");
    glUniform1i(tex_loc, 0);
    //绘制模型
    glBindVertexArrayAPPLE(vao_id);
    glDrawArrays(GL_QUADS, 0, (GLsizei)vertices.size() );
     //输出图形
    glutSwapBuffers();
}



//初始化
void init()
{
    pid=load_shader_program("vertex.glsl", "fragment.glsl");
    //获取vertex_position、vertex_texture_coord和vertex_normal的位置
    GLuint vertex_position_loc=glGetAttribLocation(pid, "vertex_position");
    GLuint vertex_texture_coord_loc=glGetAttribLocation(pid, "vertex_texture_coord");
    GLuint vertex_normal_loc=glGetAttribLocation(pid, "vertex_normal");
    //加载纹理
    texture_id=create_texture_2d("texture.bmp");
    //加载模型
    loadModel("cube.obj", vertices);
    //设置相机初始位置
    camera.set_position(0.0f,0.0f,2.0f);
    //创建VBO
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

