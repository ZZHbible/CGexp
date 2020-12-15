//
// Created by gaozhanheng on 2019/11/15.
//

#include "stdafx.h"

#include "TransMatrix.h"

TransMatrix::TransMatrix()// row by row
{
    for (int r = 0; r < 4; ++ r)
    {
        for (int c = 0; c < 4; ++ c)
        {
            this->data[r][c] = 0.;
        }
    }
	for (int k = 0; k < 4; ++k)
	{
		this->data[k][k] = 1.;
	}
}

TransMatrix::TransMatrix(float data[][4])// row by row
{
    for (int r = 0; r < 4; ++ r)
    {
        for (int c = 0; c < 4; ++ c)
        {
            this->data[r][c] = data[r][c];
        }
    }
}
TransMatrix::~TransMatrix()
{

}

TransMatrix TransMatrix::zeros()
{
    TransMatrix mat;
	mat.data[0][0] = mat.data[1][1] = mat.data[2][2] = mat.data[3][3] = 0.;
    return mat;
}

TransMatrix TransMatrix::identity()
{
    TransMatrix mat;
    mat.data[0][0] = 1.;
    mat.data[1][1] = 1.;
    mat.data[2][2] = 1.;
    mat.data[3][3] = 1.;

    return mat;
}



TransMatrix TransMatrix::translate(float tx,float ty,float tz)
{
    TransMatrix mat = identity();
    mat.data[3][0] = tx;
    mat.data[3][1] = ty;
    mat.data[3][2] = tz;

    return mat;
}


TransMatrix TransMatrix::rotate(int axis,float degree)
{
    TransMatrix mat = identity();
    switch (axis)
    {
        case 0:// x

            break;
        case 1:// y

            break;
        case 2:// z

            break;
    }

    return mat;
}



TransMatrix TransMatrix::scale(float sx,float sy,float sz)
{
    TransMatrix mat = identity();
    mat.data[0][0] = sx;
    mat.data[1][1] = sy;
    mat.data[2][2] = sz;

    return mat;
}

TransMatrix TransMatrix::parallel_project()// xie er ce
{
    TransMatrix mat = identity();

	

    return mat;
}
TransMatrix TransMatrix::perspective_project(float cx,float cy,float cz)// let z be the projection plane
{
    TransMatrix mat = identity();


    return mat;
}

TransMatrix operator*(const TransMatrix& matrix1,const TransMatrix& matrix2)
{
    TransMatrix mat = TransMatrix::zeros();

    for (int r = 0; r < 4; ++ r)
    {
        for (int c = 0; c < 4; ++ c)
        {
            for (int k = 0; k < 4; ++ k) {
                mat.data[r][c] += matrix1.data[r][k] * matrix2.data[k][c];
            }
        }
    }

    return mat;
}
Vector4 operator* (const Vector4& vector,const TransMatrix& matrix)
{
    Vector4 vec;

    for (int k = 0; k < 4; ++ k)
    {
        for (int kk = 0; kk < 4; ++ kk)
        {
            vec.data[k] += vector.data[kk] * matrix.data[kk][k];
        }
    }

    return vec;
}

ostream& operator <<(ostream& cout, const TransMatrix& matrix)
{
    for (int r = 0; r < 4; ++ r)
    {
        for (int c = 0; c < 4; ++ c)
        {
            cout << matrix.data[r][c] << " ";
        }
        cout << endl;
    }

    return cout;
}