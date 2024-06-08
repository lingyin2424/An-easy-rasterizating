//
// Created by Lingyin on 2024/4/5.
//

#ifndef ASMALLRENDER_MAT_H
#define ASMALLRENDER_MAT_H

#include<cmath>
#include<cstring>
#include<initializer_list>
#include<iostream>
#include<cassert>
#include<iomanip>
#include<algorithm>
#include "Vec.h"
#include <cmath>
#include "SomeVal.h"
#include "iomanip"

struct Mat4{
    float val[4][4]{};


// constucter
    explicit Mat4(); // E();
    explicit Mat4(float v);
    Mat4(const std::initializer_list<float> & args);


// selfT
public:
    [[nodiscard]] Mat4 T() const; // return T
    [[nodiscard]] Mat4 Inv() const; // return Inv
    float* operator [] (const int & i) const;
    void print() const;
};

Mat4 operator * (const Mat4 & a, const Mat4 & b);
Mat4 operator * (const float & k, const Mat4 & b);
Mat4 operator * (const Mat4 & b, const float & k);
Mat4 operator + (const Mat4 & a, const Mat4 & b);
Mat4 operator - (const Mat4 & a, const Mat4 & b);
Vec4 operator * (const Mat4 & a, const Vec4 & b);

Mat4 getProjectionMat(const float& w, const float& h, const float& n, const float& f); // to[-1, +1]^3
Mat4 getOrthoMat(const float& w, const float& h, const float& n, const float& f); // to[-1, +1]^3
Mat4 getOrthoMat(const float& l, const float& r, const float &d, const float& u, const float& n, const float& f);

#endif //ASMALLRENDER_MAT_H
