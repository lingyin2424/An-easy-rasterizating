//
// Created by Lingyin on 2024/4/6.
//

#ifndef ASMALLRENDER_TEXTURE_H
#define ASMALLRENDER_TEXTURE_H

#include "math/Vec.h"
#include "math/Mat.h"
#include<vector>

using std::max;
using std::min;

using std::vector;
template<class T >
struct Texture{
private:
//    T* operator[] (const int& i) const ;
public:
    int n, m;

    vector<T > textureMap;

    explicit Texture(const vector<vector<T > > &M);
    Texture() { n = m = 0; };
    Texture(const Texture& texture);
    ~Texture();
    void resize(int n, int m);


public:
    T &get(const int& i, const int& j);
    const T& get(const int& i, const int& j)const;

    const T& sampleInt(const int & u, const int & v) const;
    T interpolation(const T & a, const T & b, const float & v) const;

public:
    T sample2D(float u, float v, int type = 0) const;
    T sample2D(const Vec2& uv) const;
    T sampleAverage(const Vec2& uv, const float& r) const; // [0, 1]
};

template<class T >
Texture<T >::Texture(const Texture& texture) {

    this -> n = texture.n;
    this -> m = texture.m;
    textureMap.resize(n * m);
    for(int i = 0; i < n * m; i ++){
        textureMap[i] = texture.textureMap[i];
    }
}

template<class T >
T& Texture<T>::get(const int& i, const int& j){
    assert(0 <= i && i < n && 0 <= j && j < m);
    return textureMap[i * m + j];
}

template<class T >
const T& Texture<T>::get(const int& i, const int& j)const{
    return textureMap[i * m + j];
}
/*
template<class T >
T* Texture<T>::operator[](const int& i) const {
    return textureMap + (i * m);
}
*/
template<class T >
void Texture<T>::resize(int nn, int mm) {
    textureMap.clear();
    this -> n = nn;
    this -> m = mm;

//    textureMap = new T[nn * mm];
    textureMap.resize(n * m);
}

template<class T >
Texture<T >::Texture(const vector<vector<T > > & M){
    n = M.size();
    m = M[0].size();

    textureMap.resize(n * m);
//    this -> resize(n, m);

    for(int i = 0; i < n; i ++){
        for(int j = 0; j < m; j ++){
            textureMap[i * m + j] = M[i][j];
        }
    }
}

template<class T >
const T& Texture<T >::sampleInt(const int & u, const int & v) const {
    return textureMap[max(0, min(u, n - 1)) * m + max(0, min(v, m - 1))];
}

template<class T >
T Texture<T >::interpolation(const T & a, const T & b, const float & v) const {
    return v * a + (1 - v) * b;
}

template<class T >
T Texture<T >::sample2D(float u, float v, int type) const { // u, v in [0, 1]

    if(type){
        return sampleInt(u * n, v * m);
    }


    u *= n;
    v *= m;

    int uu = u, vv = v;
    const auto lu = sampleInt(uu, vv);
    const auto ld = sampleInt(uu + 1, vv);
    const auto ru = sampleInt(uu, vv + 1);
    const auto rd = sampleInt(uu + 1, vv + 1);
    return interpolation(
            interpolation(lu, ru, 1 - (v - vv)),
            interpolation(ld, rd, 1 - (v - vv)),
            1 - (u - uu));

}

template<class T >
T Texture<T >::sample2D(const Vec2 & uv) const {
    return sample2D(uv.x(), uv.y());
}

using std::max;
using std::min;

template<class T >
T Texture<T>::sampleAverage(const Vec2 &uv, const float& r)const {
    const int u1 = min(max(-1, (int)(n * (uv.x() - r))), n - 1);
    const int u2 = min(max(-1, (int)(n * (uv.x() + r))), n - 1);
    const int v1 = min(max(-1, (int)(m * (uv.y() - r))), m - 1);
    const int v2 = min(max(-1, (int)(m * (uv.y() + r))), m - 1);

    const float uv22 = (u2 >= 0 && v2 >= 0) ? sampleInt(u2, v2) : 0;
    const float uv11 = (u1 >= 0 && v1 >= 0) ? sampleInt(u1, v1) : 0;
    const float uv12 = (u1 >= 0 && v2 >= 0) ? sampleInt(u1, v2) : 0;
    const float uv21 = (u2 >= 0 && v1 >= 0) ? sampleInt(u2, v1) : 0;

    return (uv22 + uv11 - uv12 - uv21) / ((u2 - u1) * (v2 - v1));
}

template<class T>
Texture<T>::~Texture(){

//    delete[] (this -> textureMap);
//    this -> textureMap = nullptr;
//    std::cout << "in ~" << '\n';
}

#endif //ASMALLRENDER_TEXTURE_H
