//
// Created by Lingyin on 2024/4/5.
//
#include "Mat.h"


// constucter
Mat4::Mat4(){
    memset(val, 0, sizeof(val));
    val[0][0] = val[1][1] = val[2][2] = val[3][3] = 1;
}

Mat4::Mat4(float v){
    for(auto & i : val){
        for(float & j : i){
            j = v;
        }
    }
}

Mat4::Mat4(const std::initializer_list<float> & args){
    memset(val, 0, sizeof(val));
    int t = 0;
    for(const auto& arg : args){
        assert(t != 16);
        val[t >> 2][t & 3] = arg;
        t ++;
    }
}

Mat4 Mat4::T() const{
    Mat4 ret;
    for(int i = 0; i < 4; i ++){
        for(int j = 0; j < 4; j ++){
            ret.val[i][j] = val[j][i];
        }
    }
    return ret;
}

Mat4 Mat4::Inv() const{
    float tmpVal[4][8];
    for(int i = 0; i < 4; i ++){
        for(int j = 0; j < 4; j ++){
            tmpVal[i][j] = val[i][j];
        }
        for(int j = 4; j < 8; j ++){
            tmpVal[i][j] = (i + 4 == j);
        }
    }
    int tot = 0;
    for(int j = 0; j < 4; j ++){
        int first = -1;
        for(int i = tot; i < 4; i ++){
            if(std::fabs(tmpVal[i][j]) > myEps){
                first = i;
            }
        }
        if(first == -1){
            continue;
        }
        for(int i = 0; i < 4; i ++){
            if(i == first || tmpVal[i][j] == 0){
                continue;
            }
            {
                float v = tmpVal[i][j] / tmpVal[first][j];
                for(int k = 0; k < 8; k ++){
                    tmpVal[i][k] = tmpVal[i][k] - tmpVal[first][k] * v;
                }
            }
        }

        {
            float v = 1 / tmpVal[first][j];
            for(int k = 0; k < 8; k ++){
                tmpVal[first][k] *= v;
                std::swap(tmpVal[first][k], tmpVal[tot][k]);
            }
        }

        tot ++;
    }
    Mat4 ret;
    for(int i = 0; i < 4; i ++){
        for(int j = 0; j < 4; j ++){
            ret.val[i][j] = tmpVal[i][j + 4];
        }
    }

    {
        auto tmp = (*this) * ret;
        for(int i = 0; i < 4; i ++){
            for(int j = 0; j < 4; j ++){
                if(i == j){
                    assert(std::fabs(tmp[i][j] - 1) < myEps);
                }
                else{
                    assert(std::fabs(tmp[i][j] - 0) < myEps);
                }
            }
        }

    }

    return ret;
}

float* Mat4::operator [] (const int & i) const {
    return const_cast<float*>(val[i]);
}

void Mat4::print() const{
    std::cout << std::fixed << std::setprecision(3);
    for(auto & i : val){
        for(auto & j : i){
            std::cout << j << ' ';
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

Mat4 operator * (const Mat4 & a, const Mat4 & b){
    Mat4 ret(0);
    for(int k = 0; k < 4; k ++){
        for(int i = 0; i < 4; i ++){
            for(int j = 0; j < 4; j ++){
                ret[i][j] += a[i][k] * b[k][j];
            }
        }
    }

    return ret;
}

Mat4 operator * (const float & k, const Mat4 & b){
    Mat4 ret(0);
    for(int i = 0; i < 4; i ++){
        for(int j = 0; j < 4; j ++){
            ret[i][j] = k * b[i][j];
        }
    }
    return ret;
}

Mat4 operator * (const Mat4 & b, const float & k){
    Mat4 ret(0);
    for(int i = 0; i < 4; i ++){
        for(int j = 0; j < 4; j ++){
            ret[i][j] = k * b[i][j];
        }
    }
    return ret;
}

Mat4 operator + (const Mat4 & a, const Mat4 & b){
    Mat4 ret(0);
    for(int i = 0; i < 4; i ++){
        for(int j = 0; j < 4; j ++){
            ret[i][j] = a[i][j] + b[i][j];
        }
    }
    return ret;
}

Mat4 operator - (const Mat4 & a, const Mat4 & b){
    Mat4 ret(0);
    for(int i = 0; i < 4; i ++){
        for(int j = 0; j < 4; j ++){
            ret[i][j] = a[i][j] - b[i][j];
        }
    }
    return ret;
}

Vec4 operator * (const Mat4 & a, const Vec4 & b){
    Vec4 ret;
    for(int i = 0; i < 4; i ++){
        for(int j = 0; j < 4; j ++){
            ret.e[i] += a[i][j] * b.e[j];
        }
    }

    return ret;
}

Mat4 getProjectionMat(const float& w, const float& h, const float& n, const float& f) {
    return Mat4({
       2 * n / w, 0, 0, 0,
       0, 2 * n / h, 0, 0,
       0, 0, (n + f) / (n - f), -2 * n * f / (n - f),
       0, 0, 1, 0
       });
}

Mat4 getOrthoMat(const float& w, const float& h, const float& n, const float& f){
    return Mat4({
        2 / w, 0, 0, 0,
        0, 2 / h, 0, 0,
        0, 0, 2 / (n - f), -(n + f) / (n - f),
        0, 0, 0, 1
    });
}

Mat4 getOrthoMat(const float& l, const float& r, const float &d, const float& u, const float& n, const float& f){
    // l < r; d < u
    return Mat4({
        2 / (r - l), 0, 0, -2 * l / (r - l) - 1,
        0, 2 / (u - d), 0, -2 * d / (u - d) - 1,
        0, 0, 2 / (n - f), -(n + f) / (n - f),
        0, 0, 0, 1
    });
}