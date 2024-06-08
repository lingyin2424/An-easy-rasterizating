//
// Created by Lingyin on 2024/4/9.
//

#include "Shaders.h"

Vec4 fresnelTerm(const float& cosTheta, const Vec4& F0){
    return F0 + (Vec4(1.f, 1.f, 1.f, 0) - F0) * (float)pow(1 - cosTheta, 5);
}

float G_s_GGX(const float& cosTheta, const float& arpha){ // here is GXX / cosTheta
    auto k = pow2(arpha + 1) * (0.125f);
    auto ret = /*cosTheta*/ 1 / (cosTheta * (1 - k) + k);
    return ret;
}

float D_TR(const float& cosTheta_h, const float& arpha){
    auto dn = myPi * pow2(cosTheta_h * cosTheta_h * (arpha * arpha - 1) + 1);
    return (arpha * arpha) / dn;
}

void pbrVertexShader(const Information & publicInfo, [[maybe_unused]] const Material& mtl, Vertex & vertex){
    auto& MVP = publicInfo.MVP;
    vertex.screenV = (MVP * vertex.spaceV).one();
}

void pbrFragmentShader(const Information & publicInfo, const Material& mtl, Vertex & vertex, Vec4 & fragmentColor){

//    fragmentColor = Vec4(244, 244, 244, 0);
//    return ;
    static const float Metalic = 0.3f;
    static const auto defaultF0 = Vec4(0.04f, 0.04f, 0.04f, 0);
    static const float alpha = 0.8;

    auto spaceV = (publicInfo.Mmat * vertex.spaceV).one();
    auto lightColor = publicInfo.lightColor;
    auto I = publicInfo.lightIntersity;
    auto n = (publicInfo.Mmat * vertex.normal).normal();
    auto l = (publicInfo.lightPosition - spaceV).normal();
    auto r = (publicInfo.lightPosition - spaceV).mo();
    auto v = (publicInfo.camera.translate.position - vertex.spaceV).normal();
    auto h = (v + l).normal();

    auto invr = 1.f / r;
    auto ins = I * invr * invr * myPi;
    auto h_dot_v = std::max((float)0, dot(h, v));
    auto n_dot_v = std::max((float)0, dot(n, v));
    auto n_dot_l = std::max((float)0, dot(n, l));
    auto h_dot_n = std::max((float)0, dot(h, n));

    auto albedo = mtl.Kd;
    if(mtl.havemap_Kd){
        albedo = mtl.map_Kd.sample2D(vertex.uv);
    }
    albedo = pow(albedo, 2.2f);

    auto F0 = albedo * Metalic + defaultF0 * (1 - Metalic);
    auto F = fresnelTerm(h_dot_v, F0);
    auto k_diff = (Vec4(1.f, 1.f, 1.f, 0) - F) * (1 - Metalic);
    auto G = G_s_GGX(n_dot_l, alpha) * G_s_GGX(n_dot_v, alpha);
    auto D = D_TR(h_dot_n, alpha);
    auto f_BRDF = (k_diff * albedo * invMyPi) + D * G * F * 0.25f;

    fragmentColor = pow(f_BRDF * n_dot_l * ins * lightColor, 1.f / 2.2f);
}
