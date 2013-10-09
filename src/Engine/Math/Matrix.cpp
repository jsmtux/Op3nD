#include "Matrix.h"
#include <assimp/Importer.hpp>
#include <cmath>

Matrix::Matrix(){
    setIdentity();
}

Matrix::Matrix(const aiMatrix4x4& AssimpMatrix){
    m[0][0] = AssimpMatrix.a1; m[0][1] = AssimpMatrix.a2; m[0][2] = AssimpMatrix.a3; m[0][3] = AssimpMatrix.a4;
    m[1][0] = AssimpMatrix.b1; m[1][1] = AssimpMatrix.b2; m[1][2] = AssimpMatrix.b3; m[1][3] = AssimpMatrix.b4;
    m[2][0] = AssimpMatrix.c1; m[2][1] = AssimpMatrix.c2; m[2][2] = AssimpMatrix.c3; m[2][3] = AssimpMatrix.c4;
    m[3][0] = AssimpMatrix.d1; m[3][1] = AssimpMatrix.d2; m[3][2] = AssimpMatrix.d3; m[3][3] = AssimpMatrix.d4;
}

Matrix::Matrix(const aiMatrix3x3& AssimpMatrix){
    m[0][0] = AssimpMatrix.a1; m[0][1] = AssimpMatrix.a2; m[0][2] = AssimpMatrix.a3; m[0][3] = 0.0f;
    m[1][0] = AssimpMatrix.b1; m[1][1] = AssimpMatrix.b2; m[1][2] = AssimpMatrix.b3; m[1][3] = 0.0f;
    m[2][0] = AssimpMatrix.c1; m[2][1] = AssimpMatrix.c2; m[2][2] = AssimpMatrix.c3; m[2][3] = 0.0f;
    m[3][0] = 0.0f           ; m[3][1] = 0.0f           ; m[3][2] = 0.0f           ; m[3][3] = 1.0f;
} 

float Matrix::determinant() const
{
	return m[0][0]*m[1][1]*m[2][2]*m[3][3] - m[0][0]*m[1][1]*m[2][3]*m[3][2] + m[0][0]*m[1][2]*m[2][3]*m[3][1] - m[0][0]*m[1][2]*m[2][1]*m[3][3] 
		+ m[0][0]*m[1][3]*m[2][1]*m[3][2] - m[0][0]*m[1][3]*m[2][2]*m[3][1] - m[0][1]*m[1][2]*m[2][3]*m[3][0] + m[0][1]*m[1][2]*m[2][0]*m[3][3] 
		- m[0][1]*m[1][3]*m[2][0]*m[3][2] + m[0][1]*m[1][3]*m[2][2]*m[3][0] - m[0][1]*m[1][0]*m[2][2]*m[3][3] + m[0][1]*m[1][0]*m[2][3]*m[3][2] 
		+ m[0][2]*m[1][3]*m[2][0]*m[3][1] - m[0][2]*m[1][3]*m[2][1]*m[3][0] + m[0][2]*m[1][0]*m[2][1]*m[3][3] - m[0][2]*m[1][0]*m[2][3]*m[3][1] 
		+ m[0][2]*m[1][1]*m[2][3]*m[3][0] - m[0][2]*m[1][1]*m[2][0]*m[3][3] - m[0][3]*m[1][0]*m[2][1]*m[3][2] + m[0][3]*m[1][0]*m[2][2]*m[3][1]
		- m[0][3]*m[1][1]*m[2][2]*m[3][0] + m[0][3]*m[1][1]*m[2][0]*m[3][2] - m[0][3]*m[1][2]*m[2][0]*m[3][1] + m[0][3]*m[1][2]*m[2][1]*m[3][0];
}

