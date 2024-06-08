//
// Created by Lingyin on 2024/4/9.
//

#include "Shaders.h"



template<class T >
static T pp(const T& x){
    return x * x;
}

static int inRange(const int l, const int r, const int x){
    return max(l, min(r, x));
}

template<class T >
static T sample(const int &x, const int &y, const Texture<T >& texture){
    return (x != -1 && y != -1) ? texture.sampleInt(x, y) : 0;
}

template<class T >
static T sampleSum(const int& u1, const int& u2, const int& v1, const int& v2, const Texture<T >& texture){

    auto ret =
            sample(u2, v2, texture)
            + sample(u1, v1, texture)
            - sample(u1, v2, texture)
            - sample(u2, v1, texture);
/*
    if(ret < 0){
        ret += M;
    }
*/
    return ret;
}

static Vec4 fresnelTerm(const float& cosTheta, const Vec4& F0){
    return F0 + (Vec4(1.f, 1.f, 1.f, 0) - F0) * (float)pow(1 - cosTheta, 5);
}

static float G_s_GGX(const float& cosTheta, const float& arpha){ // here is GXX * cosTheta
    auto k = pow2(arpha + 1) * (0.125f);
    auto ret = /*cosTheta*/ 1 / (cosTheta * (1 - k) + k);
    return ret;
}

static float D_TR(const float& cosTheta_h, const float& arpha){
    auto dn = myPi * pow2(cosTheta_h * cosTheta_h * (arpha * arpha - 1) + 1);
    return (arpha * arpha) / dn;
}

void vssmVertexShader(const Information & publicInfo, [[maybe_unused]] const Material& mtl, Vertex & vertex){
    auto& MVP = publicInfo.MVP;
    vertex.screenV = (MVP * vertex.spaceV).one();
}

/*
__device__ static float max(float a, float b){
    return a > b ? a : b;
}

*/

#define CC std::abs(dim1 - 0.122904) + std::abs(dim2 - 0.557771) < myEps

static const float lightLength = 0.15;
static float findBlock(const Information& publicInfo, float dim1, float dim2, float dd, float zNear){

    if(0 <= min(dim1, dim2) && max(dim1, dim2) <= 1){

        const float r = lightLength * (dd + zNear) / (dd * 2);
        const int& n = publicInfo.shadowMap.n;
        const int& m = publicInfo.shadowMap.m;

        int u1 = inRange(-1, n - 1, (int)((dim1 - r) * (float)n));
        int u2 = inRange(-1, n - 1, (int)((dim1 + r) * (float)n));
        int v1 = inRange(-1, m - 1, (int)((dim2 - r) * (float)m));
        int v2 = inRange(-1, m - 1, (int)((dim2 + r) * (float)m));

        const int blockNum = sampleSum(u1, u2, v1, v2, publicInfo.block);
        if(blockNum == 0){
            return -1;
        }
        else{
            const double mu = sampleSum(u1, u2, v1, v2, publicInfo.sumShadowMap) / blockNum;
            const double mu2 = mu * mu;
            const double sigma2 = sampleSum(u1, u2, v1, v2, publicInfo.sumShadowMap2) / blockNum - mu2;
            const double chebychev = sigma2 / (sigma2 + pp(dd - mu));
            double avgBlock = (mu - chebychev * dd) / (1 - chebychev);

            float ret = avgBlock > myEps ? avgBlock : -1;
            float t1 = (dd < mu || chebychev + myEps > 1);
            ret = (-t1) + (1.f - t1) * ret;
            float t2 = (sigma2 <= myEps * myEps || chebychev < myEps);
            ret = (mu + myEps < dd ? mu : -1) * t2 + (1 - t2) * ret;
            return ret;
        }
    }

    return -1;
}

