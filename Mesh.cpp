//
// Created by Lingyin on 2024/4/6.
//

#include "Mesh.h"


/*
void Mesh::loadGTA(const std::string &address) {

    std::ifstream in;
    in.open (address, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    while (!in.eof()) {
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) {
            iss >> trash;
            vec3 v;
            for (int i=0;i<3;i++) iss >> v.e[i];
            vertexSpace.push_back(v);
        }
        else if (!line.compare(0, 2, "f ")) {
            std::vector<std::array<int, 3> > f;
            int itrash, idx;
            iss >> trash;
            while (iss >> idx >> trash >> itrash >> trash >> itrash) {
                idx--; // in wavefront obj all indices start at 1, not zero
                f.push_back({idx, -1, -1});
            }
            faces.push_back(f);
        }
    }
    std::cerr << "# v# " << vertexSpace.size() << " f# "  << faces.size() << std::endl;
}
*/

int read(){
    int x = 0;
    static char c = ' ';
    while(!('0' <= c && c <= '9')){
        if(c == '\n'){
            c = ' ';
            return -1;
        }

        c = getchar();
    }
    while('0' <= c && c <= '9'){
        x = x * 10 + c - '0';
        c = getchar();
    }
    return x;
}

vector<float > toFloat(string s){
    std::stringstream ss(s);
    float floatNum;
    vector<float > tmpVec;
    while(ss >> floatNum){
        tmpVec.push_back(floatNum);
    }

    return tmpVec;
}

using std::cin;
void Mesh::loadOBJ(const string& path, const string& name){

//    path = path + "\\" + name;
    string s;
    string();

    // read from .obj file
    freopen((path + "/" + name + ".obj").c_str(), "r", stdin);


    string mtlName = "__default";
    while(cin >> s){
        if(s == "usemtl"){
            cin >> mtlName;
        }
        if(s == "v"){
            std::string line;
            std::getline(std::cin, line);
            auto tmpVec = toFloat(line);
            assert(tmpVec.size() >= 3);
            vertexSpace.emplace_back(tmpVec[0], tmpVec[1], tmpVec[2]);

        }
        else if(s == "vt"){
            std::string line;
            std::getline(std::cin, line);
            auto tmpVec = toFloat(line);
            assert(tmpVec.size() >= 2);
            vertexTexture.emplace_back(tmpVec[0], tmpVec[1]);
        }
        else if(s == "vn"){
            std::string line;
            std::getline(std::cin, line);
            auto tmpVec = toFloat(line);
            assert(tmpVec.size() >= 3);
            vertexNormal.emplace_back(tmpVec[0], tmpVec[1], tmpVec[2]);
        }
        else if(s == "f"){
            vector<int > vec;
            while(true){
                vec.push_back(read());
                if(vec.back() == -1){
                    vec.pop_back();
                    break;
                }
            }

            assert(vec.size() % 3 == 0);
            Face face;
            face.materialName = mtlName;
            // v vt vn
            for(int i = 0; i < vec.size(); i += 3){
                int v = vec[i] - 1, vt = vec[i + 1] - 1, vn = vec[i + 2] - 1;
                face.points.push_back({v, vt, vn});
            }
            faces.push_back(face);
        }
        else{
            string line;
            getline(cin, line);
        }
    }
    cin.clear();

    if(vertexTexture.empty()){
        vertexTexture.resize(1);
    }

    /*
    // read from .mtl file
    auto opend = freopen((path + "\\" + name + ".mtl").c_str(), "r", stdin);

    while(cin >> s){
        if(s == "Ka"){
            float a, b, c;
            cin >> a >> b >> c;
            ret.publicInfoVec4["Ka"] = Vec4({a, b, c, 1});
        }
        else if(s == "Kd"){
            float a, b, c;
            cin >> a >> b >> c;
            ret.publicInfoVec4["Kd"] = Vec4({a, b, c, 1});
        }
        else if(s == "Ks"){
            float a, b, c;
            cin >> a >> b >> c;
            ret.publicInfoVec4["Ks"] = Vec4({a, b, c, 1});
        }
        else if(s == "Ns"){
            float a;
            cin >> a;
            ret.publicInfoFloat["Ns"] = a;
        }
        else if(s == "d"){
            float a;
            cin >> a;
            ret.publicInfoFloat["d"] = a;
        }
        else if(s == "illum"){
            float a;
            cin >> a;
            ret.publicInfoFloat["illum"] = a;
        }
        else{
            getline(cin, line);
        }
    }
    cin.clear();

    ret.fragmentFunction = cubeFrament;
    ret.vertexFunction = cubeVertex;

//    ret.vertexCount = ret.vertexPosition.size();
    freopen("con", "r", stdin);


    return ret;
    */
}
