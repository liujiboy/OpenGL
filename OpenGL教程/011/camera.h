//
//  camera.h
//  OpenGL
//
//  Created by  刘骥 on 13-8-14.
//  Copyright (c) 2013年  刘骥. All rights reserved.
//

#ifndef OpenGL_camera_h
#define OpenGL_camera_h
#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"
#include <iostream>
class Camera{
private:
    //相机位置
    glm::vec3 position;
    //相机正面的朝向
    glm::vec3 forward;
    //相机顶部的朝向
    glm::vec3 up;
    //相机右侧的朝向
    glm::vec3 right;
    //相机视角
    float fovy;
    //相机长宽比
    float aspect;
    //相机近平面
    float near;
    //相机远平面
    float far;
public:
    Camera(float fovy,float aspect,float near,float far);
    void set_position(float x,float y,float z);
    void set_aspect(float aspect);
    //相机向前移动
    void move_forward(float d);
    //相机向上移动
    void move_up(float d);
    //相机向右移动
    void move_right(float d);
    //相机绕forward方向转动
    void rotate_forward(float angle);
    //相机绕up方向转动
    void rotate_up(float angle);
    //相机绕right方向转动
    void rotate_right(float angle);
    //获取模型视图矩阵
    glm::mat4 get_model_view_matrix();
    //获取投影矩阵
    glm::mat4 get_projection_matrix();

};


#endif
