//
// Created by Lingyin on 2024/4/11.
//

#include "Shaders.h"

void shadowPbrVertexShader(const Information & publicInfo, [[maybe_unused]]const Material& mtl, Vertex & vertex){
    vertex.screenV = (publicInfo.MVP * vertex.spaceV).one();
}

void shadowPbrFragmentShader(const Information & publicInfo, const Material& mtl, Vertex & vertex, Vec4 & fragmentColor){

    const auto& shadowMVP = publicInfo.shadowMVP;
    const auto vInShadowSpace = (shadowMVP * vertex.spaceV).one();
    auto spaceV = (publicInfo.Mmat * vertex.spaceV).one();

    float dd = (publicInfo.lightPosition - spaceV).mo();

    auto dim1 = vInShadowSpace.y() * 0.5f + 0.5f;
    auto dim2 = vInShadowSpace.x() * 0.5f + 0.5f;


//    static auto E = Vec4(1, 1, 1, 0);
//    fragmentColor = Vec4(1, 1, 1, 0) * dim1 * 255;
//    fragmentColor = Vec4(1, 1, 1, 0) * dd * 10;


    float k = 1;
    if(0 <= min(dim1, dim2) && max(dim1, dim2) <= 1){
        const float disToLight2 = publicInfo.shadowMap.sample2D(dim1, dim2);
//        std::cout << dd << ' ' << disToLight2 << ' ' << std::endl;
        if(disToLight2 > myEps && disToLight2 + myEps < dd){
//            std::cout << ' ' << disToLight2 << ' ' << dd << '\n';
//            fragmentColor = Vec4(1, 1, 1, 0) * disToLight2 * 10;
 //           return ;
            k = 0.2;
        }
 //       fragmentColor = E * disToLight2 * 10;
    }




//

//    const float uu = vInShadowSpace.y() * 0.5f + 0.5f;
//    const float vv = vInShadowSpace.x() * 0.5f + 0.5f;



//    std::cout << dd << ' ' << disToLight2 << std::endl;

    pbrFragmentShader(publicInfo, mtl, vertex, fragmentColor);
    fragmentColor *= k;
}
