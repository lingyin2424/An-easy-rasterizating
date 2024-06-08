//
// Created by Lingyin on 2024/4/6.
//

#ifndef ASMALLRENDER_CAMERA_H
#define ASMALLRENDER_CAMERA_H

#include "Translate.h"

struct Camera{
    Camera();
    explicit Camera(float zNear, float zFar, float width, float height);
    Translate translate;
    [[nodiscard]] Mat4 getViewMat() const;

    float zNear, zFar, width, height;
};

#endif //ASMALLRENDER_CAMERA_H
