#include <iostream>
#include <sstream>
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
#include "EasyBMP.h"
//Vertex Array Object id
GLuint scene_id;

GLsizei scene_size;

//相机
Camera camera(100.0f,1.0f,1.0f,1000.0f);
//Shader程序id
GLuint pid;

//y旋转角度
float yAngle=0.0f;
//纹理id
GLuint texture_id;
//窗口宽和高
int window_width;
int window_height;
//键盘事件
void keyboard(unsigned char key,int x,int y)
{
    switch (key) {
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
    //每次窗体大小变化重新计算宽高比
    glViewport(0, 0, w, h);
    window_height=h;
    window_width=w;
    camera.set_aspect((float)w/(float)h);
}
void save_to_bmp(const char* file_name)
{
    unsigned char* pixels=new unsigned char[4*window_height*window_width];
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    //读取Frame Buffer中的图像
    glReadPixels(0, 0, window_width, window_height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    BMP bmp;
    bmp.SetSize(window_width, window_height);
    bmp.SetBitDepth(32);
    int i=0;
    for(int row=0;row<window_height;row++)
        for(int col=0;col<window_width;col++)
        {
            RGBApixel pixel;
            pixel.Red=pixels[i];
            pixel.Green=pixels[i+1];
            pixel.Blue=pixels[i+2];
            pixel.Alpha=pixels[i+3];
            bmp.SetPixel(row, col, pixel);
            i+=4;
        }
    bmp.WriteToFile(file_name);
    delete pixels;

}
//绘制图形
void display()
{
    //用颜色RGBA(0,0,0,1)清除整个窗口
    glClearColor(0.0f, 0.0f, 0.0f,1.0f);
    //清除颜色缓冲区
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    //模型矩阵
    glm::mat4 model_view_matrix=glm::translate(0.0f, 0.0f,-10.0f)*glm::rotate(yAngle,0.0f, 1.0f, 0.0f);
    //投影矩阵
    glm::mat4 projection_matrix=glm::perspective(60.0f, 1.0f, 1.0f, 100.0f);
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
    glBindVertexArrayAPPLE(scene_id);
    glDrawArrays(GL_QUADS, 0, scene_size);
    static int index=0;
    stringstream ss;
    ss<<index<<".bmp";
    string file_name;
    ss>>file_name;
    save_to_bmp(file_name.c_str());
    index++;
    //输出图形
    glutSwapBuffers();
}



//初始化
void init()
{
    //启动深度测试
    glEnable(GL_DEPTH_TEST);
    pid=load_shader_program("vertex.glsl", "fragment.glsl");
    //获取vertex_position、vertex_texture_coord和vertex_normal的位置
    GLuint vertex_position_loc=glGetAttribLocation(pid, "vertex_position");
    GLuint vertex_texture_coord_loc=glGetAttribLocation(pid, "vertex_texture_coord");
    GLuint vertex_normal_loc=glGetAttribLocation(pid, "vertex_normal");
    //加载纹理
    texture_id=create_texture_2d("texture.bmp");
    
    loadModel("scene.obj",vertex_position_loc,vertex_texture_coord_loc,vertex_normal_loc,scene_id,scene_size);
    //设置相机初始位置
    camera.set_position(0.0f,0.0f,2.0f);
    
}
//定时事件
void animation(int value)
{
    yAngle++;
    //重绘图形
    glutPostRedisplay();
    //下一次定时
    glutTimerFunc(200, animation,0);
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
    //注册定时事件函数
    glutTimerFunc(200, animation,0);
    //开始事件循环
    glutMainLoop();
    return 0;
}

