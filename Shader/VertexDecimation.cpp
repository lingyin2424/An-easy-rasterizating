//
// Created by Lingyin on 2024/6/6.
//
/*

#include <queue>
#include "Shaders.h"
#include "../Mesh.h"

static bool operator == (const Vec4& a, const Vec4& b){

    for(int i = 0; i < 4; i ++){
        if(std::abs(a.e[i] - b.e[i]) > myEps){
            return false;
        }
    }
    return true;
}

using std::map;
using std::pair;
static map<float, vector<pair<Vec4, int > > > idOfSpacePosition;
static int cnt = 0;
static vector<Vec4 > tmpNormal, spaceV;
static vector<bool > moreNormal;
static vector<vector<int > > adj;

static Vec3 getNormal(const Face& face, const Mesh& mesh){
    auto p0 = mesh.vertexSpace[face.points[0][0]];
    auto p1 = mesh.vertexSpace[face.points[1][0]];
    auto p2 = mesh.vertexSpace[face.points[2][0]];

    return cross(p1 - p0, p2 - p0).normal();
}

static double pToFace(const Vec3& p,  const Face& face, const Mesh& mesh){
    auto p0 = mesh.vertexSpace[face.points[0][0]];
    return std::abs(dot(p - p0, getNormal(face, mesh)));
}

template<class T >
T pp (const T& x){
    return x * x;
}

struct Edge{

    static void add(int a, int b, vector<Edge >& edges){
        edges.emplace_back();
        edges.back().a = a;
        edges.back().b = b;
        edges.back().idx = (int)edges.size() - 1;
    }

    void updateError2(const Mesh& mesh, const vector<vector<int > >& adj){
        error2 = 0;
        auto mid = (mesh.vertexSpace[a] + mesh.vertexSpace[b]) / 2;
        for(int idFace : adj[a]){
            error2 += pp(pToFace(mid, mesh.faces[idFace], mesh));
        }
        for(int idFace : adj[b]){
            error2 += pp(pToFace(mid, mesh.faces[idFace], mesh));
        }
    }

    int a = 0, b = 0, idx = 0;
    double error2 = 0;

    bool operator < (const Edge & e) const{
        return error2 > e.error2;
    }
    bool operator > (const Edge & e) const {
        return error2 < e.error2;
    }
};

vector<Edge > edges;


vector<Triangle > loopSubdivision(const Mesh& mesh, int restN){
    adj.resize(mesh.vertexSpace.size());


    auto& faces = mesh.faces;
    for(int idFace = 0; idFace < faces.size(); idFace ++){
        auto& face = faces[idFace];
        for(int a = 0; a < face.points.size(); a ++){
            int b = (a + 1) % face.points.size();

            int idA = face.points[a][0];
            int idB = face.points[b][0];
            Edge::add(idA, idB, edges);
            adj[idA].push_back(idFace);
        }
    }

    std::priority_queue<Edge > qq;
    for(auto& edge :edges){
        edge.updateError2(mesh, adj);
        qq.push(edge);
    }

    auto faceNum = mesh.faces.size();
    auto retMesh(mesh);
    while(faceNum > restN){
        auto e = qq.top();
        qq.pop();

        int a = e.a, b = e.b;

        if(mesh.vertexTexture[a])
        auto space = (mesh.vertexSpace[a] + mesh.vertexSpace[b]) / 2;
        auto normal = (mesh.vertexNormal[a] + mesh.vertexNormal[b]).normal();
        auto uv = (mesh.vertexTexture[a] + mesh.vertexTexture[b]) / 2;

        for(auto idFace : )
    }



}

 */