//
// Created by Lingyin on 2024/4/28.
//
#include "Shaders.h"

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

static int find(const Vec4& vv){
    auto& vec = idOfSpacePosition[vv.x()];
    for(auto& p: vec){
        if(p.first == vv){
            return p.second;
        }
    }

    return -1;
}

static vector<int > findCommon(int a, int b){
    vector<int > ret;
    for(int aa : adj[a]){
        for(int bb : adj[b]){
            if(aa == bb){
                ret.push_back(aa);
            }
        }
    }

    return ret;
}

static Vertex fix(int ida, int idb, const Vertex& a, const Vertex& b){

    Vertex x;
    if(moreNormal[ida] && moreNormal[idb]){
        x.spaceV = (a.spaceV + b.spaceV) * 0.5f;
        x.normal = (a.normal + b.normal).normal();
        x.uv = (a.uv + b.uv) * 0.5;
    }
    else //if(!moreNormal[ida] && !moreNormal[idb])
    {
        const auto vv = findCommon(ida, idb);
        if(vv.size() <= 1){
            x.spaceV = (a.spaceV + b.spaceV) * 0.5f;
        }
        else{
//            std::cout << vv.size() << '\n';
           if(vv.size() != 2){
                std::cout << vv.size() << '\n';
            }
            /*
            assert(vv.size() == 2ull);
            const auto p0 = spaceV[vv[0]];
            const auto p1 = spaceV[vv[1]];
            */
             Vec4 v;
             for(auto& p : vv){
                v += spaceV[p];
            }
            x.spaceV = (0.25f / (float)vv.size()) * v + 0.375f * (a.spaceV + b.spaceV);
        }

        x.normal = (a.normal + b.normal).normal();
        x.uv = (a.uv + b.uv) * 0.5;
    }


    /*
    else if(!moreNormal[ida] && moreNormal[idb]){

    }
    else{ // moreNormal[ida] && !moreNormal[idb]

    }
    */

    return x;
}

static Vertex fix(int id, const Vertex& x){
    if(moreNormal[id]){
        return x;
    }

    auto ret = x;
    Vec4 v;
    for(auto t : adj[id]){
        v += spaceV[t];
    }
    v /= 16.f;

    ret.spaceV = (x.spaceV * (16.f - (float)adj[id].size()) / 16.f) + v;
    return ret;
}

vector<Triangle > loopSubdivision(const vector<Triangle >& triangles){

    // clear
    idOfSpacePosition.clear();
    cnt = 0;
    tmpNormal.clear();
    spaceV.clear();
    moreNormal.clear();
    adj.clear();



    for(const auto& triangle : triangles){
        for(const auto& p : triangle.points){
            const auto& v = p.spaceV;

            if(find(v) == -1){
                idOfSpacePosition[v.x()].emplace_back(v, cnt ++);
                tmpNormal.push_back(p.normal);
                moreNormal.push_back(false);
                adj.emplace_back();
                spaceV.push_back(v);
            }
            else{
                int id = find(v);
                if(dot(tmpNormal[id], p.normal) < (1 - myEps)){
                    moreNormal[id] = true;
                }
            }
        }

        int a = find(triangle.points[0].spaceV);
        int b = find(triangle.points[1].spaceV);
        int c = find(triangle.points[2].spaceV);

        adj[a].push_back(b);
        adj[a].push_back(c);
        adj[b].push_back(a);
        adj[b].push_back(c);
        adj[c].push_back(a);
        adj[c].push_back(b);
    }


    for(int i = 0; i < cnt; i ++){
        auto& vec = adj[i];
        std::sort(vec.begin(), vec.end());
        vec.erase(std::unique(vec.begin(), vec.end()), vec.end());
    }

    vector<Triangle > ret;
    for(const auto& triangle : triangles){
        const auto& a = triangle.points[0];
        const auto& b = triangle.points[1];
        const auto& c = triangle.points[2];
        const int ida = find(a.spaceV);
        const int idb = find(b.spaceV);
        const int idc = find(c.spaceV);

        const auto ab = fix(ida, idb, a, b);
        const auto bc = fix(idb, idc, b, c);
        const auto ca = fix(idc, ida, c, a);

        const auto aa = fix(ida, a);
        const auto bb = fix(idb, b);
        const auto cc = fix(idc, c);

        ret.emplace_back(Triangle{ca, aa, ab});
        ret.emplace_back(Triangle{ab, bb, bc});
        ret.emplace_back(Triangle{bc, cc, ca});
        ret.emplace_back(Triangle{ab, bc, ca});
    }

    return ret;
}