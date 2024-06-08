//
// Created by Lingyin on 2024/4/5.
//
#include <iostream>
#include <cstring>
#include <cmath>
#include "SomeVal.h"
#include <cassert>
#include <random>
#include <iomanip>
// #define float long double
// #define double long double



#ifndef ASMALLRENDER_VEC_H
#define ASMALLRENDER_VEC_H
const double M = 2e5;
using std::ostream;
class Vec2{
public:
    float e[2]{};
    Vec2();
    explicit Vec2(const float & e0, const float & e1);

    [[nodiscard]] float x() const;
    [[nodiscard]] float y() const;

    float operator [] (const int & i) const;
    float & operator [] (const int & i);

    Vec2 operator -() const;
    Vec2& operator /= (const float & v);
    Vec2& operator *= (const float & v);
    Vec2& operator += (const Vec2 & vv);
    Vec2& operator -= (const Vec2 & vv);

    [[nodiscard]] float mo() const;
    [[nodiscard]] Vec2 normal() const;
};

class Vec3{
public:
    float e[3]{};
    Vec3();
    explicit Vec3(const float & e0, const float & e1, const float & e2);
    explicit Vec3(const Vec2 & vv, const float & e2);
    [[nodiscard]] float x() const;
    [[nodiscard]] float y() const;
    [[nodiscard]] float z() const;

    float operator [] (const int & i) const;
    float & operator [] (const int & i);

    Vec3 operator -() const;
    Vec3& operator /= (const float & v);
    Vec3& operator *= (const float & v);
    Vec3& operator += (const Vec3 & vvv);
    Vec3& operator -= (const Vec3 & vvv);

    [[nodiscard]] float mo() const;
    [[nodiscard]] Vec3 normal() const;
};

class Vec4{
public:
    float e[4]{};
    Vec4();
    explicit Vec4(const float & e0, const float & e1, const float & e2, const float & e3);
    explicit Vec4(const Vec2 & vv, const float & e2, const float & e3);
    explicit Vec4(const Vec3 & vvv, const float & e3);

    [[nodiscard]] float x() const;
    [[nodiscard]] float y() const;
    [[nodiscard]] float z() const;
    [[nodiscard]] float w() const;

    float operator [] (const int & i) const;
    float & operator [] (const int & i);

    Vec4 operator -() const;
    Vec4& operator /= (const float & v);
    Vec4& operator *= (const float & v);
    Vec4& operator += (const Vec4 & vvvv);
    Vec4& operator -= (const Vec4 & vvvv);

    [[nodiscard]] float mo() const;
    [[nodiscard]] Vec4 normal() const;
    [[nodiscard]] Vec4 one() const;
    [[nodiscard]] Vec3 xyz() const;
    [[nodiscard]] Vec2 xy() const;

    [[nodiscard]] Vec4 color255() const ;

    void print() const;
};

Vec2 operator + (const Vec2 & a, const Vec2 & b);
Vec2 operator - (const Vec2 & a, const Vec2 & b);

Vec3 operator + (const Vec3 & a, const Vec3 & b);
Vec3 operator - (const Vec3 & a, const Vec3 & b);

Vec4 operator + (const Vec4 & a, const Vec4 & b);
Vec4 operator - (const Vec4 & a, const Vec4 & b);


float cross(const Vec2 & a, const Vec2 & b);
Vec3 cross(const Vec3 & a, const Vec3 & b);

float dot(const Vec2 & a, const Vec2 & b);
float dot(const Vec3 & a, const Vec3 & b);
float dot(const Vec4 & a, const Vec4 & b);

ostream& operator << (ostream& a, const Vec2 & b);
ostream& operator << (ostream& a, const Vec3 & b);
ostream& operator << (ostream& a, const Vec4 & b);

Vec2 operator * (const float& k, const Vec2& vv);
Vec2 operator * (const Vec2& vv, const float& k);
Vec2 operator / (const Vec2& vv, const float& k);

Vec3 operator * (const float& k, const Vec3& vvv);
Vec3 operator * (const Vec3& vvv, const float& k);
Vec3 operator / (const Vec3& vvv, const float& k);

Vec4 operator * (const float& k, const Vec4& vvvv);
Vec4 operator * (const Vec4& vvvv, const float& k);
Vec4 operator / (const Vec4& vvvv, const float& k);


Vec2 operator * (const Vec2& a, const Vec2 & b);
Vec3 operator * (const Vec3& a, const Vec3 & b);
Vec4 operator * (const Vec4& a, const Vec4 & b);

Vec4 cross(const Vec4& a, const Vec4& b);

float pow2(const float& x);

float dis2(const Vec4& a, const Vec4& b);

Vec4 pow(const Vec4& v, const float& y);


#endif //ASMALLRENDER_VEC_H
