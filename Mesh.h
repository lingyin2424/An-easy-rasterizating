//
// Created by Lingyin on 2024/4/6.
//

#ifndef ASMALLRENDER_MESH_H
#define ASMALLRENDER_MESH_H

#include "Face.h"
#include <vector>
#include "math/Vec.h"
#include <cstring>
#include <array>
#include "fstream"
#include "iostream"
#include <sstream>
#include <cassert>

using std::vector;
using std::string;

class Mesh{
public:
    vector<Vec3 > vertexSpace, vertexNormal;
    vector<Vec2 > vertexTexture;
    vector<Face > faces; // (v, n, t)

    void loadOBJ(const string& path, const string& name);
};

#endif //ASMALLRENDER_MESH_H
