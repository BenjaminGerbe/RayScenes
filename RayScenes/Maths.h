#pragma once
#include <iostream>
#include <math.h>

template<int H,int W>
class Matrix {
   
    float tab[H*W];
    int N = H;
    int M = W;

    public:

    Matrix();

    Matrix(float t[H * W]);

    template<int P, int M>
    Matrix(Matrix<P,M> mat){
        (*this).tab = mat.tab;
        N = mat.N;
        M = mat.M;
    }

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
    float norme;

    public :

    Vector();

    Vector(float* t[N]);

    float* getTab()const;

    void setAt(int i, float v);

    float getAt(int i) const;

    void updateNorme();

    float norm();

    void normalized();

    Vector operator+(const Vector& vec) {
        Vector v;

        for (int i = 0; i < N; i++)
        {
            v[i] = (*this).getAt(i) + vec[i];
        }


        v.updateNorme();
        return v;
    }

    Vector operator-(const Vector& vec) {
        Vector v;

        for (int i = 0; i < N; i++)
        {
            v[i] = (*this).getAt(i) - vec[i];
        }


        v.updateNorme();

        return v;
    }

    Vector operator*(float s) {
        Vector v;

        for (int i = 0; i < N; i++)
        {
            v[i] = (*this).getAt(i)*s;
        }

        v.updateNorme();
        return v;
    }

    Vector operator-() {
        Vector v;

        for (int i = 0; i < N; i++)
        {
            v[i] = -(*this).getAt(i);
        }

        v.updateNorme();
        return v;
    }

    Vector operator/(float s) {
        Vector v;

        for (int i = 0; i < N; i++)
        {
            v[i] = (*this).getAt(i) / s;
        }

        v.updateNorme();
        return v;
    }


    float operator[](int i) const {
        return tab[i];
    }

    float dot(const Vector<N> vec);


};

