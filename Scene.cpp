//
// Created by Lingyin on 2024/4/6.
//

#include "Scene.h"

template<class T>
void Scene::resize(T* &obj, int nn, int mm){
    delete[] obj;
    obj = new T[nn * mm];
    // obj.clear();
    // obj.resize(nn * mm);
}

float& Scene::getZ(const int& x, const int& y) const{
    return zBuffer(x, y);
}

Vec4& Scene::getColor(const int &x, const int &y) const {
    return colorBuffer(x, y);
}

void Scene::setSize(int nn, int mm){
    zBuffer.resize(nn, mm);
    colorBuffer.resize(nn, mm);
    this -> n = nn;
    this -> m = mm;
}

void Scene::cutAndMakeTriangle(const Face &face, const vector<float > &zAfterMV,
                               vector<Triangle >& triangles, const Mesh& mesh, const float& zN) {
    int len = (int)face.points.size();
    int out = 0;
    vector<float > z(len);
    for(int i = 0; i < z.size(); i ++){
        z[i] = zAfterMV[face.points[i][0]];
        out += (z[i] > zN + myEps);
    }

    vector<Vertex > newFace;
    if(out == len){
        return ;
    }
    else if(out == 0){
        for(int i = 0; i < len; i ++){
            newFace.emplace_back(Vec4(mesh.vertexSpace[face.points[i][0]], 1),
                           Vec4(mesh.vertexNormal[face.points[i][2]].normal(), 0),
                           mesh.vertexTexture[face.points[i][1]]);
        }
    }
    else{
        int first = -1;
        for(int a = 0; a < len; a ++){
            int b = (a == len - 1 ? 0 : a + 1);
            if((z[a] > zN + myEps) && z[b] <= zN + myEps){ // i out but nn in
                first = b;
                float za = std::abs(zAfterMV[face.points[a][0]] - zN) + myEps;
                float zb = std::abs(zAfterMV[face.points[b][0]] - zN) + myEps;
                const float invab = 1 / (za + zb);
                za *= invab;
                zb *= invab;
                newFace.emplace_back(
                        Vec4(zb * mesh.vertexSpace[face.points[a][0]] + za * mesh.vertexSpace[face.points[b][0]], 1),
                        Vec4((zb * mesh.vertexNormal[face.points[a][2]] + za * mesh.vertexNormal[face.points[b][2]]).normal(), 0),
                        zb * mesh.vertexTexture[face.points[a][1]] + za * mesh.vertexTexture[face.points[b][1]]
                );
            }
        }

        assert(newFace.size() == 1);
        while(z[first] <= zN + myEps){
            newFace.emplace_back(
                    Vec4(mesh.vertexSpace[face.points[first][0]], 1),
                    Vec4(mesh.vertexNormal[face.points[first][2]].normal(), 0),
                    mesh.vertexTexture[face.points[first][1]]);
            first = (first == len - 1 ? 0 : first + 1);
        }

        int a = first;
        int b = (a == 0 ? len - 1 : a - 1);
        float za = std::abs(zAfterMV[face.points[a][0]] - zN) + myEps;
        float zb = std::abs(zAfterMV[face.points[b][0]] - zN) + myEps;
        const float invab = 1 / (za + zb);
        za *= invab;
        zb *= invab;
        newFace.emplace_back(
                Vec4(zb * mesh.vertexSpace[face.points[a][0]] + za * mesh.vertexSpace[face.points[b][0]], 1),
                Vec4((zb * mesh.vertexNormal[face.points[a][2]] + za * mesh.vertexNormal[face.points[b][2]]).normal(), 0),
                zb * mesh.vertexTexture[face.points[a][1]] + za * mesh.vertexTexture[face.points[b][1]]
        );

        assert(newFace.size() == len - out + 2);
    }

    for(size_t i = 1; i + 1 < newFace.size(); i ++){
        triangles.push_back({newFace[0], newFace[i], newFace[i + 1], face.materialId});
    }
}


void Scene::clearMutex() {
//    resize(mutexOfBuffer, n, m);
}

template<class T>
T& Scene::get(vector<T >& vec, int x, int y) const{
    return vec[x * m + y];
}

