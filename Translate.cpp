//
// Created by Lingyin on 2024/4/6.
//

#include "Translate.h"

Translate::Translate()
        : position(0, 0, 0, 1),
        aim(0, 0, -1, 1),
        up(0, 1, 0, 0), raito(1) {}


Mat4 Translate::getTranslateMat() const {

        const auto e = position;
        const auto g = (aim - position).normal();
        const auto t = up.normal();
        const auto v = cross(g, t).normal();

        Mat4 rr({
            raito, 0, 0, 0,
            0, raito, 0, 0,
            0, 0, raito, 0,
            0, 0, 0, 1,
        });

        Mat4 R({
                       v.x(), t.x(), -g.x(), e.x(),
                       v.y(), t.y(), -g.y(), e.y(),
                       v.z(), t.z(), -g.z(), e.z(),
                       0, 0, 0, 1
               });

        return R * rr;
}

Mat4 Translate::getInvTranslateMat() const{

    const auto e = position;
    const auto g = (aim - position).normal();
    const auto t = up.normal();
    const auto gt = cross(g, t).normal();

    Mat4 T({
        1, 0, 0, -e.x(),
        0, 1, 0, -e.y(),
        0, 0, 1, -e.z(),
        0, 0, 0, 1
    });


    Mat4 R({
        gt.x(), gt.y(), gt.z(), 0,
        t.x(), t.y(), t.z(), 0,
        -g.x(), -g.y(), -g.z(), 0,
        0, 0, 0, 1
    });

    return R * T;
}

void Translate::add(float x, float y){

    const auto toWards = (aim - position);
    const auto xRight = cross(toWards, up).normal();
    const auto yRight = up.normal();

    float dis = toWards.mo();
    auto relativePosition = position + y * yRight + x * xRight - aim;
    relativePosition = dis * relativePosition.normal();
    position = aim + relativePosition;

    const auto & newToWards = (aim - position);
    up = cross(xRight, newToWards).normal();
}

void Translate::addX(float x){

    const auto toWards = (aim - position);
    const auto xRight = cross(toWards, up).normal();

    float dis = toWards.mo();
    auto relativePosition = position + x * xRight - aim;
    relativePosition = dis * relativePosition.normal();
    position = aim + relativePosition;
}

void Translate::addY(float y){
    const auto toWards = (aim - position);
    const auto xRight = cross(toWards, up).normal();
    const auto yRight = up.normal();

    float dis = toWards.mo();
    auto relativePosition = position + y * yRight - aim;
    relativePosition = dis * relativePosition.normal();
    position = aim + relativePosition;
    const auto & newToWards = (aim - position);
    up = cross(xRight, newToWards).normal();
}

void Translate::goInto(float d){

    float dis = (aim - position).mo();
    dis = std::min(dis - myEps * 10, d);
    position += (aim - position).normal() * dis;
}

void Translate::setPosition(float x, float y, float z){
    position = Vec4(x, y, z, 1);
    const auto toWards = aim - position;
    const auto tmp = cross(toWards, Vec4(0, 1, 0, 0));
    up = cross(toWards, tmp).normal();
    if(up.y() < 0){
        up = -up;
    }
}