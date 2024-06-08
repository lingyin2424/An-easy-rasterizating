//
// Created by Lingyin on 2024/4/6.
//

#ifndef ASMALLRENDER_SCENE_H
#define ASMALLRENDER_SCENE_H

#include<vector>
#include"Thing.h"
#include "Material.h"
#include"Camera.h"
#include "Mesh.h"
#include<map>
#include<array>
#include "Shader/Shaders.h"
#include <algorithm>
#include <random>
#include <thread>
#include "Array2D.h"
using std::vector;

struct Scene{

public:
    int n = 0, m = 0;
    static const int usingCameraParameter = 0;
public:

    Array2D<float > zBuffer;
    Array2D<Vec4 > colorBuffer;

    //    float* zBuffer{};
//    Vec4* colorBuffer{};
//    vector<float > zBuffer;
//    vector<Vec4 > colorBuffer;

    //    float ** zBuffer;
//    Vec4 ** colorBuffer;
//    std::mutex ** mutexOfBuffer;

    Scene() = default;

private:
    template<class T>
    void resize(T* &obj, int nn, int mm);
    static void cutAndMakeTriangle(const Face & face,
                            const vector<float > &zAfterMV,
                            vector<Triangle >& triangles, const Mesh& mesh, const float& zN);

    template<class T >
    static T inster(const T& a, const T & b, const T& c, const Vec3& BC);
public:
    void setSize(int nn, int mm);

public:

    void clearMutex();

    [[nodiscard]] inline float& getZ(const int& x, const int& y) const;
    [[nodiscard]] inline Vec4& getColor(const int& x, const int& y) const;


    template<class T>
    T& get(vector<T >& vec, int x, int y) const;

public:
    void clearScene();

private:
    static bool in(float x, float y, const Vec4 & a, const Vec4 & b, const Vec4 & c);
    static Vec3 barycenteicCoordinates(
            float x, float y, const Vec4 & a, const Vec4 & b, const Vec4 & c);
//    Vec4 barycenteicCoordinates(
//            const Vec4 & D, const Vec4 & a, const Vec4 & b, const Vec4 & c);

    static float mixProduct(const Vec4 & a, const Vec4 &b, const Vec4 & c);
public:
    void beginDraw(const Thing & thing,
                    Information& publicInfo,
                    VertexFunctionPointer vertexFunction = nullptr,
                    FragmentFunctionPointer fragmentFunction = nullptr,
                    float width = usingCameraParameter,
                    float height = usingCameraParameter,
                    float nZ = usingCameraParameter,
                    float fZ = usingCameraParameter);

    static void drawOnFragment(const vector<Triangle > & triangles,
                               Information& publicInfo,
                               const Thing& thing, FragmentFunctionPointer
                               fragmentFunction,
                               Scene* scene);

    ~Scene();
private:
    void drawLine(const Vec4& a, const Vec4& b);
public:
    void drawLine(const Thing& thing,
                  const Mat4& Mmat, const Mat4& Vmat, const Mat4 & Pmat,
                  float nZ);

};



#endif //ASMALLRENDER_SCENE_H
