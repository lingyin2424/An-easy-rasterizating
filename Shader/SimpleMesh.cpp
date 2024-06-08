//
// Created by Lingyin on 2024/6/8.
//
#include "Shaders.h"
#include <utility>
#include "set"

using std::vector;
using std::set;

double pp(float x){
    return x * x;
}


struct Graph{
    vector<Vec3 > point;
    vector<set<array<int ,3> > > adj;

    struct Edge{
        int a, b;
        double error2;
        Vec3 point;
    };
    vector<Edge > edges;

    static float mix(const Vec3& a, const Vec3& b, const Vec3& c){
        return dot(cross(a, b), c);
    }

    static float solve(const Vec3& A, float& a,
                      const Vec3& B, float& b,
                      const Vec3& C, float& c,
                      const Vec3& D){
        auto dn = mix(A, B, C);
        auto inv = 1 / dn;
        a = mix(D, B, C) * inv;
        b = mix(A, D, C) * inv;
        c = mix(A, B, D) * inv;
        return dn;
    }

    void addError2(int a, int b){

        double A = 0, B = 0, C = 0;
        double D = 0, E = 0, F = 0;
        double G = 0, H = 0, I = 0;
        double J = 0;

        for(auto& ff : adj[a]){
            auto p = point[ff[0]];
            auto n = cross(point[1] - point[0], point[2] - point[0]);

            auto b = dot(p, n);
            J += b * b;

            A += pp(n.x());
            B += pp(n.y());
            C += pp(n.z());

            D += 2 * n.x() * n.y();
            E += 2 * n.y() * n.z();
            F += 2 * n.z() * n.x();

            F += -2 * b * n.x();
            G += -2 * b * n.y();
            H += -2 * b * n.z();
        }

        for(auto& ff : adj[b]){
            auto p = point[ff[0]];
            auto n = cross(point[1] - point[0], point[2] - point[0]);

            auto b = dot(p, n);
            J += b * b;

            A += pp(n.x());
            B += pp(n.y());
            C += pp(n.z());

            D += 2 * n.x() * n.y();
            E += 2 * n.y() * n.z();
            F += 2 * n.z() * n.x();

            F += -2 * b * n.x();
            G += -2 * b * n.y();
            H += -2 * b * n.z();
        }

        Vec3 AA(2 * A, D, F);
        Vec3 BB(D, 2 * B, E);
        Vec3 CC(F, E, 2 * C);
        Vec3 DD(-G, -H, -I);

        float x, y, z;
        auto dn = solve(AA, x, BB, y, CC, z, DD);

        edges.emplace_back();
        auto& e = edges.back();
        e.a = a;
        e.b = b;

        if(dn <= myEps){
            e.point = (point[a] + point[b]) / 2;
        }
        else{
            e.point = Vec3(x, y, z);
        }
    }

    void addFace(set<array<int, 3 > >& s, int a, int b, int c){
        if(a != b && b != c && c != a){
            int aa = min({a, b, c});
            int cc = max({a, b, c});
            int bb = a ^ b ^ c ^ aa ^ cc;

            s.insert({aa, bb, cc});
        }
    }

    void delEdge(Edge& e){
        int a = e.a;
        int b = e.b;
        point.push_back(e.point);

        set<int > pointNeedChange;
        for(auto& ff : adj[a]){
            pointNeedChange.insert(ff[0]);
            pointNeedChange.insert(ff[1]);
            pointNeedChange.insert(ff[2]);
        }
        for(auto& ff : adj[b]){
            pointNeedChange.insert(ff[0]);
            pointNeedChange.insert(ff[1]);
            pointNeedChange.insert(ff[2]);
        }

        for(int v : pointNeedChange){
            set<array<int, 3 > > tmpSet;
            for(auto& ff : adj[v]){
                auto tmpArray = ff;
                for(int i = 0; i < 3; i ++){
                    if(ff[i] == a || ff[i] == b){
                        tmpArray[i] = point.size() - 1;
                    }
                }
                addFace(tmpSet, tmpArray[0], tmpArray[1], tmpArray[2]);
            }
            adj[v] = tmpSet;
        }


    }


};


Graph simpleMesh(Graph g, int n){


}