Matrix& Matrix::inverse()
{
	// Compute the reciprocal determinant
	float det = determinant();
	if(det == 0.0f) 
	{
		// Matrix not invertible. Setting all elements to nan is not really
		// correct in a mathematical sense but it is easy to debug for the
		// programmer.
		/*const float nan = std::numeric_limits<float>::quiet_NaN();
		*this = Matrix4f(
			nan,nan,nan,nan,
			nan,nan,nan,nan,
			nan,nan,nan,nan,
			nan,nan,nan,nan);*/
        assert(0);
		return *this;
	}

	float invdet = 1.0f / det;

	Matrix res;
	res.m[0][0] = invdet  * (m[1][1] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) + m[1][2] * (m[2][3] * m[3][1] - m[2][1] * m[3][3]) + m[1][3] * (m[2][1] * m[3][2] - m[2][2] * m[3][1]));
	res.m[0][1] = -invdet * (m[0][1] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) + m[0][2] * (m[2][3] * m[3][1] - m[2][1] * m[3][3]) + m[0][3] * (m[2][1] * m[3][2] - m[2][2] * m[3][1]));
	res.m[0][2] = invdet  * (m[0][1] * (m[1][2] * m[3][3] - m[1][3] * m[3][2]) + m[0][2] * (m[1][3] * m[3][1] - m[1][1] * m[3][3]) + m[0][3] * (m[1][1] * m[3][2] - m[1][2] * m[3][1]));
	res.m[0][3] = -invdet * (m[0][1] * (m[1][2] * m[2][3] - m[1][3] * m[2][2]) + m[0][2] * (m[1][3] * m[2][1] - m[1][1] * m[2][3]) + m[0][3] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]));
	res.m[1][0] = -invdet * (m[1][0] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) + m[1][2] * (m[2][3] * m[3][0] - m[2][0] * m[3][3]) + m[1][3] * (m[2][0] * m[3][2] - m[2][2] * m[3][0]));
	res.m[1][1] = invdet  * (m[0][0] * (m[2][2] * m[3][3] - m[2][3] * m[3][2]) + m[0][2] * (m[2][3] * m[3][0] - m[2][0] * m[3][3]) + m[0][3] * (m[2][0] * m[3][2] - m[2][2] * m[3][0]));
	res.m[1][2] = -invdet * (m[0][0] * (m[1][2] * m[3][3] - m[1][3] * m[3][2]) + m[0][2] * (m[1][3] * m[3][0] - m[1][0] * m[3][3]) + m[0][3] * (m[1][0] * m[3][2] - m[1][2] * m[3][0]));
	res.m[1][3] = invdet  * (m[0][0] * (m[1][2] * m[2][3] - m[1][3] * m[2][2]) + m[0][2] * (m[1][3] * m[2][0] - m[1][0] * m[2][3]) + m[0][3] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]));
	res.m[2][0] = invdet  * (m[1][0] * (m[2][1] * m[3][3] - m[2][3] * m[3][1]) + m[1][1] * (m[2][3] * m[3][0] - m[2][0] * m[3][3]) + m[1][3] * (m[2][0] * m[3][1] - m[2][1] * m[3][0]));
	res.m[2][1] = -invdet * (m[0][0] * (m[2][1] * m[3][3] - m[2][3] * m[3][1]) + m[0][1] * (m[2][3] * m[3][0] - m[2][0] * m[3][3]) + m[0][3] * (m[2][0] * m[3][1] - m[2][1] * m[3][0]));
	res.m[2][2] = invdet  * (m[0][0] * (m[1][1] * m[3][3] - m[1][3] * m[3][1]) + m[0][1] * (m[1][3] * m[3][0] - m[1][0] * m[3][3]) + m[0][3] * (m[1][0] * m[3][1] - m[1][1] * m[3][0]));
	res.m[2][3] = -invdet * (m[0][0] * (m[1][1] * m[2][3] - m[1][3] * m[2][1]) + m[0][1] * (m[1][3] * m[2][0] - m[1][0] * m[2][3]) + m[0][3] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]));
	res.m[3][0] = -invdet * (m[1][0] * (m[2][1] * m[3][2] - m[2][2] * m[3][1]) + m[1][1] * (m[2][2] * m[3][0] - m[2][0] * m[3][2]) + m[1][2] * (m[2][0] * m[3][1] - m[2][1] * m[3][0]));
	res.m[3][1] = invdet  * (m[0][0] * (m[2][1] * m[3][2] - m[2][2] * m[3][1]) + m[0][1] * (m[2][2] * m[3][0] - m[2][0] * m[3][2]) + m[0][2] * (m[2][0] * m[3][1] - m[2][1] * m[3][0]));
	res.m[3][2] = -invdet * (m[0][0] * (m[1][1] * m[3][2] - m[1][2] * m[3][1]) + m[0][1] * (m[1][2] * m[3][0] - m[1][0] * m[3][2]) + m[0][2] * (m[1][0] * m[3][1] - m[1][1] * m[3][0]));
	res.m[3][3] = invdet  * (m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) + m[0][1] * (m[1][2] * m[2][0] - m[1][0] * m[2][2]) + m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0])); 
	*this = res;

	return *this;
}

