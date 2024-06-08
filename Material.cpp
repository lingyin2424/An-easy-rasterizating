//
// Created by Lingyin on 2024/4/6.
//

#include "Material.h"

Vertex::Vertex()= default;
Vertex::Vertex(const Vec4& spaceV, const Vec4& normal, const Vec2& uv){
    this -> spaceV = spaceV;
    this -> normal = normal;
    this -> uv = uv;
}

Vertex::Vertex(const Vec4& spaceV, const Vec4& normal, const Vec2& uv, const Vec4& screenV){
    this -> spaceV = spaceV;
    this -> normal = normal;
    this -> uv = uv;
    this -> screenV = screenV;
}

Material::Material(){
    havemap_Kd = havemap_Ka = havemap_Ks = havemap_bump = false;
    vertexFunctionPointerId = 0;
    fragmentFunctionPointerId = 0;
    Ka = Vec4(1, 1, 1, 0);
    Kd = Vec4(1, 1, 1, 0);
    Ks = Vec4(1, 1, 1, 0);
    Ke = Vec4(0, 0, 0, 0);
    illum = 1;
}

void Material::loadMTL(const string& path, const string& name){

}

Vertex insterVertex(const Vertex& a, const Vertex & b, const float& aa, const float& bb){
    Vertex ret;
    ret.spaceV = (a.spaceV * aa + b.spaceV * bb);
    ret.screenV = (a.screenV * aa, b.screenV * bb);
    ret.uv = (a.uv * aa + b.uv * bb);
    ret.normal = (a.normal * (aa / a.normal.mo()) + b.normal * (bb / b.normal.mo())).normal();
    return ret;
}

Vertex insterVertex(const Vertex& a, const Vertex & b, const Vec2& vv){
    return insterVertex(a, b, vv.x(), vv.y());
}

Vertex insterVertex(const Vertex& a, const Vertex & b, const Vertex& c, const Vec3& vvv){
    float aa = vvv.x(), bb = vvv.y(), cc = vvv.z();
    Vertex ret;
    ret.spaceV = (a.spaceV * aa + b.spaceV * bb + c.spaceV * cc);
    ret.screenV = (a.screenV * aa + b.screenV * bb + c.screenV * cc);
    ret.uv = (a.uv * aa + b.uv * bb + c.uv * cc);
    ret.normal = (
            a.normal * (aa / a.normal.mo()) +
            b.normal * (bb / b.normal.mo()) +
            c.normal * (cc / c.normal.mo())
            ).normal();
    return ret;
}