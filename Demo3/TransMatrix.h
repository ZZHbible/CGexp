//
// Created by gaozhanheng on 2019/11/15.
//

#ifndef DEMO3_TRANSMATRIX_H
#define DEMO3_TRANSMATRIX_H

#include "Vector4.h"

class TransMatrix
{
public:
    TransMatrix();
    TransMatrix(float data[][4]);// row by row
    ~TransMatrix();

public:
    static TransMatrix zeros();
    static TransMatrix identity();

    static TransMatrix translate(float tx,float ty,float tz);
    static TransMatrix rotate(int axis,float degree);
    static TransMatrix scale(float sx,float sy,float sz);

    static TransMatrix parallel_project();// xie er ce
    static TransMatrix perspective_project(float cx,float cy,float cz);// let z be the projection plane

public:
    friend TransMatrix operator*(const TransMatrix& matrix1,const TransMatrix& matrix2);
    friend Vector4 operator* (const Vector4& vector,const TransMatrix& matrix);
    friend ostream& operator <<(ostream& out, const TransMatrix& matrix);

protected:

private:
    float data[4][4];
};

#endif //DEMO3_TRANSMATRIX_H
