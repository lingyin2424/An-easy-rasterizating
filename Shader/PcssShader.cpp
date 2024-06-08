//
// Created by Lingyin on 2024/4/11.
//

#include <cmath>

#include "Shaders.h"



static float TMP_EPS = 0.03f;
void pcssVertexShader(const Information & publicInfo, [[maybe_unused]]const Material& mtl, Vertex & vertex){
    vertex.screenV = (publicInfo.MVP * vertex.spaceV).one();
}

using std::pow;
using std::sin;
using std::cos;
using std::floor;


static const int NUM_SAMPLES = 50;
static const int NUM_RINGS = 10;
static const float lightLength = 0.15;

static Vec2 poissonDisk[NUM_SAMPLES];

static float mod(const float &a, const float& b){
    return a - b * floor(a / b);
}

static float fract(const float & v){
    return v - floor(v);
}

static float rand_2to1(const Vec2& uv){
    // 0 - 1
    static const float a = 12.9898, b = 78.233, c = 43758.5453;
    float dt = dot(uv, Vec2(a,b)), sn = mod(dt, myPi);
    return fract(sin(sn) * c);
}

static void poissonDiskSamples(const Vec2& randomSeed){

    float ANGLE_STEP = 2 * myPi * float(NUM_RINGS) / float(NUM_SAMPLES);
    float INV_NUM_SAMPLES = 1.0 / float(NUM_SAMPLES);

    float angle = rand_2to1(randomSeed) * 2 * myPi;
    float radius = INV_NUM_SAMPLES;
    float radiusStep = radius;

    for(auto & i : poissonDisk) {
        i = Vec2(cos(angle), sin(angle)) * pow(radius, 0.75f);
        radius += radiusStep;
        angle += ANGLE_STEP;
    }
}

static float pcf(const Texture<double >& shadowMap, const Vec2& shadowSpace, float dd, float r){

    float visible = 0;
    poissonDiskSamples(shadowSpace);

    for(auto i : poissonDisk){
        auto tmp = shadowSpace + i * r;
        auto disToLight = shadowMap.sample2D(tmp.x(), tmp.y(), 0);
        if(disToLight < myEps || dd <= disToLight + TMP_EPS){
            visible ++;
        }
    }
    visible /= NUM_SAMPLES;

    return visible;

}

static float findBlock(const Texture<double >& shadowMap, const Vec2& shadowSpace, float dd, float zNear){

    const float r = lightLength * (dd + zNear) / (dd * 2);
    poissonDiskSamples(shadowSpace);

    float avgBlock = 0;
    int numBlock = 0;
    for(auto i : poissonDisk){
        auto tmp = shadowSpace + i * r;
        const float sampleDis = shadowMap.sample2D(tmp.x(), tmp.y(), 0);
        if(dd > sampleDis + myEps && sampleDis > TMP_EPS){
            numBlock ++;
            avgBlock += sampleDis;
        }
    }

    if(numBlock == 0){
        return -1;
    }
    else{
        return avgBlock / (float)numBlock;
    }
}

static float pcss(const Texture<double >& shadowMap, const Vec2& shadowSpace, float dd, float zNear){
    if(!(0 <= min(shadowSpace.x(), shadowSpace.y()) && max(shadowSpace.x(), shadowSpace.y()) <= 1)){
        return 1.f;
    }

    // find avgBlock
    const float avgBlock = findBlock(shadowMap, shadowSpace, dd, zNear);
    if(avgBlock == -1){
        return 1;
    }

    // cal sample range
    const float r = lightLength * (dd - avgBlock) / avgBlock;

    // filter
    return pcf(shadowMap, shadowSpace, dd, r);
}


void pcssFragmentShader(const Information & publicInfo, const Material& mtl, Vertex & vertex, Vec4 & fragmentColor){

    const auto shadowMVP = publicInfo.shadowMVP;
    const auto vInShadowSpace = (shadowMVP * vertex.spaceV).one();
    auto spaceV = (publicInfo.Mmat * vertex.spaceV).one();

    float dd = (publicInfo.lightPosition - spaceV).mo();

    const Vec2 shadowSpace(vInShadowSpace.y() * 0.5f + 0.5f, vInShadowSpace.x() * 0.5f + 0.5f);
    const auto visible = pcss(publicInfo.shadowMap, shadowSpace, dd, publicInfo.camera.zNear);

    pbrFragmentShader(publicInfo, mtl, vertex, fragmentColor);
    fragmentColor *= (0.8f * visible + 0.2f);
}
