//
// Created by Lingyin on 2024/4/5.
//
#include "Vec.h"

#include <cmath>


Vec2::Vec2() = default;
Vec2::Vec2(const float & e0, const float & e1){ e[0] = e0; e[1] = e1; }
float Vec2::x() const { return e[0]; }
float Vec2::y() const { return e[1]; }

float Vec2::operator [] (const int & i) const { assert(i < 2); return e[i]; }
float& Vec2::operator [] (const int & i) { assert(i < 2); return e[i]; }

Vec2 Vec2::operator -() const { return Vec2(-e[0], -e[1]); }
Vec2& Vec2::operator /= (const float & v) { e[0] /= v, e[1] /= v; return (*this); }
Vec2& Vec2::operator *= (const float & v) { e[0] *= v, e[1] *= v; return (*this); }
Vec2& Vec2::operator += (const Vec2 & vv) { e[0] += vv.e[0], e[1] += vv.e[1]; return (*this); }
Vec2& Vec2::operator -= (const Vec2 & vv) { e[0] -= vv.e[0], e[1] -= vv.e[1]; return (*this); }

float Vec2::mo() const { return std::sqrt(e[0] * e[0] + e[1] * e[1]); }
Vec2 Vec2::normal() const { auto m = this -> mo(); return Vec2(e[0] / m, e[1] / m); }



Vec3::Vec3() = default;
Vec3::Vec3(const float & e0, const float & e1, const float & e2) { e[0] = e0; e[1] = e1, e[2] = e2; }
Vec3::Vec3(const Vec2 & vv, const float & e2) { e[0] = vv.x(), e[1] = vv.y(); e[2] = e2; }
float Vec3::x() const { return e[0]; }
float Vec3::y() const { return e[1]; }
float Vec3::z() const { return e[2]; }

float Vec3::operator [] (const int & i) const { assert(i < 3); return e[i]; }
float& Vec3::operator [] (const int & i) { assert(i < 3); return e[i]; }

Vec3 Vec3::operator -() const { return Vec3(-e[0], -e[1], -e[2]); }
Vec3& Vec3::operator /= (const float & v) { e[0] /= v, e[1] /= v, e[2] /= v; return (*this); }
Vec3& Vec3::operator *= (const float & v) { e[0] *= v, e[1] *= v, e[2] *= v; return (*this); }
Vec3& Vec3::operator += (const Vec3 & vvv) { e[0] += vvv.e[0], e[1] += vvv.e[1], e[2] += vvv.e[2]; return (*this); }
Vec3& Vec3::operator -= (const Vec3 & vvv) { e[0] -= vvv.e[0], e[1] -= vvv.e[1], e[2] -= vvv.e[2]; return (*this); }

float Vec3::mo() const { return std::sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]); }
Vec3 Vec3::normal() const { auto m = this -> mo(); return Vec3(e[0] / m, e[1] / m, e[2] / m); }



Vec4::Vec4() = default;
Vec4::Vec4(const float & e0, const float & e1, const float & e2, const float & e3) { e[0] = e0; e[1] = e1, e[2] = e2; e[3] = e3; }
Vec4::Vec4(const Vec2 & vv, const float & e2, const float & e3) { e[0] = vv.x(), e[1] = vv.y(); e[2] = e2; e[3] = e3; }
Vec4::Vec4(const Vec3 & vvv, const float & e3) { e[0] = vvv.x(), e[1] = vvv.y(), e[2] = vvv.z(); e[3] = e3; }

float Vec4::x() const { return e[0]; }
float Vec4::y() const { return e[1]; }
float Vec4::z() const { return e[2]; }
float Vec4::w() const { return e[3]; }

float Vec4::operator [] (const int & i) const { assert(i < 4); return e[i]; }
float& Vec4::operator [] (const int & i) { assert(i < 4); return e[i]; }

Vec4 Vec4::operator -() const { return Vec4(-e[0], -e[1], -e[2], -e[3]); }
Vec4& Vec4::operator /= (const float & v) { e[0] /= v, e[1] /= v, e[2] /= v, e[3] /= v; return (*this); }
Vec4& Vec4::operator *= (const float & v) { e[0] *= v, e[1] *= v, e[2] *= v, e[3] *= v; return (*this); }
Vec4& Vec4::operator += (const Vec4 & vvvv) { e[0] += vvvv.e[0], e[1] += vvvv.e[1], e[2] += vvvv.e[2], e[3] += vvvv.e[3]; return (*this); }
Vec4& Vec4::operator -= (const Vec4 & vvvv) { e[0] -= vvvv.e[0], e[1] -= vvvv.e[1], e[2] -= vvvv.e[2]; e[3] -= vvvv.e[3]; return (*this); }

float Vec4::mo() const { return std::sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2] + e[3] * e[3]); }
Vec4 Vec4::normal() const { auto m = this -> mo(); return Vec4(e[0] / m, e[1] / m, e[2] / m, e[3] / m); }
Vec4 Vec4::one() const { return Vec4(e[0] / e[3], e[1] / e[3], e[2] / e[3], 1); }
Vec3 Vec4::xyz() const { return Vec3(e[0], e[1], e[2]); }
Vec2 Vec4::xy() const { return Vec2(e[0], e[1]); }

