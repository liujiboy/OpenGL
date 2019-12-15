//
//  camera.cpp
//  OpenGL
//
//  Created by  刘骥 on 13-8-14.
//  Copyright (c) 2013年  刘骥. All rights reserved.
//

#include "camera.h"
Camera::Camera(float fovy,float aspect,float near,float far):position(0.0f,0.0f,0.0f),forward(0.0f,0.0f,-1.0f),up(0.0f,1.0f,0.0f),right(1.0f,0.0f,0.0f)
{
    this->fovy=fovy;
    this->aspect=aspect;
    this->near=near;
    this->far=far;
}
void Camera ::move_forward(float d)
{
    position+=forward*d;
}
void Camera::move_right(float d)
{
    position+=right*d;
}
void Camera::move_up(float d)
{
    position+=up*d;
}
void Camera::rotate_forward(float angle)
{
    glm::mat3 mat=glm::mat3(glm::rotate(angle, forward));
    up=mat*up;
    right=mat*right;
}
void Camera::rotate_right(float angle)
{
    glm::mat3 mat=glm::mat3(glm::rotate(angle, right));
    up=mat*up;
    forward=mat*forward;
}
void Camera::rotate_up(float angle)
{
    glm::mat3 mat=glm::mat3(glm::rotate(angle, up));
    right=mat*right;
    forward=mat*forward;
}
void Camera::set_position(float x,float y,float z)
{
    this->position=glm::vec3(x,y,z);
}
glm::mat4 Camera::get_model_view_matrix()
{
    glm::vec3 center=position+forward;
    return glm::lookAt(position, center, up);
}
glm::mat4 Camera::get_projection_matrix()
{
    return glm::perspective(fovy, aspect, near, far);
}
void Camera::set_aspect(float aspect)
{
    this->aspect=aspect;
}
