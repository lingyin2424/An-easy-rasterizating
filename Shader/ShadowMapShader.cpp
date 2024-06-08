//
// Created by Lingyin on 2024/4/11.
//

#include <cmath>

#include "Shaders.h"

void shadowMapVertexShader(const Information & publicInfo, [[maybe_unused]]const Material& mtl, Vertex & vertex){
    vertex.screenV = (publicInfo.MVP * vertex.spaceV).one();
}

void shadowMapFragmentShader(const Information & publicInfo, [[maybe_unused]]const Material& mtl, Vertex & vertex, Vec4 & fragmentColor){

    const float& dd = dis2(publicInfo.lightPosition,  publicInfo.Mmat * vertex.spaceV);

    fragmentColor[0] = fragmentColor[1] = std::sqrt(dd);
    fragmentColor[2] = dd;
}
