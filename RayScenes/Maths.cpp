#include "Maths.h"


// Matrix implementation //

template<int H, int W>
float* Matrix <H,W>::getTab() const{
    return this->tab;
}

template<int H, int W>
void Matrix <H,W>::setAt(int i, int j, int v) {
    tab[(i * N) + j] = v;
}

template<int H, int W>
float Matrix <H,W>::getAt(int i, int j) const{
    return tab[(i * N) + j];
}

template<int H, int W>
Matrix <H, W>::Matrix() {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            tab[(i * N) + j] = 0.0;
            if (i == j) {
                tab[(i * N) + j] = 1.0;
            }
        }
    }
}

template<int H, int W>
Matrix <H, W>::Matrix(float t[H * W]) {

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            tab[(i * N) + j] = t[(i * N) + j];
        }
    }
}


template<int H, int W>
Matrix <H, W>::~Matrix() {
    
}



// Vector implementation //

template<int N>
Vector<N>::Vector() {

    for (int i = 0; i < N; i++)
    {
        tab[i] = 0.0;
    }

    updateNorme();
    
}

template<int N>
Vector<N>::Vector(float* t[N]) {
    for (int i = 0; i < N; i++)
    {
        tab[i] = t[i];
    }

    updateNorme();
}


template<int N>
float* Vector<N>::getTab()const {
    return  tab;
}

template<int N>
void Vector<N>::setAt(int i, float v) {
    tab[i] = v;
    updateNorme();
}



template<int N>
float Vector<N>::getAt(int i) const {
    return tab[i];
}

template<int N>
float Vector<N>::norm() {
    return norme;
}


template<int N>
void Vector<N>::updateNorme() {

    float sum = 0.0;
    for (int i = 0; i < N; i++)
    {
        sum += tab[i] * tab[i];
    }

    norme = std::sqrtf(sum);
}

template<int N>
void Vector<N>::normalized() {
    float norme = norme();

    for (int i = 0; i < N; i++)
    {
        tab[i] = tab[i] / norme;
    }
}


template<int N>
float Vector<N>::dot(const Vector<N> vec) {
    float v = 0.0;

    for (int i = 0; i < N; i++)
    {
        v += tab[i] * vec.getAt(i);
    }

    return v;
}



