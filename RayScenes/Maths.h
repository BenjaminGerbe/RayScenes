﻿#ifndef __MATHlib__
#define __MATHlib__

# define M_PI           3.14159265358979323846  /* pi */

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
    Matrix<H,P> operator*(const Matrix<O,P>& m) {
       
        
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

    Vector(float t[N]);

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
            setAt(i, vec.getAt(i));
        }

        return (*this);
    }


    Vector operator+(const Vector& vec) {
        Vector v;

        for (int i = 0; i < N; i++)
        {
            v.setAt(i, getAt(i) + vec[i]);
        }

        return v;
    }


    Vector operator+(const Vector& vec) const {
        Vector v;

        for (int i = 0; i < N; i++)
        {
            v.setAt(i, getAt(i) + vec[i]);
        }

        return v;
    }

    Vector operator-(const Vector& vec) {
        Vector v;

        for (int i = 0; i < N; i++)
        {
            v.setAt(i, getAt(i) - vec[i]);
        }

        return v;
    }

    Vector operator-(const Vector& vec) const {
        Vector v;

        for (int i = 0; i < N; i++)
        {
            v.setAt(i, getAt(i) - vec[i]);
        }

        return v;
    }

    Vector operator*(float s) {
        Vector v;

        for (int i = 0; i < N; i++)
        {
            v.setAt(i, getAt(i) * s);
        }
        return v;
    }

 
    Vector operator-() {
        Vector v;

        for (int i = 0; i < N; i++)
        {
            v.setAt(i, -(*this).getAt(i));
        }
        return v;
    }

    Vector operator/(float s) {
        Vector v;

        for (int i = 0; i < N; i++)
        {
            v.setAt(i, v[i] / s);
        }

        return v;
    }

    

    template<int H, int W>
    friend Vector<W> operator*(const Matrix<H, W>& t, const Vector<W>& m);


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

    Vector3(Vector<3> vec) {
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


    float getX() const {
        return Vector::getAt(0);
    };

    float getY()const {
        return Vector::getAt(1);
    };

    float getZ()const {
        return Vector::getAt(2);
    };

    Vector3 operator=(Vector<3> vec) {
        for (int i = 0; i < 3; i++)
        {
            setAt(i, vec.getAt(i));
        }

        return (*this);
    }

    Vector3 crossProduct(const Vector3 b) const {
        Vector3 a = (*this);

        float x = a.y*b.z-a.z*b.y;
        float y = a.z * b.x - a.x * b.z;
        float z = a.x*b.y - a.y*b.x;

        return Vector3(x, y, z);
    }

};

class Vector4 : public Vector<4> {

public:

    __declspec(property(get = getX)) float x;
    __declspec(property(get = getY)) float y;
    __declspec(property(get = getZ)) float z;
    __declspec(property(get = getW)) float w;

    Vector4() {
    };

    Vector4(Vector<4> vec) {

        for (int i = 0; i < 4; i++) {
            setAt(i, vec.getAt(i));
        }
    };

    Vector4(float _x, float _y, float _z,float _w) {
        setAt(0, _x);
        setAt(1, _y);
        setAt(2, _z);
        setAt(3, _w);
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

    Vector4 operator*(float scalar) const {
        return Vector4(getAt(0) * scalar, getAt(1) * scalar, getAt(2) * scalar, getAt(3) * scalar);
    }

};


class Ray {
    Vector3 origin;
    Vector3 direction;

    public:
    Ray() {
        origin = Vector3();
        direction = Vector3();
    }

    Ray(Vector3 ori, Vector3 dir) {
        ori = origin;
        direction = dir;
    }

    Ray(float ox, float oy, float oz, float dx, float dy, float dz) {
        origin = Vector3(ox,oy,oz);
        direction = Vector3(dx,dy,dz);
    }

    Vector3 getOrigin() {
        return this->origin;
    }

    Vector3 getDirection() {
        return this->direction;
    }

    void setOrigin(Vector3 o) {
       this->origin = o;
    }

    void setDirection(Vector3 d) {
        this->origin = d;
    }

};

class Ray4 {
    Vector4 origin;
    Vector4 direction;

public:
    Ray4() {
        origin = Vector4();
        direction = Vector4();
    }

    Ray4(Ray r) {
        origin =  Vector4(r.getOrigin().x, r.getOrigin().y, r.getOrigin().z, 1);
        direction =  Vector4(r.getDirection().x, r.getDirection().y, r.getDirection().z, 0);

    }

    Ray4( Ray4 &r) {
        origin =  Vector4(r.getOrigin());
        direction =  Vector4(r.getDirection());
    }

    Ray4(const Ray4& r) {
        origin =  Vector4(r.getOrigin());
        direction =  Vector4(r.getDirection());
    }

    Ray4(Vector4 ori, Vector4 dir) {
        origin =   ori ;
        direction =  dir;
    }

    

    Ray4(float ox, float oy, float oz, float ow, float dx, float dy, float dz, float dw) {
        origin =  Vector4(ox, oy, oz,ow);
        direction =  Vector4(dx, dy, dz,dw);
    }

    Vector4 getOrigin() const{
        return (this->origin);
    }

    Vector4 getDirection()const {
        return this->direction;
    }

    void setOrigin(const Vector4 o) {
        this->origin =  Vector4(o);
    }

    void setDirection(const Vector4 d) {
        this->direction =  Vector4(d);
    }

    Ray4 normalized() {
        this->direction = direction.normalized();

        return (*this);
    }
    
    friend Ray4 operator*(const Matrix4x4& mat,const Ray4& r) {
    

        Ray4 ray( mat * r.getOrigin(), mat * r.getDirection());
    
        return ray;
    }

    Ray4 operator=(const Ray4 &r)  {
        origin =  Vector4(r.getOrigin());
        direction =  Vector4(r.getDirection());

        return (*this);
    }

};

template<int H, int W>
Vector<W> operator*(const Matrix<H, W>& t, const Vector<W>& m) {

    Vector<W> mat;
    for (int i = 0; i < H; i++) {    
        mat.setAt(i, 0);
        for (int k = 0; k < W; k++) {
            float v = mat.getAt(i) + (t)(i, k) * m.getAt(k);
            mat.setAt(i, v);
        }
    }

    return mat;
}

template<int N>
Vector<N> operator*(  float& b,  Vector<N>& vec) {
    return vec * b;
}




template<int N>
float distance(Vector<N> a, Vector<N> b) {
    float d = 0;

    for (int i = 0; i < N; i++)
    {
        d += a[i] - b[i];
    }
    return std::sqrtf(d);
}



#endif

