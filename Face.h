//
// Created by Lingyin on 2024/4/7.
//

#ifndef ASMALLRENDER_FACE_H
#define ASMALLRENDER_FACE_H

#include <array>
#include "math/Vec.h"
#include "math/Mat.h"
#include "vector"
#include "Material.h"

using std::vector;
using std::array;
using std::string;

class Face{
public:
    vector<array<int, 3> > points;
    int materialId = 0;
    string materialName;
};

class Triangle{
public:
    array<Vertex, 3 > points;
    int materialId = 0;
    float disToCamera;
};

#endif //ASMALLRENDER_FACE_H
