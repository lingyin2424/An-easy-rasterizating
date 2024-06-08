//
// Created by Lingyin on 2024/4/6.
//

#ifndef ASMALLRENDER_RENDER_H
#define ASMALLRENDER_RENDER_H

#include<vector>
#include"Thing.h"
#include "Material.h"
#include"Camera.h"
#include "Mesh.h"
#include<map>
#include<array>
#include "Scene.h"
#include "Shader/Shaders.h"
using std::vector;

class Render{

public:



    Scene scene;
    vector<Light > Lights;
    vector<Thing > Things;
    Camera camera;

    void addLight(const Light & light);
    void addThing(const Thing & thing);

    void rasterizating();
    Render() = default;

    void drawLine();
};


#endif //ASMALLRENDER_RENDER_H
