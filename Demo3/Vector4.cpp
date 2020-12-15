//
// Created by gaozhanheng on 2019/11/15.
//

#include "stdafx.h"

#include <iostream>
#include "Vector4.h"
using namespace std;

Vector4::Vector4() {
    this->data[0] = this->data[1] = this->data[2] = this->data[3] = 0;
}

Vector4::Vector4(float data[])
{
    this->data[0] = data[0];
    this->data[1] = data[1];
    this->data[2] = data[2];
    this->data[3] = data[3];
}
Vector4::~Vector4()
{
}

void Vector4::dividedByW()
{
    if (data[3] == 0)
        return ;

    x() /= w();
    y() /= w();
    z() /= w();
    w() = 1;

}
float& Vector4::x()
{
    return data[0];
}
float& Vector4::y()
{
    return data[1];
}
float& Vector4::z()
{
    return data[2];
}
float& Vector4::w()
{
    return data[3];
}

ostream& operator << (ostream& out, const Vector4& vector)
{
    out << vector.data[0] << " " << vector.data[1] << " " << vector.data[2] << " " << vector.data[3];

    return out;
}
