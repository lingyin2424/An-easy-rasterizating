//
// Created by Lingyin on 2024/4/6.
//

#include "Camera.h"

Camera::Camera() = default;
Camera::Camera(float zNear, float zFar, float width, float height){
    this -> zNear = zNear;
    this -> zFar = zFar;
    this -> width = width;
    this -> height = height;
}

Mat4 Camera::getViewMat() const{
//    return translate.getTranslateMat().Inv();
    return translate.getInvTranslateMat();
}
