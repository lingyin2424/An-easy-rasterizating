//
// Created by Lingyin on 2024/4/6.
//

#ifndef ASMALLRENDER_MATERIAL_H
#define ASMALLRENDER_MATERIAL_H
#include "math/Vec.h"
#include "math/Mat.h"
#include "unordered_map"
#include "Camera.h"
#include "Texture.h"
using std::unordered_map;
using std::string;

class Information{
public:
    unordered_map<string, float > floatMap;
    unordered_map<string, Vec2 > vec2Map;
    unordered_map<string, Vec3 > vec3Map;
    unordered_map<string, Vec4 > vec4Map;
    unordered_map<string, Mat4 > mat4Map;

    Vec4 lightNormal;
    Camera camera;
    Vec4 lightPosition, lightColor;
    float lightIntersity;
    Mat4 Mmat, Vmat, Pmat, MVP, shadowMVP;
    Texture<Vec4 > texture;
    Texture<double > shadowMap;
    Texture<double > sumShadowMap;
    Texture<double > sumShadowMap2;
    Texture<int > block;
};

class Vertex{
public:
    Vertex();
    Vertex(const Vec4& spaceV, const Vec4& normal, const Vec2& uv);
    Vertex(const Vec4& spaceV, const Vec4& normal, const Vec2& uv, const Vec4& screenV);
    Vec4 spaceV, screenV, normal;
    Vec2 uv;
};

Vertex insterVertex(const Vertex& a, const Vertex & b, const float& aa, const float& bb);
Vertex insterVertex(const Vertex& a, const Vertex & b, const Vec2& vv);
Vertex insterVertex(const Vertex& a, const Vertex & b, const Vertex& c, const Vec3& vvv);



class Material{
public:

    Material();
    int vertexFunctionPointerId;
    int fragmentFunctionPointerId;

    float Ns, Ni, d;
    Vec4 Ka, Kd, Ks, Ke;
    int illum;

    bool havemap_Kd, havemap_Ka, havemap_Ks, havemap_bump;
    Texture<Vec4 > map_Kd, map_Ka, map_Ks, map_bump;

    void loadMTL(const string& path, const string& name);
};

#endif //ASMALLRENDER_MATERIAL_H
