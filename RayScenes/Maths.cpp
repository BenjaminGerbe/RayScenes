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
    
}

template<int N>
Vector<N>::Vector(float* t[N]) {
    for (int i = 0; i < N; i++)
    {
        tab[i] = t[i];
    }
}


template<int N>
float* Vector<N>::getTab()const {
    return  tab;
}

template<int N>
void Vector<N>::setAt(int i, float v) {
    tab[i] = v;
}

template<int N>
float Vector<N>::getAt(int i) const {
    return tab[i];
}