void Matrix::setZero(){
    memset(m,0,16*sizeof(float));
}

void Matrix::setIdentity(){
    setZero();
    for(int i=0;i<4;i++){
        m[i][i]=1;
    }
}

void Matrix::setScaleTransform(float ScaleX, float ScaleY, float ScaleZ){
    m[0][0] = ScaleX; m[0][1] = 0.0f;   m[0][2] = 0.0f;   m[0][3] = 0.0f;
    m[1][0] = 0.0f;   m[1][1] = ScaleY; m[1][2] = 0.0f;   m[1][3] = 0.0f;
    m[2][0] = 0.0f;   m[2][1] = 0.0f;   m[2][2] = ScaleZ; m[2][3] = 0.0f;
    m[3][0] = 0.0f;   m[3][1] = 0.0f;   m[3][2] = 0.0f;   m[3][3] = 1.0f;
}

void Matrix::setRotateTransform(float RotateX, float RotateY, float RotateZ){
    Matrix rx, ry, rz;

    const float x = ToRadian(RotateX);
    const float y = ToRadian(RotateY);
    const float z = ToRadian(RotateZ);

    rx.m[0][0] = 1.0f; rx.m[0][1] = 0.0f   ; rx.m[0][2] = 0.0f    ; rx.m[0][3] = 0.0f;
    rx.m[1][0] = 0.0f; rx.m[1][1] = cosf(x); rx.m[1][2] = -sinf(x); rx.m[1][3] = 0.0f;
    rx.m[2][0] = 0.0f; rx.m[2][1] = sinf(x); rx.m[2][2] = cosf(x) ; rx.m[2][3] = 0.0f;
    rx.m[3][0] = 0.0f; rx.m[3][1] = 0.0f   ; rx.m[3][2] = 0.0f    ; rx.m[3][3] = 1.0f;

    ry.m[0][0] = cosf(y); ry.m[0][1] = 0.0f; ry.m[0][2] = -sinf(y); ry.m[0][3] = 0.0f;
    ry.m[1][0] = 0.0f   ; ry.m[1][1] = 1.0f; ry.m[1][2] = 0.0f    ; ry.m[1][3] = 0.0f;
    ry.m[2][0] = sinf(y); ry.m[2][1] = 0.0f; ry.m[2][2] = cosf(y) ; ry.m[2][3] = 0.0f;
    ry.m[3][0] = 0.0f   ; ry.m[3][1] = 0.0f; ry.m[3][2] = 0.0f    ; ry.m[3][3] = 1.0f;

    rz.m[0][0] = cosf(z); rz.m[0][1] = -sinf(z); rz.m[0][2] = 0.0f; rz.m[0][3] = 0.0f;
    rz.m[1][0] = sinf(z); rz.m[1][1] = cosf(z) ; rz.m[1][2] = 0.0f; rz.m[1][3] = 0.0f;
    rz.m[2][0] = 0.0f   ; rz.m[2][1] = 0.0f    ; rz.m[2][2] = 1.0f; rz.m[2][3] = 0.0f;
    rz.m[3][0] = 0.0f   ; rz.m[3][1] = 0.0f    ; rz.m[3][2] = 0.0f; rz.m[3][3] = 1.0f;

    *this = rz * ry * rx;
}

void Matrix::setTranslationTransform(float x, float y, float z){
    m[0][0] = 1.0f; m[0][1] = 0.0f; m[0][2] = 0.0f; m[0][3] = x;
    m[1][0] = 0.0f; m[1][1] = 1.0f; m[1][2] = 0.0f; m[1][3] = y;
    m[2][0] = 0.0f; m[2][1] = 0.0f; m[2][2] = 1.0f; m[2][3] = z;
    m[3][0] = 0.0f; m[3][1] = 0.0f; m[3][2] = 0.0f; m[3][3] = 1.0f;
}

