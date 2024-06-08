//
// Created by Lingyin on 2024/5/21.
//

#include "Array2D.h"

template<class T>
Array2D<T >::Array2D(int n, int m){
    this -> n = n;
    this -> m = m;
    this -> p = new T[n * m];
}

template<class T>
Array2D<T >::Array2D(const Array2D<T>& v){
    this -> n = v.n;
    this -> m = v.m;
    this -> p = new T[n * m];
    for(int i = 0; i < n * m; i ++){
        (this -> p)[i] = v.p[i];
    }
}

template<class T>
int Array2D<T>::getn() const {
    return n;
}

template<class T>
int Array2D<T>::getm() const {
    return m;
}

template<class T>
void Array2D<T>::resize(int n, int m){
    delete[] this -> p;
    this -> p = new T[n * m];
    this -> n = n;
    this -> m = m;
}

template<class T>
Array2D<T>::~Array2D(){
    delete[] this -> p;
}

template<class T>
T& Array2D<T>::operator ()(const int& i, const int& j) const{
    return p[i * m + j];
}

template<class T>
Array2D<T>& Array2D<T >::operator =(const Array2D<T >& b){

    delete [] p;
    this -> n = b.n;
    this -> m = b.m;
    this -> p = new T[n * m];
    for(int i = 0; i < n * m; i ++){
        p[i] = b.p[i];
    }
    return *this;
}



template struct Array2D<int >;
template struct Array2D<float >;
template struct Array2D<Vec4 >;


