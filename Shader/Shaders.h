//
// Created by Lingyin on 2024/4/6.
//

#ifndef ASMALLRENDER_SHADERS_H
#define ASMALLRENDER_SHADERS_H

#include "../Material.h"
#include "../Face.h"
#include <map>

class Material;
class Information;
class Vertex;

using VertexFunctionPointer
        = void (*)(const Information &, const Material&, Vertex &);
using FragmentFunctionPointer
        = void (*)(const Information &, const Material&, Vertex &, Vec4 & fragmentColor);



void defaultVertexShader(const Information & publicInfo, const Material& mtl, Vertex & vertex);
void defaultFragmentShader(const Information & publicInfo, const Material& mtl, Vertex & vertex, Vec4 & fragmentColor);

void phongVertexShader(const Information & publicInfo, const Material& mtl, Vertex & vertex);
void phongFragmentShader(const Information & publicInfo, const Material& mtl, Vertex & vertex, Vec4 & fragmentColor);

void pbrVertexShader(const Information & publicInfo, const Material& mtl, Vertex & vertex);
void pbrFragmentShader(const Information & publicInfo, const Material& mtl, Vertex & vertex, Vec4 & fragmentColor);

void shadowMapVertexShader(const Information & publicInfo, const Material& mtl, Vertex & vertex);
void shadowMapFragmentShader(const Information & publicInfo, const Material& mtl, Vertex & vertex, Vec4 & fragmentColor);

void shadowPbrVertexShader(const Information & publicInfo, const Material& mtl, Vertex & vertex);
void shadowPbrFragmentShader(const Information & publicInfo, const Material& mtl, Vertex & vertex, Vec4 & fragmentColor);

void pcssVertexShader(const Information & publicInfo, const Material& mtl, Vertex & vertex);
void pcssFragmentShader(const Information & publicInfo, const Material& mtl, Vertex & vertex, Vec4 & fragmentColor);

void pcfVertexShader(const Information & publicInfo, const Material& mtl, Vertex & vertex);
void pcfFragmentShader(const Information & publicInfo, const Material& mtl, Vertex & vertex, Vec4 & fragmentColor);

void vssmVertexShader(const Information & publicInfo, const Material& mtl, Vertex & vertex);
void vssmFragmentShader(const Information & publicInfo, const Material& mtl, Vertex & vertex, Vec4 & fragmentColor);

void vsmVertexShader(const Information & publicInfo, const Material& mtl, Vertex & vertex);
void vsmFragmentShader(const Information & publicInfo, const Material& mtl, Vertex & vertex, Vec4 & fragmentColor);

vector<Triangle > loopSubdivision(const vector<Triangle >& triangles);

/*
 void defaultVertexShader(Information & publicInfo, Vertex & vertex){
    vertex.spaceV = publicInfo.mat4Map["MVP"] * vertex.spaceV;
}

void defaultFragmentShader(Information & publicInfo, Vertex & vertex, Vec4 & fragmentColor){
    fragmentColor = Vec4(255, 255, 255, 0);
}

 */
#endif //ASMALLRENDER_SHADERS_H