void Vec4::print() const{
    std::cout <<  e[0] << ' ' << e[1] << ' ' << e[2] << ' ' << e[3] << '\n';
}

Vec4 Vec4::color255() const {
    using std::min;
    using std::max;

    const float a = max(min((float)254.9, e[0]), (float)0);
    const float b = max(min((float)254.9, e[1]), (float)0);
    const float c = max(min((float)254.9, e[2]), (float)0);

    return Vec4(a, b, c, 0);
}

float cross(const Vec2 & a, const Vec2 & b){
    return a.x() * b.y() - a.y() * b.x();
}

Vec3 cross(const Vec3 & a, const Vec3 & b){
    return Vec3(
            a.y() * b.z() - a.z() * b.y(),
            a.z() * b.x() - a.x() * b.z(),
            a.x() * b.y() - a.y() * b.x()
            );
}

float dot(const Vec2 & a, const Vec2 & b){
    return a.x() * b.x() + a.y() * b.y();
}
float dot(const Vec3 & a, const Vec3 & b){
    return a.x() * b.x() + a.y() * b.y() + a.z() * b.z();
}
float dot(const Vec4 & a, const Vec4 & b){
    return a.x() * b.x() + a.y() * b.y() + a.z() * b.z() + a.w() * b.w();
}

Vec2 operator + (const Vec2 & a, const Vec2 & b){
    return Vec2(a.x() + b.x(), a.y() + b.y());
}
Vec2 operator - (const Vec2 & a, const Vec2 & b){
    return Vec2(a.x() - b.x(), a.y() - b.y());
}

Vec3 operator + (const Vec3 & a, const Vec3 & b){
    return Vec3(a.x() + b.x(), a.y() + b.y(), a.z() + b.z());
}
Vec3 operator - (const Vec3 & a, const Vec3 & b){
    return Vec3(a.x() - b.x(), a.y() - b.y(), a.z() - b.z());
}

Vec4 operator + (const Vec4 & a, const Vec4 & b){
    return Vec4(a.x() + b.x(), a.y() + b.y(), a.z() + b.z(), a.w() + b.w());
}
Vec4 operator - (const Vec4 & a, const Vec4 & b){
    return Vec4(a.x() - b.x(), a.y() - b.y(), a.z() - b.z(), a.w() - b.w());
}

ostream& operator << (ostream& a, const Vec2 & b){
    return a << '[' << b.x() << ' ' << b.y() << ']';
}

ostream& operator << (ostream& a, const Vec3 & b){
    return a << '[' << b.x() << ' ' << b.y() << ' ' << b.z() << ']';
}

ostream& operator << (ostream& a, const Vec4 & b){
    return a << '[' << b.x() << ' ' << b.y() << ' ' << b.z() << ' ' << b.w() << ']';
}

Vec2 operator * (const float& k, const Vec2& vv){
    return Vec2(vv.x() * k, vv.y() * k);
}

Vec2 operator * (const Vec2& vv, const float& k){
    return Vec2(vv.x() * k, vv.y() * k);
}

Vec2 operator / (const Vec2& vv, const float& k){
    return Vec2(vv.x() / k, vv.y() / k);
}

Vec3 operator * (const float& k, const Vec3& vvv){
    return Vec3(vvv.x() * k, vvv.y() * k, vvv.z() * k);
}
Vec3 operator * (const Vec3& vvv, const float& k){
    return Vec3(vvv.x() * k, vvv.y() * k, vvv.z() * k);
}
Vec3 operator / (const Vec3& vvv, const float& k){
    return Vec3(vvv.x() / k, vvv.y() / k, vvv.z() / k);
}

Vec4 operator * (const float& k, const Vec4& vvvv){
    return Vec4(vvvv.x() * k, vvvv.y() * k, vvvv.z() * k, vvvv.w() * k);
}
Vec4 operator * (const Vec4& vvvv, const float& k){
    return Vec4(vvvv.x() * k, vvvv.y() * k, vvvv.z() * k, vvvv.w() * k);
}
Vec4 operator / (const Vec4& vvvv, const float& k){
    return Vec4(vvvv.x() / k, vvvv.y() / k, vvvv.z() / k, vvvv.w() / k);
}


Vec2 operator * (const Vec2& a, const Vec2 & b){
    return Vec2(a.x() * b.x(), a.y() * b.y());
}

Vec3 operator * (const Vec3& a, const Vec3 & b){
    return Vec3(a.x() * b.x(), a.y() * b.y(), a.z() * b.z());
}
Vec4 operator * (const Vec4& a, const Vec4 & b){
    return Vec4(a.x() * b.x(), a.y() * b.y(), a.z() * b.z(), a.w() * b.w());
}

Vec4 cross(const Vec4& a, const Vec4& b){
    return Vec4(cross(a.xyz(), b.xyz()), 0);
}

float pow2(const float& x){
    return x * x;
}

float dis2(const Vec4& a, const Vec4& b){
    return pow2(a.x() - b.x()) + pow2(a.y() - b.y()) + pow2(a.z() - b.z());
}

Vec4 pow(const Vec4& v, const float& y){
    return Vec4(std::pow(v.x(), y),
                std::pow(v.y(), y),
                std::pow(v.z(), y),
                std::pow(v.w(), y));
}