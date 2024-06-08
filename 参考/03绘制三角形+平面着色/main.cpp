#include <vector>
#include <cmath>
#include "tgaimage.h"
#include "model.h"
#include "geometry.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green = TGAColor(0,   255, 0,   255);
Model *model = NULL;
const int width  = 800;
const int height = 800;

//�����㷨(����1������2��tgaָ�룬��ɫ)
void line(Vec2i p0, Vec2i p1, TGAImage &image, TGAColor color) {
    bool steep = false;
    if (std::abs(p0.x-p1.x)<std::abs(p0.y-p1.y)) {
        std::swap(p0.x, p0.y);
        std::swap(p1.x, p1.y);
        steep = true;
    }
    if (p0.x>p1.x) {
        std::swap(p0, p1);
    }

    for (int x=p0.x; x<=p1.x; x++) {
        float t = (x-p0.x)/(float)(p1.x-p0.x);
        int y = p0.y*(1.-t) + p1.y*t;
        if (steep) {
            image.set(y, x, color);
        } else {
            image.set(x, y, color);
        }
    }
}

//����������(����1������2������3��tgaָ�룬��ɫ)
void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor color) {
    //���������Ϊ0
    if (t0.y==t1.y && t0.y==t2.y) return;
    //����y�Ĵ�С�������������
    if (t0.y>t1.y) std::swap(t0, t1);
    if (t0.y>t2.y) std::swap(t0, t2);
    if (t1.y>t2.y) std::swap(t1, t2);
    int total_height = t2.y-t0.y;
    //�Ը߶Ȳ���Ϊѭ�����Ʊ�������ʱ����Ҫ����б�ʣ���Ϊ��ɫ���ÿ�ж��ᱻ���
    for (int i=0; i<total_height; i++) {
        //����t1�������ηָ�Ϊ����������
        bool second_half = i>t1.y-t0.y || t1.y==t0.y;
        int segment_height = second_half ? t2.y-t1.y : t1.y-t0.y;
        float alpha = (float)i/total_height;
        float beta  = (float)(i-(second_half ? t1.y-t0.y : 0))/segment_height; 
        //����A,B���������
        Vec2i A =               t0 + (t2-t0)*alpha;
        Vec2i B = second_half ? t1 + (t2-t1)*beta : t0 + (t1-t0)*beta;
        if (A.x>B.x) std::swap(A, B);
        //����A,B�͵�ǰ�߶ȶ�tga��ɫ
        for (int j=A.x; j<=B.x; j++) {
            image.set(j, t0.y+i, color);
        }
    }
}

int main(int argc, char** argv) {
    //��ȡmodel�ļ�
    if (2==argc) {
        model = new Model(argv[1]);
    } else {
        model = new Model("obj/african_head.obj");
    }
    //����tgaimage
    TGAImage image(width, height, TGAImage::RGB);
    //�������ɫ
    //ָ�����շ���
    Vec3f light_dir(0,0,-1);
    //ģ�͵�����Ϊѭ�����Ʊ���
    for (int i=0; i<model->nfaces(); i++) {
        //face��һ�����飬���ڴ洢һ�������������
        std::vector<int> face = model->face(i);
        Vec2i screen_coords[3];
        Vec3f world_coords[3];
        for (int j=0; j<3; j++) {
            Vec3f v = model->vert(face[j]);
            //��Ļ����    (-1,-1)ӳ��Ϊ(0,0)  ��1,1��ӳ��Ϊ(width,height)
            screen_coords[j] = Vec2i((v.x+1.)*width/2., (v.y+1.)*height/2.);
            //��������    ��ģ������
            world_coords[j]  = v;
        }
        //������������㷨����
        Vec3f n = (world_coords[2]-world_coords[0])^(world_coords[1]-world_coords[0]);
        n.normalize();
        //ǿ��=������*���շ���   ���������͹��շ����غ�ʱ���������
        float intensity = n*light_dir;
        //ǿ��С��0��˵��ƽ�泯��Ϊ��  ������ü�
        if (intensity>0) {
            triangle(screen_coords[0], screen_coords[1], screen_coords[2], image, TGAColor(intensity*255, intensity*255, intensity*255, 255));
        }
    }

    image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
    image.write_tga_file("output.tga");
    delete model;
    return 0;
}

