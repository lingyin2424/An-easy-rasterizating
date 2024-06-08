//
// Created by Lingyin on 2024/4/6.
//

#include "Shaders.h"

void defaultVertexShader(const Information & publicInfo, [[maybe_unused]] const Material& mtl, Vertex & vertex){
    auto& MVP = publicInfo.MVP;
    vertex.screenV = (MVP * vertex.spaceV).one();
}

void defaultFragmentShader(const Information & publicInfo, [[maybe_unused]] const Material& mtl, Vertex & vertex, Vec4 & fragmentColor){
    auto& lightPosition = publicInfo.lightPosition;
    auto& lightColor = publicInfo.lightColor;
    auto& I = publicInfo.lightIntersity;

    auto l = (lightPosition.one() - vertex.spaceV.one()).normal();
    auto r = (lightPosition - vertex.spaceV).mo();
    fragmentColor = std::max((float)0, dot(l, vertex.normal.normal())) * I / r / r * lightColor + Vec4(40, 40, 40, 0);
}