void Scene::clearScene(){
    for(int i = 0; i < n * m; i ++){
        zBuffer.p[i] = 0;
        colorBuffer.p[i] = Vec4(0.f, 0.f, 0.f, 0.f);
    }

//    clearMutex();
}

bool Scene::in(float x, float y, const Vec4 & a, const Vec4 & b, const Vec4 & c){
    Vec2 p(x, y);
    const auto aa = a.xy();
    const auto bb = b.xy();
    const auto cc = c.xy();

    const float v1 = cross(aa - p, aa - bb);
    const float v2 = cross(bb - p, bb - cc);
    const float v3 = cross(cc - p, cc - aa);
    if(v1 >= -myEps && v2 >= -myEps && v3 >= -myEps){
        return true;
    }
    if(v1 <= +myEps && v2 <= +myEps && v3 <= +myEps){
        return true;
    }
    return false;
}

Vec3 Scene::barycenteicCoordinates(float x, float y, const Vec4 & a, const Vec4 & b, const Vec4 & c){
    const float& xa = a.x(), ya = a.y();
    const float& xb = b.x(), yb = b.y();
    const float& xc = c.x(), yc = c.y();
    float arfa = (-( x - xb) * (yc - yb) + ( y - yb) * (xc - xb))
               / (-(xa - xb) * (yc - yb) + (ya - yb) * (xc - xb));
    float beta = (-( x - xc) * (ya - yc) + ( y - yc) * (xa - xc))
               / (-(xb - xc) * (ya - yc) + (yb - yc) * (xa - xc));
    float gama = 1 - arfa - beta;

    return Vec3(arfa, beta, gama);
}

float Scene::mixProduct(const Vec4 & a, const Vec4 &b, const Vec4 & c){
    return dot(cross(a, b), c);
}

template<class T >
T Scene::inster(const T& a, const T & b, const T& c, const Vec3& BC){
    return a * BC.x() + b * BC.y() + c * BC.z();
}

static bool cmp(const Triangle& a, const Triangle& b){
    return a.disToCamera < b.disToCamera;
}

