//
// Created by Lingyin on 2024/4/6.
//

#ifndef ASMALLRENDER_TRANSLATE_H
#define ASMALLRENDER_TRANSLATE_H

#include"math/Vec.h"
#include"math/Mat.h"

struct Translate{
public:
    Vec4 position, aim, up;

    Translate();
    float raito = 1;

    [[nodiscard]] Mat4 getTranslateMat() const;
    [[nodiscard]] Mat4 getInvTranslateMat() const;

    void add(float x, float y);
    void goInto(float x);

    void addX(float x);
    void addY(float y);

    void setPosition(float x, float y, float z);
};


#endif //ASMALLRENDER_transLATE_H
