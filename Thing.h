//
// Created by Lingyin on 2024/4/6.
//

#ifndef ASMALLRENDER_THING_H
#define ASMALLRENDER_THING_H

#include "Mesh.h"
#include "Material.h"
#include "Translate.h"
#include "map"
#include <iostream>
#include "tgaimage.h"
#include <cstdlib>
#include "Texture.h"

class Thing{
public:
    Mesh mesh;
    vector<Material > materials;
    Translate translate;
    array<Vec4, 8 > aabb;
    bool floor = false;

    void load(const string& path, const string& name);
};


class Light : public Thing{
public:
    float intersity;
    Vec4 color;
    bool direct;
};
#endif //ASMALLRENDER_RENDER_H