static float vssm(const Information& publicInfo, float dim1, float dim2, float dd, float zNear){

    const auto& shadowMap = publicInfo.shadowMap;

    // find avgBlock
    const float avgBlock = findBlock(publicInfo, dim1, dim2, dd, zNear);
    if(avgBlock <= myEps){
        return 1;
    }
//    return avgBlock * 100;

//    return avgBlock;

    // cal sample range
    const double r = lightLength * (dd - avgBlock) / avgBlock;

//    return r;
    float visible = 1.f;
    const int& n = publicInfo.shadowMap.n;
    const int& m = publicInfo.shadowMap.m;
    // r is very small to do
    int u1 = inRange(-1, n - 1, (dim1 - r) * n);
    int u2 = inRange(-1, n - 1, (dim1 + r) * n);
    int v1 = inRange(-1, m - 1, (dim2 - r) * n);
    int v2 = inRange(-1, m - 1, (dim2 + r) * n);

    if((u2 - u1) * (v2 - v1) > 4){
        auto uuvv = (u2 - u1) * (v2 - v1);
        const int blockNum = sampleSum(u1, u2, v1, v2, publicInfo.block);
        if(blockNum == 0){
            visible = 1;
        }
        else{
            const double mu = sampleSum(u1, u2, v1, v2, publicInfo.sumShadowMap) / blockNum;
            const double mu2 = mu * mu;
            const double sigma2 = sampleSum(u1, u2, v1, v2, publicInfo.sumShadowMap2) / blockNum - mu2;

            if(sigma2 <= myEps * myEps){
                return 1 - (mu + myEps <= dd) * blockNum / uuvv;
            }
            if(dd + myEps < mu){
                return 1.f;
            }
            const double chebychev = ((uuvv - blockNum) + sigma2 / (sigma2 + pp(max(dd - mu, 0.0))) * blockNum) / uuvv;
            visible = chebychev;
        }
    }
    return visible;
}


void vssmFragmentShader(const Information & publicInfo, const Material& mtl, Vertex & vertex, Vec4 & fragmentColor){

    const auto& shadowMVP = publicInfo.shadowMVP;
    const auto vInShadowSpace = (shadowMVP * vertex.spaceV).one();
    float shadowSreenX = vInShadowSpace.x() * 0.5f + 0.5f;
    float shadowSreenY = vInShadowSpace.y() * 0.5f + 0.5f;
    auto spaceV = publicInfo.Mmat * vertex.spaceV;

    float dd = (publicInfo.lightPosition - spaceV).mo();


    float visible = vssm(publicInfo, shadowSreenY, shadowSreenX, dd, publicInfo.camera.zNear);

    float Metalic = 0.3f;
    auto defaultF0 = Vec4(0.04f, 0.04f, 0.04f, 0);
    float alpha = 0.8;

    auto& lightColor = publicInfo.lightColor;
    auto& I = publicInfo.lightIntersity;
    auto& n = vertex.normal;
    auto l = (publicInfo.lightPosition - spaceV).normal();
    auto r = (publicInfo.lightPosition - spaceV).mo();
    auto v = (publicInfo.camera.translate.position - spaceV).normal();
    auto h = (v + l).normal();

    auto invr = 1.f / r;
    auto ins = I * invr * invr * myPi;
    auto h_dot_v = ((float)0 > dot(h, v) ? 0 : dot(h, v));
    auto n_dot_v = ((float)0 > dot(n, v) ? 0 : dot(n, v));
    auto n_dot_l = ((float)0 > dot(n, l) ? 0 : dot(n, l));
    auto h_dot_n = ((float)0 > dot(h, n) ? 0 : dot(h, n));

    auto albedo = mtl.havemap_Kd ? mtl.map_Kd.sample2D(vertex.uv) : mtl.Kd;
    albedo = pow(albedo, 2.2f);

    auto F0 = albedo * Metalic + defaultF0 * (1 - Metalic);
    auto F = fresnelTerm(h_dot_v, F0);
    auto k_diff = (Vec4(1.f, 1.f, 1.f, 0) - F) * (1 - Metalic);
    auto G = G_s_GGX(n_dot_l, alpha) * G_s_GGX(n_dot_v, alpha);
    auto D = D_TR(h_dot_n, alpha);
    auto f_BRDF = (k_diff * albedo * invMyPi) + D * G * F * 0.25f;

    fragmentColor = pow((f_BRDF * n_dot_l * ins * lightColor), 1.f / 2.2f) * (0.2f + 0.8 * visible);
}
