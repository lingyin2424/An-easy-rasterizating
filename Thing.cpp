//
// Created by Lingyin on 2024/4/6.
//
#include "Thing.h"



Vec3 readVec3(){
    float a, b, c;
    std::cin >> a >> b >> c;
    return Vec3(a, b, c);
}

void readTextureVec4(Texture<Vec4> &t, string name){

    // trans

    string nameType = name.substr(name.length() - 4);
    if(nameType[0] == '.'){
        name[name.length() - 3] = 't';
        name[name.length() - 2] = 'g';
        name[name.length() - 1] = 'a';
    }

    std::cout << name << '\n';


    TGAImage img;
    std::cout << (img.read_tga_file(name.c_str()) ? "read ok" : "read false") << std::endl;
    int w = img.get_width();
    int h = img.get_height();
    std::cout << w << ' ' << h << '\n';
    t.resize(w, h);
    auto p = sizeof (t.textureMap);
    const float inv255 = 1.f / 255.f;
    for(int i = 0; i < w; i ++){
        for(int j = 0; j < h; j ++){
            const auto&c = img.get(i, h - j - 1);
            t.get(i, j) = Vec4(c.r, c.g, c.b, c.a) * inv255;
        }
    }
}

static float pp(const float& x){
    return x * x;
}

void Thing::load(const string& path, const string& name){

    std::map<string, int > materialId;

    // read mtl

    {
        using std::cin;
        cin.clear();


        std::cout << (path + "/" + name + ".mtl") << '\n';
        freopen((path + "/" + name + ".mtl").c_str(), "r", stdin);
        string s, line;
        while(cin >> s){
            if(s == "#"){
                getline(cin, line);
            }
            else if(s == "newmtl"){
                cin >> line;
                materials.emplace_back();
                materialId[line] = (int)materials.size() - 1;
            }
            else{
                auto& m = materials.back();
                if(s == "Ns"){
                    cin >> m.Ns;
                }
                else if(s == "Ni"){
                    cin >> m.Ni;
                }
                else if(s == "d"){
                    cin >> m.d;
                }
                else if(s == "illum"){
                    cin >> m.illum;
                }
                else if(s == "Ka"){
                    m.Ka = Vec4(readVec3(), 0);
                }
                else if(s == "Kd"){
                    m.Kd = Vec4(readVec3(), 0);
                }
                else if(s == "Ks"){
                    m.Ks = Vec4(readVec3(), 0);
                }
                else if(s == "Ke"){
                    m.Ke = Vec4(readVec3(), 0);
                }
                else if(s == "map_Kd"){
                    cin >> line;
                    m.havemap_Kd = true;
                    readTextureVec4(m.map_Kd, path + "/" + line);
                }
                else if(s == "map_Ka"){
                    cin >> line;
                    m.havemap_Ka = true;
                    readTextureVec4(m.map_Ka, path + "/" + line);
                }
                else if(s == "map_Ks"){
                    cin >> line;
                    m.havemap_Ks = true;
                    readTextureVec4(m.map_Ks, path + "/" + line);
                }
                else if(s == "map_bump"){
                    cin >> line;
                    m.havemap_bump = true;
                    readTextureVec4(m.map_bump, path + "/" + line);
                }
            }
        }
        cin.clear();
    }





    mesh.loadOBJ(path, name);

    for(auto& face : mesh.faces){
        const auto& name = face.materialName;
        if(name == "__default"){
            if(materialId.find(name) == materialId.end()){
                materials.emplace_back();
                materialId[name] = materials.size() - 1;
            }
        }
        assert(materialId.find(name) != materialId.end());
        face.materialId = materialId[name];
    }

    float xMin = 1e9, xMax = -1e9;
    float yMin = 1e9, yMax = -1e9;
    float zMin = 1e9, zMax = -1e9;

    for(const auto& v: this -> mesh.vertexSpace){
        xMin = min(xMin, v.x());
        xMax = max(xMax, v.x());
        yMin = min(yMin, v.y());
        yMax = max(yMax, v.y());
        zMin = min(zMin, v.z());
        zMax = max(zMax, v.z());
    }


    aabb[0] = Vec4(xMin, yMin, zMin, 1);
    aabb[1] = Vec4(xMin, yMin, zMax, 1);
    aabb[2] = Vec4(xMin, yMax, zMin, 1);
    aabb[3] = Vec4(xMin, yMax, zMax, 1);
    aabb[4] = Vec4(xMax, yMin, zMin, 1);
    aabb[5] = Vec4(xMax, yMin, zMax, 1);
    aabb[6] = Vec4(xMax, yMax, zMin, 1);
    aabb[7] = Vec4(xMax, yMax, zMax, 1);

//    material.loadMTL(path, name);
}