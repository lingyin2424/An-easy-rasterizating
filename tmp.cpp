#include<iostream>
#include<iomanip>
#include<assert.h>
#include<vector>
#include<algorithm>

using namespace std;

const long double eps = 1e-9;

const int N = 1e6+10;
int n;
int x[N], y[N], xv[N], yv[N];

void myassert(bool x){
    if(x == false){
        int cnt = 0;
        for(;;){
            cnt += rand();
        }
    }
}


struct Line{
    int k, b; // y = k * x + b
};

long double cross(const Line& u, const Line& v){
    assert(u.k != v.k);
    return (long double)(v.b - u.b) / (u.k - v.k);
}

struct Hull{

    int type;
    vector<Line > st;

    bool cmp_by_k_dn(const Line& a, const Line& b){
        return a.k > b.k;
    }
    bool cmp_by_k_up(const Line& a, const Line& b){
        return a.k < b.k;
    }

    void makeHull(vector<Line > lines){
        if(type == 1){
            sort(lines.begin(), lines.end(), cmp_by_k_dn);
        }
        else{
            sort(lines.begin(), lines.end(), cmp_by_k_up);
        }
        st.clear();


        for(auto & l : lines){
            if(!st.empty() && st.back().k == l.k){
                if(l.b * type <= st.back().b * type){
                    continue;
                }
                else{
                    st.pop_back();
                }
            }

            if(st.size() == 0){
                st.push_back(l);
                continue;
            }

            assert(l.k * type < st.back().k * type);
            while(st.size() >= 2 && cross(st[st.size() - 2], l) < cross(st[st.size() - 2], st.back())){
                st.pop_back();
            }
            st.push_back(l);
        }
    }


    long double tim = 0;
    int p = 0;
    void getTime(vector<long double >& times) const {
        for(int i = 1; i < st.size(); i ++){
            times.push_back(cross(st[i - 1], st[i]));
        }
    }

    void moveTime(long double t){
        while(p + 1 != st.size() && t >= cross(st[p], st[p + 1]) - eps){
            p ++;
        }
    }

}xMin, xMax, yMin, yMax;


long double getMin(long double l, long double r, int k1, int b1, int k2, int b2){

    auto ret = min((k1 * l + b1) * (k2 * l + b2), (k1 * r + b1) * (k2 * r + b1));
    auto mid = - (long double)(k1 * b2 + k2 * b1) / (2 * k1 * k2);
    if(l <= mid && mid <= r){
        ret = min(ret, (k1 * mid + b1) * (k2 * mid + b1));
    }

    return ret;
}

void work(){

    cin >> n;
    for(int i = 1; i <= n; i ++){
        cin >> x[i] >> y[i] >> xv[i] >> yv[i];
    }

    vector<Line > xLine(n), yLine(n);
    for(int i = 0; i < n; i ++){
        xLine[i].k = xv[i + 1]; xLine[i].b = x[i + 1];
        yLine[i].k = yv[i + 1]; yLine[i].b = y[i + 1];
    }

    xMin.type = +1; xMin.makeHull(xLine);
    xMax.type = -1; xMax.makeHull(xLine);
    yMin.type = +1; yMin.makeHull(yLine);
    yMax.type = -1; yMax.makeHull(yLine);

    vector<long double > importantTime;
    xMax.getTime(importantTime);
    xMin.getTime(importantTime);
    yMax.getTime(importantTime);
    yMin.getTime(importantTime);

    sort(importantTime.begin(), importantTime.end());

    long double seele = 4e19;
    for(int i = 1; i < importantTime.size(); i ++){
        auto l = importantTime[i - 1], r = importantTime[i];
        xMin.moveTime(l);
        xMax.moveTime(l);
        yMin.moveTime(l);
        yMax.moveTime(l);

        int kx = xMax.st[xMax.p].k - xMin.st[xMin.p].k;
        int bx = xMax.st[xMax.p].b - xMin.st[xMin.p].b;

        int ky = yMax.st[yMax.p].k - yMin.st[yMin.p].k;
        int by = yMax.st[yMax.p].b - yMin.st[yMin.p].b;

        seele = min(seele, getMin(l, r, kx, bx, ky, by));
    }

    cout << fixed << setprecision(15) << seele << endl;

}

signed main(){

    ios::sync_with_stdio(0);
    cin.tie(0);

    int __ = 1;
//    cin >> __;

    while(__ --){
        work();
    }
}

/*

2
1000000000 1000000000 -1 -1
-1000000000 -1000000000 0 0


23
150 41240 110 -76
150 -71806 110 127
-135 100373 -117 -457
-135 -120701 -117 360
-135 -71304 -117 125
-135 -185507 -117 853
-135 -96206 -117 228
-135 -89846 -117 199
150 106114 110 -512
-135 -364728 -117 3338
-135 -240138 -117 1435
-135 -136915 -117 463
150 38262 110 -66
-135 -486593 -117 5988
150 259486 110 -3116
-135 -68203 -117 114
150 69180 110 -216
150 -67749 110 113
-135 -272201 -117 1848
150 -112075 110 310
-135 -385412 -117 3732
-135 81309 -117 -299
150 -137841 110 470

*/