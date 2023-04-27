#pragma once
#include <iostream>

template<int H,int W>
class Matrix {
   
    float tab[H*W];
    int N = H;
    int M = W;

    public:

    Matrix();

    Matrix(float t[H * W]);

    ~Matrix();

    float* getTab()const;

    void setAt(int i, int j, int v);

    float getAt(int i, int j) const;

    float operator()(int i, int j) const {
        return tab[(i * N) + j];
    }

    template<int O, int P>
    Matrix<H,P> operator*(Matrix<O,P>& m) {
        Matrix<H,P> mat;

        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                mat.setAt(i, j, 0);

                for (int k = 0; k < P; k++) {
                    mat.setAt(i, j, mat(i, j) + (*this)(i, k) * m(k, j));
                }
            }
        }

        return mat;
    }

    friend std::ostream& operator<<(std::ostream& os, const Matrix< H, W >& mat) {
   
        for (int i = 0; i < H; ++i) {
            os << "[ ";
            for (int j = 0; j < W; ++j) {
                if (j > 0) os << ", ";
                os << mat(i, j);
            }
            os << " ]";
            os << std::endl;
        }

        return os;
    };

};

typedef Matrix<4, 4> Matrix4x4;

template<int N>
class Vector {


    float tab[N];

    public :


    Vector();

    Vector(float* t[N]);

    float* getTab()const;

    void setAt(int i, float v);

    float getAt(int i) const;

    Vector operator+(const Vector& vec) {
        Vector v;

        for (int i = 0; i < N; i++)
        {
            v[i] += vec[i];
        }

        return v;
    }

    float operator()(int i) const {
        return tab[i];
    }

};


class Vector3 : public Vector<3> {
    public:
    float* x, y, z;

    

};