Matrix Matrix::operator*(const Matrix& Right) const{
    Matrix Ret;

    for (unsigned int i = 0 ; i < 4 ; i++) {
        for (unsigned int j = 0 ; j < 4 ; j++) {
            Ret.m[i][j] = m[i][0] * Right.m[0][j] +
                            m[i][1] * Right.m[1][j] +
                            m[i][2] * Right.m[2][j] +
                            m[i][3] * Right.m[3][j];
        }
    }

    return Ret;
}

Vector3 Matrix::operator*(const Vector3& Right) const{
    float res[3]={0,0,0};
    float vec[4];
    vec[0]=Right.x;
    vec[1]=Right.y;
    vec[2]=Right.z;
    vec[3]=1;
    for(int i=0;i<3;i++){
        for(int j=0;j<4;j++){
            res[i]+=m[j][i]*vec[j];
        }
    }
    Vector3 ret(res[0],res[1],res[2]);
    return ret;
}

Matrix Matrix::inverted(){
    float det;
    int i;
    Matrix inv;

    inv.m[0][0]= m[1][1] * m[2][2]* m[3][3]- 
             m[1][1] * m[2][3]* m[3][2]- 
             m[2][1] * m[1][2] * m[3][3]+ 
             m[2][1] * m[1][3] * m[3][2]+
             m[3][1]* m[1][2] * m[2][3]- 
             m[3][1]* m[1][3] * m[2][2];

    inv.m[1][0]= -m[1][0] * m[2][2]* m[3][3]+ 
              m[1][0] * m[2][3]* m[3][2]+ 
              m[2][0] * m[1][2] * m[3][3]- 
              m[2][0] * m[1][3] * m[3][2]- 
              m[3][0]* m[1][2] * m[2][3]+ 
              m[3][0]* m[1][3] * m[2][2];

    inv.m[2][0]= m[1][0] * m[2][1]* m[3][3]- 
             m[1][0] * m[2][3]* m[3][1]- 
             m[2][0] * m[1][1]* m[3][3]+ 
             m[2][0] * m[1][3]* m[3][1]+ 
             m[3][0]* m[1][1]* m[2][3]- 
             m[3][0]* m[1][3]* m[2][1];

    inv.m[3][0]= -m[1][0] * m[2][1]* m[3][2]+ 
               m[1][0] * m[2][2]* m[3][1]+
               m[2][0] * m[1][1]* m[3][2]- 
               m[2][0] * m[1][2]* m[3][1]- 
               m[3][0]* m[1][1]* m[2][2]+ 
               m[3][0]* m[1][2]* m[2][1];

    inv.m[0][1]= -m[0][1] * m[2][2]* m[3][3]+ 
              m[0][1] * m[2][3]* m[3][2]+ 
              m[2][1] * m[0][2]* m[3][3]- 
              m[2][1] * m[0][3]* m[3][2]- 
              m[3][1]* m[0][2]* m[2][3]+ 
              m[3][1]* m[0][3]* m[2][2];

    inv.m[1][1]= m[0][0] * m[2][2]* m[3][3]- 
             m[0][0] * m[2][3]* m[3][2]- 
             m[2][0] * m[0][2]* m[3][3]+ 
             m[2][0] * m[0][3]* m[3][2]+ 
             m[3][0]* m[0][2]* m[2][3]- 
             m[3][0]* m[0][3]* m[2][2];

    inv.m[2][1]= -m[0][0] * m[2][1]* m[3][3]+ 
              m[0][0] * m[2][3]* m[3][1]+ 
              m[2][0] * m[0][1]* m[3][3]- 
              m[2][0] * m[0][3]* m[3][1]- 
              m[3][0]* m[0][1]* m[2][3]+ 
              m[3][0]* m[0][3]* m[2][1];

    inv.m[3][1]= m[0][0] * m[2][1]* m[3][2]- 
              m[0][0] * m[2][2]* m[3][1]- 
              m[2][0] * m[0][1]* m[3][2]+ 
              m[2][0] * m[0][2]* m[3][1]+ 
              m[3][0]* m[0][1]* m[2][2]- 
              m[3][0]* m[0][2]* m[2][1];

    inv.m[0][2]= m[0][1] * m[1][2]* m[3][3]- 
             m[0][1] * m[1][3]* m[3][2]- 
             m[1][1] * m[0][2]* m[3][3]+ 
             m[1][1] * m[0][3]* m[3][2]+ 
             m[3][1]* m[0][2]* m[1][3]- 
             m[3][1]* m[0][3]* m[1][2];

    inv.m[1][2]= -m[0][0] * m[1][2]* m[3][3]+ 
              m[0][0] * m[1][3]* m[3][2]+ 
              m[1][0] * m[0][2]* m[3][3]- 
              m[1][0] * m[0][3]* m[3][2]- 
              m[3][0]* m[0][2]* m[1][3]+ 
              m[3][0]* m[0][3]* m[1][2];

    inv.m[2][2]= m[0][0] * m[1][1]* m[3][3]- 
              m[0][0] * m[1][3]* m[3][1]- 
              m[1][0] * m[0][1]* m[3][3]+ 
              m[1][0] * m[0][3]* m[3][1]+ 
              m[3][0]* m[0][1]* m[1][3]- 
              m[3][0]* m[0][3]* m[1][1];

    inv.m[3][2]= -m[0][0] * m[1][1]* m[3][2]+ 
               m[0][0] * m[1][2]* m[3][1]+ 
               m[1][0] * m[0][1]* m[3][2]- 
               m[1][0] * m[0][2]* m[3][1]- 
               m[3][0]* m[0][1]* m[1][2]+ 
               m[3][0]* m[0][2]* m[1][1];

    inv.m[0][3]= -m[0][1]* m[1][2]* m[2][3]+ 
              m[0][1]* m[1][3]* m[2][2]+ 
              m[1][1]* m[0][2]* m[2][3]- 
              m[1][1]* m[0][3]* m[2][2]- 
              m[2][1]* m[0][2]* m[1][3]+ 
              m[2][1]* m[0][3]* m[1][2];

    inv.m[1][3]= m[0][0]* m[1][2]* m[2][3]- 
             m[0][0]* m[1][3]* m[2][2]- 
             m[1][0]* m[0][2]* m[2][3]+ 
             m[1][0]* m[0][3]* m[2][2]+ 
             m[2][0]* m[0][2]* m[1][3]- 
             m[2][0]* m[0][3]* m[1][2];

    inv.m[2][3]= -m[0][0]* m[1][1]* m[2][3]+ 
               m[0][0]* m[1][3]* m[2][1]+ 
               m[1][0]* m[0][1]* m[2][3]- 
               m[1][0]* m[0][3]* m[2][1]- 
               m[2][0]* m[0][1]* m[1][3]+ 
               m[2][0]* m[0][3]* m[1][1];

    inv.m[3][3]= m[0][0]* m[1][1]* m[2][2]- 
              m[0][0]* m[1][2]* m[2][1]- 
              m[1][0]* m[0][1]* m[2][2]+ 
              m[1][0]* m[0][2]* m[2][1]+ 
              m[2][0]* m[0][1]* m[1][2]- 
              m[2][0]* m[0][2]* m[1][1];

    det = m[0][0]* inv.m[0][0]+ m[0][1]* inv.m[1][0]+ m[0][2]* inv.m[2][0]+ m[0][3]* inv.m[3][0];

    if (det == 0)
        return Matrix();

    det = 1.0 / det;

    for (i = 0; i < 4; i++)
        for(int j=0;j<4;j++)
                inv.m[i][j] =inv.m[i][j] * det;

    return inv;
}

Matrix Matrix::transposed(){
    Matrix mat;
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            mat.m[i][j]=m[j][i];
    return mat;
}

bool Matrix::operator==(const Matrix& Right){
    for(int i=0;i<4;i++)
        for(int j=0;j<4;j++)
            if(Right.m[i][j]!=m[i][j])
                return false;
        
    return true;
}

ostream& operator<<(ostream& os,const Matrix& m){
    for(int i=0;i<4;i++){
        os << "(";
        for(int j=0;j<4;j++)
            os << m.m[i][j] << " ";
        os << ")\n";
    }
    return os;
}
