//
// Created by gaozhanheng on 2019/11/15.
//

#ifndef DEMO3_VECTOR4_H
#define DEMO3_VECTOR4_H

#include <iostream>
using namespace std;

class Vector4 {

public:
    Vector4();
    Vector4(float data[]);
    ~Vector4();

public:
    void dividedByW();
    float& x();
    float& y();
    float& z();
    float& w();

    friend Vector4 operator* (const Vector4& vector,const class TransMatrix& matrix);
    friend ostream& operator << (ostream& , const Vector4& vector);

protected:


private:
    float data[4];

};


#endif //DEMO3_VECTOR4_H
