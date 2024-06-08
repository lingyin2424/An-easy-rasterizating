//
// Created by Lingyin on 2024/4/7.
//
#include "Shaders.h"

void phongVertexShader(const Information & publicInfo, [[maybe_unused]]const Material& mtl, Vertex & vertex){
    vertex.screenV = (publicInfo.MVP * vertex.spaceV).one();
}

void phongFragmentShader(const Information & publicInfo, const Material& mtl, Vertex & vertex, Vec4 & fragmentColor){

    auto kd = mtl.Kd;
    if(mtl.havemap_Kd){
        kd = mtl.map_Kd.sample2D(vertex.uv);
    }

    const auto spaceV = (publicInfo.Mmat * vertex.spaceV).one();
    const auto normal = (publicInfo.Mmat * vertex.normal).normal();// publicInfo.Mmat * vertex.normal;
//    std :: cout << normal << ' ' << publicInfo.Mmat * vertex.normal << std :: endl;

    const auto l = (publicInfo.lightPosition - spaceV).normal();
    const auto e = (publicInfo.camera.translate.position - spaceV).normal();
    const auto h = (l + e).normal();

    const auto lightPosition = publicInfo.lightPosition;
//    const auto& lightColor = publicInfo.lightColor;
    const auto I = publicInfo.lightIntersity / 1000;

    const auto disToLight = (publicInfo.lightPosition - spaceV).mo();
    const auto disToEye = (publicInfo.camera.translate.position - spaceV).mo();


    float tmp = std::max((float)0, dot(l, normal));

    const int T = 16;
    tmp = std::ceil(tmp * T) / T;

    const Vec4 La = mtl.Ka * (I * 0.0002f);
    const Vec4 Ld = I * tmp * kd / disToLight / disToLight;
    const Vec4 Ls = 3e-3 * I * kd * (float)std::pow(std::max(0.f, dot(h, normal)), 32);
    fragmentColor = La + Ld;// + Ls;
}
