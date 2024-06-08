//
// Created by Lingyin on 2024/4/18.
//

#include "Shaders.h"


void vsmVertexShader(const Information & publicInfo, [[maybe_unused]]const Material& mtl, Vertex & vertex){
    vertex.screenV = (publicInfo.MVP * vertex.spaceV).one();
}

template<class T >
static T pp(const T& x){
    return x * x;
}

static int inRange(const int& l, const int& r, const int& x){
    return max(l, min(r, x));
}

template<class T >
static T sample(const int &x, const int &y, const Texture<T >& texture){
    return (x != -1 && y != -1) ? texture.sampleInt(x, y) : 0;
}

template<class T >
static T sampleSum(const int& u1, const int& u2, const int& v1, const int& v2, const Texture<T >& texture){
    auto ret =
      sample(u2, v2, texture)
    + sample(u1, v1, texture)
    - sample(u1, v2, texture)
    - sample(u2, v1, texture);
/*
    if(ret < 0){
        ret += M;
    }
*/
    return ret;
}

void vsmFragmentShader(const Information & publicInfo, const Material& mtl, Vertex & vertex, Vec4 & fragmentColor){

    const auto& lightPosition = publicInfo.lightPosition;
    const auto& v = vertex.spaceV;
    const auto& shadowMVP = publicInfo.shadowMVP;
    const auto& vInShadowSpace = (shadowMVP * v).one();
    const float& dd = std::sqrt(dis2(lightPosition, v));

    const Vec2 shadowSpace(vInShadowSpace.y() * 0.5f + 0.5f, vInShadowSpace.x() * 0.5f + 0.5f);

    float visible = 1.f;
    if(-1 < vInShadowSpace.x() && vInShadowSpace.x() < 1
    && -1 < vInShadowSpace.y() && vInShadowSpace.y() < 1
    && -1 < vInShadowSpace.z() && vInShadowSpace.z() < 1){

        const int& n = publicInfo.shadowMap.n;
        const int& m = publicInfo.shadowMap.m;

        const double& r = 0.01;
        int u1 = inRange(-1, n - 1, (shadowSpace.x() - r) * n);
        int u2 = inRange(-1, n - 1, (shadowSpace.x() + r) * n);
        int v1 = inRange(-1, m - 1, (shadowSpace.y() - r) * m);
        int v2 = inRange(-1, m - 1, (shadowSpace.y() + r) * m);
        auto uuvv = (u2 - u1) * (v2 - v1);
        const int& blockNum = sampleSum(u1, u2, v1, v2, publicInfo.block);
        if(blockNum == 0){
            visible = 1;
        }
        else{

            const double& mu = sampleSum(u1, u2, v1, v2, publicInfo.sumShadowMap) / blockNum;
            const double& mu2 = mu * mu;
            const double& sigma2 = sampleSum(u1, u2, v1, v2, publicInfo.sumShadowMap2) / blockNum - mu2 + myEps;
            const double& chebychev = ((uuvv - blockNum) + sigma2 / (sigma2 + pp(dd - mu)) * blockNum) / uuvv;

     //       fragmentColor = Vec4(blockNum, blockNum, blockNum, 0);
       //     fragmentColor = Vec4(chebychev, chebychev, chebychev, 0) * 255;
        //    return ;

            /*
        if(sigma2 <= 0){
            assert(0);
            fragmentColor = Vec4(249, 249, 249, 0);
            return ;
        }

            if(!(-myEps < chebychev && chebychev < 1.f + myEps)){
                using namespace std;
                cout << sampleSum(u1, u2, v1, v2, publicInfo.sumShadowMap2) / blockNum << endl;
                cout << sampleSum(u1, u2, v1, v2, publicInfo.sumShadowMap2) / blockNum * (1 + myEps) - mu2 << endl;
                cout << sampleSum(u1, u2, v1, v2, publicInfo.sumShadowMap2) / blockNum - mu2 + myEps << endl;

                cout
                        << " mu = " << mu
                        << " sigma = " << sampleSum(u1, u2, v1, v2, publicInfo.sumShadowMap2) / blockNum
                        << " sigma2 =  " << sigma2
                        << " mu2 = " << mu2 << endl;
//
//            assert(0);

                double mu = 0, mu2 = 0, sigma2 = 0;
                for(int i = u1 + 1; i <= u2; i ++){

                    for(int j = v1 + 1; j <= v2; j ++){
                        mu += publicInfo.shadowMap.sampleInt(i, j);
                        sigma2 += pp(publicInfo.shadowMap.sampleInt(i, j));
                        cout << fixed << setprecision(2) << publicInfo.shadowMap.sampleInt(i, j) << ' ';
                    }


                    cout << endl;
                }

                double qq = publicInfo.sumShadowMap.sampleInt(u2, v2);
                double ww = publicInfo.sumShadowMap.sampleInt(u1, v1);
                double qw = publicInfo.sumShadowMap.sampleInt(u1, v2);
                double wq = publicInfo.sumShadowMap.sampleInt(u2, v1);
                cout << qq << ' ' << ww << ' ' << qw << ' ' << wq << endl;
                cout << qq + ww - qw - wq << endl;
                cout << (qq + ww - qw - wq) / blockNum << endl;
                cout << u1 << ' ' << u2 << ' ' << v1 << ' ' << v2 << endl;
                mu /= blockNum;
                mu2 = pp(mu);


                cout << mu << ' ' << sigma2 << endl;
                cout << mu << ' ' << mu2 << ' ' << sigma2 << ' ' << sigma2 - mu2 << endl;
                cout << "fangcha = " << sigma2 / blockNum - mu2 << endl;
                assert(0);

            }

            */
            visible = chebychev;
        }
    }

  //  fragmentColor = Vec4(254, 254, 254, 0);
  //  return ;

    pbrFragmentShader(publicInfo, mtl, vertex, fragmentColor);
    fragmentColor *= visible;
}

