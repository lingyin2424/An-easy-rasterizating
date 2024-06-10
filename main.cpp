#include <filesystem>
#include <cmath>
#include <cassert>
#include <iostream>

#include "tgaimage.h"

#include "math/Vec.h"
#include "math/Mat.h"
#include "Mesh.h"
#include "Thing.h"
#include "Render.h"
#include<iomanip>

#include "Texture.h"

long long totTim = 0;
using std::swap;

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);

void drawLine(int x0, int y0, int x1, int y1, TGAImage &image, const TGAColor& color) {

    if(abs(x0 - x1) >= abs(y0 - y1)){
        if(x0 > x1){
            swap(x0, x1);
            swap(y0, y1);
        }
        int len = x1 - x0 + 1;
        for(int x = x0; x <= x1; x ++){
            int y = (y0 * (x1 - x) + y1 * (x - x0 + 1)) / len;
            image.set(x, y, color);
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
            image.set(x, y, color);
        }
    }
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {

    Render render;


//    model1.mesh.loadOBJ(R"(C:\Users\Lingyin\CLionProjects\aSmallRender\obj)", "testObj");
//    model1.mesh.loadOBJ(R"(C:\Users\Lingyin\CLionProjects\aSmallRender\obj)", "african_head");
   // model1.load(R"(C:\Users\Lingyin\CLionProjects\aSmallRender\obj)", "202");

//    model1.mesh.loadOBJ(R"(C:\Users\Lingyin\CLionProjects\aSmallRender\obj)", "diablo3_pose");

//    model1.mesh.loadOBJ(R"(C:\Users\Lingyin\CLionProjects\aSmallRender\obj)", "floor");

    Thing model1;
    model1.load(R"(../obj)", "floor");
    model1.translate.position = Vec4(0, 1, 0, 1);
    model1.translate.aim = Vec4(0, 1, -1, 1);
    model1.translate.raito = 5;
    model1.floor = true;

    Thing model2;
    model2.load(R"(../obj)", "202");
    model2.translate.position = Vec4(0, -4, 0, 1);
    model2.translate.aim = Vec4(0, -4, -1, 1);
    model2.translate.raito = 1;
    model2.floor = false;


    Light lig;

    lig.intersity = 500*250*1000;
    lig.color = Vec4(255, 255, 255, 0);
    lig.translate.aim = Vec4(0, -4, 0, 1);
    lig.translate.setPosition(0, 15, 15);
    // lig.translate.setPosition(0, -4, 10);
    lig.color = Vec4(1, 1, 0.9, 0);
    render.addLight(lig);
    render.addThing(model1);
    render.addThing(model2);


    render.camera.translate.position = Vec4(0, 0, 3, 0);
    render.camera.width = 2;
    render.camera.height = 2;
    render.camera.zNear = -1;
    render.camera.zFar = -100;

    const int N = 700;
    render.scene.setSize(N, N);

    TGAImage image(N, N, TGAImage::RGB);
    for(int i = 10; i <= 100; i += 1){


//        render.camera.translate.position = Vec4(0.02 * i, 0, 0.02 * i, 0);


        render.camera.translate.aim = Vec4(0, -4, 0, 0);
        render.camera.translate.setPosition(1, (float)i * 0.1f - 4, 1); // 43


    //   render.camera.translate.aim = Vec4(0, -2, 0, 0);
    //   render.camera.translate.setPosition(2, (float)i * 0.1f - 2, 1);// 2



//        render.camera.translate.aim = Vec4(0, -4, 0, 0);
//        render.camera.translate.setPosition(1, 10 * 0.1f - 4, 1);
//        render.camera.translate.goInto(0.03f * i);



        std::cout << i << ' ' << render.camera.translate.up << '\n';


    //    render.rasterizating();
        render.drawLine();

//

/*
        freopen("outputL.txt", "w", stdout);
        std::cout << std::fixed << std::setprecision(2);
        for(int i = 0; i < N; i ++){
            for(int j = 0; j < N; j ++){
                const auto c = render.scene.colorBuffer[i][j].color255();
                std::cout << c.x() << ' ' << c.y() << ' ' << c.z() << "  ";
            }
            std::cout << '\n';
        }
*/
        
        for(int ii = 0; ii < N; ii ++){
            for(int jj = 0; jj < N; jj ++){
                const auto c = render.scene.colorBuffer(ii, jj).color255();
                image.set(jj, ii, TGAColor((char)c.x(), (char)c.y(), (char)c.z(), 255));
            }
        }


        image.flip_vertically();
        string name = "output" + std::to_string(i) + ".tga";
        image.write_tga_file(name.c_str());
        //        image.write_tga_file("output.tga");

    }


    return 0;


}

