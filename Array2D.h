#pragma once

#include "Thing.h"


template<class T>
struct Array2D{
    T *p = nullptr;
    int n = 0, m = 0;

private:
public:
    Array2D() = default;
    Array2D(int n, int m);
    Array2D(const Array2D<T>& v);
    int getn() const ;
    int getm() const ;
    void resize(int n, int m);
    ~Array2D();
    T& operator ()(const int& i, const int& j) const;

    Array2D<T >& operator =(const Array2D<T >& b);
};

/*
using Array2DInt = Array2D<int>;
using Array2DVec4 = Array2D<Vec4>;
using Array2DFloat = Array2D<float>;
*/