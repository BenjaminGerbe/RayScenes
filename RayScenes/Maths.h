#pragma once
#include <iostream>
#include <math.h>
#include <iomanip> 

template<int H,int W>
class Matrix {
   
    float* tab = new float[H*W];
    int N = H;
    int M = W;

    public:

    Matrix();

    Matrix(float t[H * W]);
  
    Matrix(const Matrix& mat){
        
        for (int i = 0; i < H; i++)
        {
            for (int j = 0; j < W; j++)
            {
                setAt(i, j, mat.getAt(i, j));
            }
        }

        N = mat.N;
        M = mat.M;
    }

    ~Matrix();


    float* getTab()const;

    void setAt(int i, int j, float v);

    float getAt(int i, int j) const;

    float operator()(int i, int j) const {
        return tab[(i * N) + j];
    }

    float getDet(int n = H);

    Matrix subMatrix(int p, int q, int n);

    Matrix getCofactor();

    Matrix getInverse();

    Matrix transpose();

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

    Matrix<H, W> operator=(const Matrix<H,W>& mat) {
        for (int i = 0; i < H; i++)
        {
            for (int j = 0; j < W; j++)
            {
                setAt(i, j, mat.getAt(i, j));
            }
        }

        N = mat.N;
        M = mat.M;

        return (*this);
    };

    friend std::ostream& operator<<(std::ostream& os, const Matrix< H, W >& mat) {
   
      
        for (int i = 0; i < H; ++i) {
            os << "[ ";
            for (int j = 0; j < W; ++j) {
                if (j > 0) os << ", ";

                os<< mat(i, j);
            }
            os << " ]";
            os << std::endl;
        }

        return os;
    };

};


typedef Matrix<4, 4> Matrix4x4;
typedef Matrix<3, 3> Matrix3x3;

template<int N>
class Vector {


    float* tab = new float[N];
 

    public :

    Vector();

    Vector(float* t[N]);

    float* getTab()const;

    void setAt(int i, float v);

    float getAt(int i) const;

    float getNorme();

    Vector normalized();

    Vector(const Vector& vec);

    ~Vector();

    Vector operator=(const Vector& vec) {
        for (int i = 0; i < N; i++)
        {
            tab[i] = vec.tab[i];
        }

        return (*this);
    }

    Vector operator+(const Vector& vec) {
        Vector v;

        for (int i = 0; i < N; i++)
        {
            v[i] = (*this).getAt(i) + vec[i];
        }

        return v;
    }

    Vector operator-(const Vector& vec) {
        Vector v;

        for (int i = 0; i < N; i++)
        {
            v[i] = (*this).getAt(i) - vec[i];
        }

        return v;
    }

    Vector operator*(float s) {
        Vector v;

        for (int i = 0; i < N; i++)
        {
            v[i] = (*this).getAt(i)*s;
        }
        return v;
    }

    Vector operator-() {
        Vector v;

        for (int i = 0; i < N; i++)
        {
            v[i] = -(*this).getAt(i);
        }
        return v;
    }

    Vector operator/(float s) {
        Vector v;

        for (int i = 0; i < N; i++)
        {
            v[i] = (*this).getAt(i) / s;
        }

        return v;
    }


    float operator[](int i) const {
        return tab[i];
    }

    float dot(const Vector<N> vec);

    friend std::ostream& operator<<(std::ostream& os, const Vector<N>& vec) {
        os << "[ ";
        for (int i = 0; i < N; ++i) {
           
            if (i > 0) os << " , ";
            os << vec[i];
        }
        os << " ]";
        os << std::endl;

        return os;
    };

};



class Vector3 : public Vector<3> {
    
    public :

    __declspec(property(get = getX)) float x;
    __declspec(property(get = getY)) float y;
    __declspec(property(get = getZ)) float z;

    Vector3() {
    };

    Vector3(float _x, float _y, float _z) {
        setAt(0, _x);
        setAt(1, _y);
        setAt(2, _z);
    }

    float getX() {
        return Vector::getAt(0);
    };

    float getY() {
        return Vector::getAt(1);
    };

    float getZ() {
        return Vector::getAt(2);
    };

};

class Vector4 : public Vector<4> {

public:

    __declspec(property(get = getX)) float x;
    __declspec(property(get = getY)) float y;
    __declspec(property(get = getZ)) float z;
    __declspec(property(get = getW)) float w;

    Vector4() {
    };

    Vector4(float _x, float _y, float _z,float _w) {
        setAt(0, _x);
        setAt(1, _y);
        setAt(2, _z);
        setAt(3, _z);
    }

    float getX() {
        return Vector::getAt(0);
    };

    float getY() {
        return Vector::getAt(1);
    };

    float getZ() {
        return Vector::getAt(2);
    };

    float getW() {
        return Vector::getAt(3);
    };

};


class Ray {
    Vector3 origin;
    Vector3 direction;

    public:
    Ray() {
        origin = Vector3();
        direction = Vector3();
        
    }

};

