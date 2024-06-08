//
// Created by Lingyin on 2024/4/6.
//
#include "Render.h"


void Render::addLight(const Light & light) { Lights.push_back(light); }
void Render::addThing(const Thing & thing) { Things.push_back(thing); }

template<class T >
static T getVec(const Texture<T > & v, const int &i, const int &j){
    return (min(i, j) >= 0) ? v.get(i, j) : 0;
}

void Render::rasterizating() {
    scene.clearScene();
    Information publicInfo;

   int nn = scene.n, mm = scene.m;
   publicInfo.shadowMap.resize(nn, mm);
   publicInfo.sumShadowMap.resize(nn, mm);
   publicInfo.sumShadowMap2.resize(nn, mm);
   publicInfo.block.resize(nn, mm);

    for (auto &light: Lights) {
        auto lightCamera = camera;
        lightCamera.width *= 3;
        lightCamera.height *= 3;

        Mat4 PmatOfLight;
        {
            float l = 1e9, r = -1e9;
            float d = 1e9, u = -1e9;
            for(auto& thing: Things){
                if(thing.floor){
                    continue;
                }

                const auto& M = thing.translate.getTranslateMat();
                const auto& V = light.translate.getInvTranslateMat();
                for(auto& vv : thing.mesh.vertexSpace){
                    const auto& v = V * M * Vec4(vv, 0.f);
                    l = min(l, v.x());
                    r = max(r, v.x());
                    d = min(d, v.y());
                    u = max(u, v.y());
                }

                assert(l < r - myEps && d < u - myEps);
            }

            PmatOfLight = getOrthoMat(l - 0.1, r + 0.1, d - 0.1, u+0.1, lightCamera.zNear, lightCamera.zFar);
//            PmatOfLight = getOrthoMat(l-3, r+3, d-3, u+3, lightCamera.zNear, lightCamera.zFar);
        }
        publicInfo.lightNormal = (light.translate.position - lightCamera.translate.aim).normal();
        // make shader Map
        for (auto &thing: Things) {
            if(thing.floor){
                continue;
            }

            const auto M = thing.translate.getTranslateMat();
            const auto V = light.translate.getInvTranslateMat();
//            const auto& P = getOrthoMat(lightCamera.width, lightCamera.height,
//                                        lightCamera.zNear, lightCamera.zFar);
//            const auto& P = getOrthoMat(-lightCamera.width / 2, lightCamera.width / 2,
//                                        -lightCamera.height / 2, lightCamera.height / 2,
//                                        lightCamera.zNear, lightCamera.zFar);
            const auto P = PmatOfLight;
            publicInfo.Mmat = M;
            publicInfo.Vmat = V;
            publicInfo.Pmat = P;
            publicInfo.MVP = P * V * M;

            publicInfo.camera = lightCamera;
            publicInfo.lightPosition = light.translate.position;
            publicInfo.lightColor = light.color;
            publicInfo.lightIntersity = light.intersity;

            scene.beginDraw(thing,
                             publicInfo,
                            nullptr, nullptr
            );

        }






        const auto& dd = scene.colorBuffer;

        for(int i = 0; i < nn; i ++){
            for(int j = 0; j < mm; j ++){
                auto & v = dd(i, j);
                v = Vec4(v[0], v[0], v[0], 0);
            }
        }

        auto& block = publicInfo.block;
        auto& shadowMap = publicInfo.shadowMap;
        auto& sumShadowMap = publicInfo.sumShadowMap;
        auto& sumShadowMap2 = publicInfo.sumShadowMap2;
        auto& zBuffer = scene.zBuffer;

        auto disZfar =  (double)std::abs(camera.zFar);
        auto disZnear = (double)std::abs(camera.zNear);
        for(int i = 0; i < nn * mm; i ++){
            block.textureMap[i] = (int)(shadowMap.textureMap[i] > myEps);
            sumShadowMap.textureMap[i] = shadowMap.textureMap[i];
            sumShadowMap2.textureMap[i] = 1.0 * shadowMap.textureMap[i] * shadowMap.textureMap[i];
        }

        for(int i = 0; i < nn; i ++){
            for(int j = 1; j < mm; j ++){
                block.textureMap[i * mm + j] += block.textureMap[i * mm + j - 1];
                sumShadowMap.textureMap[i * mm + j] += sumShadowMap.textureMap[i * mm + j - 1];
                sumShadowMap2.textureMap[i * mm + j] += sumShadowMap2.textureMap[i * mm + j - 1];
            }
        }

        for(int i = 1; i < nn; i ++){
            for(int j = 0; j < mm; j ++){
                block.textureMap[i * mm + j] += block.textureMap[i * mm + j - mm];
                sumShadowMap.textureMap[i * mm + j] += sumShadowMap.textureMap[i * mm + j - mm];
                sumShadowMap2.textureMap[i * mm + j] += sumShadowMap2.textureMap[i * mm + j - mm];
            }
        }


        if(0){ // show Shadow
            for(int i = 0; i < nn * mm; i ++){
                auto& c = shadowMap.textureMap[i];
                scene.colorBuffer.p[i] = Vec4(c, c, c, 0) * 10;
            }

            return ;
        }





        // make color
        scene.clearScene();
        for (auto &thing: Things) {
            const auto M = thing.translate.getTranslateMat();
            const auto V = camera.translate.getInvTranslateMat();
            const auto P = getProjectionMat(camera.width, camera.height, camera.zNear, camera.zFar);

            const auto VmatShadow = light.translate.getInvTranslateMat();
            const auto PmatShadow = PmatOfLight;
            const auto shadowMVP = PmatShadow * VmatShadow * M;

            publicInfo.shadowMVP = shadowMVP;
            publicInfo.Mmat = M;
            publicInfo.Vmat = V;
            publicInfo.Pmat = P;
            publicInfo.MVP = P * V * M;

            publicInfo.camera = camera;
            publicInfo.lightPosition = light.translate.position;
            publicInfo.lightColor = light.color;
            publicInfo.lightIntersity = light.intersity;

            scene.beginDraw(thing,
                            publicInfo,
                            vssmVertexShader,
                             vssmFragmentShader);
//                            vssmFragmentShader);
        }
    }
}


void Render::drawLine(){

    scene.clearScene();
    for(const auto& thing : Things) {
        const auto M = thing.translate.getTranslateMat();
        const auto V = camera.translate.getInvTranslateMat();
        const auto P = getProjectionMat(camera.width, camera.height, camera.zNear, camera.zFar);

        scene.drawLine(thing, M, V, P, camera.zNear);
    }
}