void Scene::drawOnFragment(const vector<Triangle > & triangles,
                           Information& publicInfo,
                           const Thing& thing,
                           FragmentFunctionPointer fragmentFunction,
                           Scene* scene){
//    extern long long totTim;
//    long long tim = clock();
    int val = 0;

    int n = scene -> n, m = scene -> m;
    using std::min;
    using std::max;

    for(const auto & triangle : triangles){
        const auto & vertexa = triangle.points[0];
        const auto & vertexb = triangle.points[1];
        const auto & vertexc = triangle.points[2];

        const auto & screen_v_a = vertexa.screenV; // [-1, +1]^3
        const auto & screen_v_b = vertexb.screenV; // [-1, +1]^3
        const auto & screen_v_c = vertexc.screenV; // [-1, +1]^3


        const auto MV = publicInfo.Vmat * publicInfo.Mmat;
        const auto MV2 = Vec4(MV[2][0], MV[2][1], MV[2][2], MV[2][3]);
        float iMin = (max(min({screen_v_a.x(), screen_v_b.x(), screen_v_c.x()}), (float)-1) + 1) / 2;
        float iMax = (min(max({screen_v_a.x(), screen_v_b.x(), screen_v_c.x()}), (float)+1) + 1) / 2;
        float jMin = (max(min({screen_v_a.y(), screen_v_b.y(), screen_v_c.y()}), (float)-1) + 1) / 2;
        float jMax = (min(max({screen_v_a.y(), screen_v_b.y(), screen_v_c.y()}), (float)+1) + 1) / 2;
        const auto& MVaz = dot(MV2, vertexa.spaceV);
        const auto& MVbz = dot(MV2, vertexb.spaceV);
        const auto& MVcz = dot(MV2, vertexc.spaceV);



        int il = max(0, (int)(iMin * m - 1)), ir = min(m - 1, (int)(iMax * m));
        int jl = max(0, (int)(jMin * n - 1)), jr = min(n - 1, (int)(jMax * n));


        const float& xa = screen_v_a.x(), ya = screen_v_a.y();
        const float& xb = screen_v_b.x(), yb = screen_v_b.y();
        const float& xc = screen_v_c.x(), yc = screen_v_c.y();

        const float invDnArfa = 1.f / (-(xa - xb) * (yc - yb) + (ya - yb) * (xc - xb));
        const float invDnBeta = 1.f / (-(xb - xc) * (ya - yc) + (yb - yc) * (xa - xc));

        for(int i = il; i <= ir; i ++){
            for(int j = jl; j <= jr; j ++){
                float x = 2.0f * i / m - 1, y = 2.0f * j / n - 1;


                /*

                    float arfa = (-( x - xb) * (yc - yb) + ( y - yb) * (xc - xb))
                               / (-(xa - xb) * (yc - yb) + (ya - yb) * (xc - xb)); == dnArfa
                    float beta = (-( x - xc) * (ya - yc) + ( y - yc) * (xa - xc))
                               / (-(xb - xc) * (ya - yc) + (yb - yc) * (xa - xc)); == dnBeta
                    float gama = 1 - arfa - beta;

                 */
        //        Vec3 BC = barycenteicCoordinates(x, y, screen_v_a, screen_v_b, screen_v_c);

                float arfa = (-(x - xb) * (yc - yb) + (y - yb) * (xc - xb)) * invDnArfa;
                float beta = (-(x - xc) * (ya - yc) + (y - yc) * (xa - xc)) * invDnBeta;
                Vec3 BC(arfa, beta, 1 - arfa - beta);

                if(BC.x() < -myEps || BC.y() < -myEps || BC.z() < -myEps){
                    continue;
                }

                const auto screen_v_fragment = inster(screen_v_a, screen_v_b, screen_v_c, BC);


                if(std::abs(publicInfo.Pmat[3][3]) < myEps){
                    BC = Vec3(BC.x() / MVaz, BC.y() / MVbz, BC.z() / MVcz);
                    BC = 1.f / (BC.x() + BC.y() + BC.z()) * BC;
                }


                if(screen_v_fragment.z() + 1 + myEps >= scene -> getZ(j, i)
                   && screen_v_fragment.z() < 1 + myEps
                   && screen_v_fragment.z() >= -1 -myEps){
                    val ++;

                    auto thisFragmentVertex = insterVertex(vertexa, vertexb, vertexc, BC);
                    if(fragmentFunction == nullptr){
                        scene -> getZ(j, i) = screen_v_fragment.z() + 1 + myEps;
                        publicInfo.shadowMap.textureMap[j * m + i] = (publicInfo.Mmat * thisFragmentVertex.spaceV - publicInfo.lightPosition).mo();
                    }
                    else{
                        scene -> getZ(j, i) = screen_v_fragment.z() + 1 + myEps;
                        fragmentFunction(publicInfo, thing.materials[triangle.materialId], thisFragmentVertex, scene -> getColor(j, i));
                    }
                }
            }
        }
    }

/*
    val = 0;
    for(int i = 0; i < n; i ++){
        for(int j = 0; j < m; j ++)
            if(scene -> get(scene -> zBuffer, i, j) > myEps){
                fragmentFunction(publicInfo,
                                 thing.materials[scene -> get(scene -> materialIdBuffer, i, j)],
                                 scene -> get(scene -> vertexBuffer, i, j),
                                 scene -> get(scene -> colorBuffer, i, j));
                val ++;
        }
    }

*/
//    totTim += clock() - tim;
//    std::cout << "val = " << val << '\n';
}


