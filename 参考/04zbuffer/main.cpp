#include <vector>
#include <cmath>
#include <cstdlib>
#include <limits>
#include "tgaimage.h"
#include "model.h"
#include "geometry.h"
#include <algorithm>

//�������
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
Model *model = NULL;
const int width  = 800;
const int height = 800;

//�����㷨
void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color) {
    bool steep = false;
    if (std::abs(x0-x1)<std::abs(y0-y1)) {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0>x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    for (int x=x0; x<=x1; x++) {
        float t = (x-x0)/(float)(x1-x0);
        int y = y0*(1.-t) + y1*t;
        if (steep) {
            image.set(y, x, color);
        } else {
            image.set(x, y, color);
        }
    }
}

//������������
Vec3f barycentric(Vec3f A, Vec3f B, Vec3f C, Vec3f P) {
    Vec3f s[2];
    //����[AB,AC,PA]��x��y����
    for (int i=2; i--; ) {
        s[i][0] = C[i]-A[i];
        s[i][1] = B[i]-A[i];
        s[i][2] = A[i]-P[i];
    }
    //[u,v,1]��[AB,AC,PA]��Ӧ��x��y��������ֱ�����Բ��
    Vec3f u = cross(s[0], s[1]);
    //���㹲��ʱ���ᵼ��u[2]Ϊ0����ʱ����(-1,1,1)
    if (std::abs(u[2])>1e-2)
        //��1-u-v��u��vȫΪ����0��������ʾ�����������ڲ�
        return Vec3f(1.f-(u.x+u.y)/u.z, u.y/u.z, u.x/u.z);
    return Vec3f(-1,1,1);
}

//����������(�������飬zbufferָ�룬tgaָ�룬��ɫ)
void triangle(Vec3f *pts, float *zbuffer, TGAImage &image, TGAColor color) {
    Vec2f bboxmin( std::numeric_limits<float>::max(),  std::numeric_limits<float>::max());
    Vec2f bboxmax(-std::numeric_limits<float>::max(), -std::numeric_limits<float>::max());
    Vec2f clamp(image.get_width()-1, image.get_height()-1);
    //ȷ�������εı߿�
    for (int i=0; i<3; i++) {
        for (int j=0; j<2; j++) {
            bboxmin[j] = std::max(0.f,      std::min(bboxmin[j], pts[i][j]));
            bboxmax[j] = std::min(clamp[j], std::max(bboxmax[j], pts[i][j]));
        }
    }
    Vec3f P;
    //�����߿��е�ÿһ����
    for (P.x=bboxmin.x; P.x<=bboxmax.x; P.x++) {
        for (P.y=bboxmin.y; P.y<=bboxmax.y; P.y++) {
            //��������
            if (P.x > 600 && P.y > 500)
            {
                P.x += 0.01;
            }
            Vec3f bc_screen  = barycentric(pts[0], pts[1], pts[2], P);
            //����������һ����ֵ��˵��������������
            if (bc_screen.x<0 || bc_screen.y<0 || bc_screen.z<0) continue;
            P.z = 0;
            //����zbuffer
            for (int i=0; i<3; i++) P.z += pts[i][2]*bc_screen[i];
            if (zbuffer[int(P.x+P.y*width)]<P.z) {
                zbuffer[int(P.x+P.y*width)] = P.z;
                image.set(P.x, P.y, color);
            }
        }
    }
}

//��������ת��Ļ����
Vec3f world2screen(Vec3f v) {
    return Vec3f(int((v.x+1.)*width/2.+.5), int((v.y+1.)*height/2.+.5), v.z);
}

int main(int argc, char** argv) {
    //��ȡģ���ļ�
    if (2==argc) {
        model = new Model(argv[1]);
    } else {
        model = new Model("obj/african_head/african_head.obj");
    }
    //����zbuffer����СΪ������С
    float *zbuffer = new float[width*height];
    //��ʼ��zbuffer���趨һ����С��ֵ
    for (int i=width*height; i--; zbuffer[i] = -std::numeric_limits<float>::max());
    TGAImage image(width, height, TGAImage::RGB);
    //�趨���շ���
    Vec3f light_dir(0, 0, -1);
    for (int i=0; i<model->nfaces(); i++) {
        //��Ļ���꣬��������
        Vec3f screen_coords[3];
        Vec3f world_coords[3];
        std::vector<int> face = model->face(i);
        for (int j = 0; j < 3; j++) {
            Vec3f v = model->vert(face[j]);
            //��������ת��Ļ����
            screen_coords[j] = world2screen(model->vert(face[j]));
            world_coords[j] = v;
        }
        //�����������ڼ��㷨����
        Vec3f n = cross((world_coords[2] - world_coords[0]),(world_coords[1] - world_coords[0]));
        n.normalize();
        float intensity = n * light_dir;
        //����ü�
        if (intensity > 0) {
            triangle(screen_coords, zbuffer, image, TGAColor(intensity * 255, intensity * 255, intensity * 255, 255));
        }
    }

    image.flip_vertically();
    image.write_tga_file("output.tga");
    delete model;
    return 0;
}

