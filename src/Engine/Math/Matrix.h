/* 
 * File:   Matrix.h
 * Author: jsmtux
 *
 * Created on September 25, 2012, 9:10 PM
 */

#ifndef MATRIX_H
#define	MATRIX_H
#include <iostream>
#include "Vector3.h"
#include <assimp/Importer.hpp>
using namespace std;

class Vector3;

class Matrix{
public:
    float m[4][4];
    Matrix();
    Matrix(const aiMatrix4x4& AssimpMatrix);
    Matrix(const aiMatrix3x3& AssimpMatrix);
    Matrix operator*(const Matrix& Right) const;
    Vector3 operator*(const Vector3& Right) const;
    Matrix inverted();
    bool operator==(const Matrix& Right);
    friend ostream& operator<<(ostream& os,const Matrix& m);
    void setZero();
    void setIdentity();
    void setScaleTransform(float ScaleX, float ScaleY, float ScaleZ);
    void setRotateTransform(float RotateX, float RotateY, float RotateZ);
    void setTranslationTransform(float x, float y, float z);
    Matrix& inverse();
    Matrix transposed();
    float determinant() const;
};

#endif	/* MATRIX_H */