void Scene::beginDraw(const Thing& thing,
                      Information& publicInfo,
                      VertexFunctionPointer vertexFunction,
                      FragmentFunctionPointer fragmentFunction,
                      float width, float height, float nZ, float fZ){

    const auto& camera = publicInfo.camera;

    // notice nZ < 0
    if(width == Scene::usingCameraParameter){
        width = camera.width;
        height = camera.height;
        nZ = camera.zNear;
        fZ = camera.zFar;
    }

    auto& mesh = thing.mesh;

    const auto MV = publicInfo.Vmat * publicInfo.Mmat;
    auto numOfNode = mesh.vertexSpace.size();
    vector<float > zAfterMV(numOfNode);
    for(int i = 0; i < numOfNode; i ++){
        auto& spaceV = mesh.vertexSpace[i];
        const auto tmp = MV * Vec4(spaceV, 1);
        zAfterMV[i] = tmp.z() / tmp.w();
        //        zAfterMV[i] = MV.val[2][0] * spaceV.x() +  MV.val[2][1] * spaceV.y() + MV.val[2][2] * spaceV.z() + MV.val[2][3];
    }

    vector<Triangle > triangles;
    for(auto& face : mesh.faces){
        cutAndMakeTriangle(face, zAfterMV, triangles, mesh, nZ);
    }

//    triangles = loopSubdivision(triangles);
    // vertex Scene
    for(auto & triangle : triangles){
        for(auto & vertex : triangle.points){
            if(vertexFunction != nullptr){
                vertexFunction(publicInfo, thing.materials[triangle.materialId], vertex);
//            assert(vertexInfo.TextureInfo.size() == 0); // can't have texture
                // screen must be 'one'
            }
            else{
                vertex.screenV = (publicInfo.MVP * vertex.spaceV).one();
            }
        }
    }


    drawOnFragment(triangles, publicInfo, thing, fragmentFunction, this);





}

void Scene::drawLine(const Thing& thing,
                     const Mat4& Mmat, const Mat4& Vmat, const Mat4 & Pmat,
                     float nZ){

    // notice nZ < 0

    auto& mesh = thing.mesh;

    const auto MV = Vmat * Mmat;
    auto numOfNode = mesh.vertexSpace.size();
    vector<float > zAfterMV(numOfNode);
    for(int i = 0; i < numOfNode; i ++){
        auto& spaceV = mesh.vertexSpace[i];
        const auto tmp = MV * Vec4(spaceV, 1);
        zAfterMV[i] = tmp.z() / tmp.w();
    }

    vector<Triangle > triangles;
    for(auto& face : mesh.faces){
        cutAndMakeTriangle(face, zAfterMV, triangles, mesh, nZ);
    }


    const auto MVP = Pmat * Vmat * Mmat;

//    triangles = loopSubdivision(triangles);
    for(auto & triangle : triangles){
        const auto p0 = (MVP * triangle.points[0].spaceV).one();
        const auto p1 = (MVP * triangle.points[1].spaceV).one();
        const auto p2 = (MVP * triangle.points[2].spaceV).one();

        drawLine(p0, p1);
        drawLine(p1, p2);
        drawLine(p2, p0);
    }

}


void Scene::drawLine(const Vec4& a, const Vec4& b){
    // [-1, +1] ^ 3
    int x0 = (a.x() + 1) * n / 2, y0 = (a.y() + 1) * m / 2;
    int x1 = (b.x() + 1) * n / 2, y1 = (b.y() + 1) * m / 2;


//    Vec4 color(249.5, 0, 0, 0);
    Vec4 color(249, 249, 249, 0);
    using std::swap;
    if(abs(x0 - x1) >= abs(y0 - y1)){
        if(x0 > x1){
            swap(x0, x1);
            swap(y0, y1);
        }
        int len = x1 - x0 + 1;
        for(int x = x0; x <= x1; x ++){
            int y = (y0 * (x1 - x) + y1 * (x - x0 + 1)) / len;
            if(0 <= y && y < n && 0 <= x && x < m){
                colorBuffer(y, x) = color;
            }
        }
    }
    else{
        if(y0 > y1){
            swap(x0, x1);
            swap(y0, y1);
        }
        int len = y1 - y0 + 1;
        for(int y = y0; y <= y1; y ++){
            int x = (x0 * (y1 - y) + x1 * (y - y0 + 1)) / len;
            if(0 <= y && y < n && 0 <= x && x < m){
                colorBuffer(y, x) = color;
            }
        }
    }
}


Scene::~Scene(){


//    delete[] zBuffer;
//    delete[] colorBuffer;

//    zBuffer = nullptr;
//    colorBuffer = nullptr;

/*

    for(int i = 0; i < n; i ++){
        delete[] zBuffer[i];
        delete[] colorBuffer[i];
//        delete[] mutexOfBuffer[i];
    }
    delete[] zBuffer;
    delete[] colorBuffer;
//    delete[] mutexOfBuffer;

*/
}