#include <iostream>

using namespace std;

#include "Vector4.h"
#include "TransMatrix.h"

int main() {

    cout << "Testing vector" << endl;
    Vector4 vec;
    vec.x() = 1.;
    vec.w() = 2.;
    cout << vec << endl;
    vec.dividedByW();
    cout << vec << endl;

    TransMatrix mat,mat2;
    cout << "Testing matrix" << endl;
    mat = TransMatrix::identity();
    mat = TransMatrix::translate(1,2,3);
    mat2 = TransMatrix::scale(2,3,4);
//    mat = TransMatrix::rotate(0,10);
    cout << mat * mat2;

    cout << mat;


    return 0;
}