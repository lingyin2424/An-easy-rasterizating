//
// Created by Lingyin on 2024/4/14.
//

#include <cmath>

#include "Shaders.h"

void pcfVertexShader(const Information & publicInfo, [[maybe_unused]] const Material& mtl, Vertex & vertex){
    vertex.screenV = (publicInfo.MVP * vertex.spaceV).one();
}

static const int NUM_SAMPLES = 500;
static const int NUM_RINGS = 10;


static Vec2 poissonDisk[NUM_SAMPLES];

static float mod(const float &a, const float& b){
    return a - b * std::floor(a / b);
}

static float fract(const float & v){
    return v - std::floor(v);
}

static float rand_2to1(const Vec2& uv){
    // 0 - 1
    static const float a = 12.9898, b = 78.233, c = 43758.5453;
    float dt = dot(uv, Vec2(a,b)), sn = mod(dt, myPi);
    return fract(std::sin(sn) * c);
}

static void poissonDiskSamples(const Vec2& randomSeed){

    float ANGLE_STEP = 2 * myPi * float(NUM_RINGS) / float(NUM_SAMPLES);
    float INV_NUM_SAMPLES = 1.0 / float(NUM_SAMPLES);

    float angle = rand_2to1(randomSeed) * 2 * myPi;
    float radius = INV_NUM_SAMPLES;
    float radiusStep = radius;

    for(auto & i : poissonDisk) {
        i = Vec2(std::cos(angle), std::sin(angle)) * std::pow(radius, 0.75f);
        radius += radiusStep;
        angle += ANGLE_STEP;
    }
}

static float pcf(const Texture<double >& shadowMap, const Vec2& shadowSpace, float dd, float r){

    if(0 <= min(shadowSpace.x(), shadowSpace.y()) && max(shadowSpace.x(), shadowSpace.y()) <= 1){
        float visible = 0;
        poissonDiskSamples(shadowSpace);

        for(auto i : poissonDisk){
            auto tmp = shadowSpace + i * r;
            auto disToLight = shadowMap.sample2D(tmp.x(), tmp.y(), 1);
            if(disToLight < myEps || dd <= disToLight + myEps){
                visible ++;
            }
        }
        visible /= NUM_SAMPLES;

        return visible;
    }
    else{
        return 1.f;
    }
}

void pcfFragmentShader(const Information & publicInfo, const Material& mtl, Vertex & vertex, Vec4 & fragmentColor){
    const auto shadowMVP = publicInfo.shadowMVP;
    const auto vInShadowSpace = (shadowMVP * vertex.spaceV).one();
    auto spaceV = (publicInfo.Mmat * vertex.spaceV).one();

    float dd = (publicInfo.lightPosition - spaceV).mo();

    const Vec2 shadowSpace(vInShadowSpace.y() * 0.5f + 0.5f, vInShadowSpace.x() * 0.5f + 0.5f);
    float visible = pcf(publicInfo.shadowMap, shadowSpace, dd, 0.01);


    pbrFragmentShader(publicInfo, mtl, vertex, fragmentColor);
    fragmentColor *= (0.8f * visible + 0.2f);
}
