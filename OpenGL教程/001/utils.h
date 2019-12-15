#ifndef OpenGL_utils_h
#define OpenGL_utils_h

using namespace std;
typedef struct{
    float m[16];
}Matrix;

Matrix identity();
/*
 创建平移矩阵
 1   0   0   tx
 0   1   0   ty
 0   0   1   tz
 0   0   0   1
 */
Matrix translate(float tx, float ty, float tz);
/*
 创建绕Z轴旋转的矩阵
 cos -sin    0   0
 sin cos     0   0
 0   0       1   0
 0   0       0   1
 */
Matrix rotationZ(float degree);
/*
 创建缩放矩阵
 sx  0   0   0
 0   sy  0   0
 0   0   sz  0
 0   0   0   1
 */
Matrix scale(float sx,float sy,float sz);
/*
 绕任意轴旋转
 */
Matrix rotate(float rx,float ry,float rz,float degree);
/*
 矩阵乘法
 */
Matrix multiply(const Matrix&m0, const Matrix&m1);
/*
 矩阵乘法（运算符重载的方法）
 */
Matrix operator*(const Matrix&m0, const Matrix&m1);
/*
 读取文件的所有内容
 */
char* get_file_contents(const char *filename);
#endif
