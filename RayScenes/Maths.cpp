#include "Maths.h"


// Matrix implementation //

template<int H, int W>
float* Matrix <H,W>::getTab() const{
    return this->tab;
}

template<int H, int W>
void Matrix <H,W>::setAt(int i, int j, int v) {
    if (i >= H || j >= W || i < 0 || j < 0) {
        throw std::exception(" out of bounds");
    }
    tab[(i * N) + j] = v;
}

template<int H, int W>
float Matrix <H,W>::getAt(int i, int j) const{
    return tab[(i * N) + j];
}

template<int H, int W>
Matrix < H,W > Matrix<H, W>::subMatrix(int p, int q, int n)
{
    Matrix <H,W> temp;

    for (int k = 0; k < H; k++)
    {
        for (int l = 0; l < W; l++)
        {
            temp.setAt(k,l,0);
        }
    }

    int i = 0, j = 0;
    for (int r = p; r < p + n; r++) {
        for (int c = q; c < q + n; c++) {
            std::cout << c << " " << W << std::endl;
            if (c >= W || r >= H) {
                  temp.setAt(i, j,0);
            }
            else {
                temp.setAt(i, j, getAt(r, c));
            }
          
            j++;
        }
        i++;
        j = 0;
    }
    return temp;
}

template<int H, int W>
float Matrix<H, W>::getDet()
{
    
    if (H != W) throw std::exception("la matrix doit être carré");

    if (H == 0) return 1;

    if (H == 1) return getAt(0, 0);

    if (H == 2) {
        return getAt(0, 0) * getAt(1, 1) - getAt(0, 1) * getAt(1, 0);
    }

    float result = 0;
    int signe = 1;

    for (int i = 0; i < H; i++)
    {
        Matrix<H - 1, H - 1> temp;

        for (int m = 1; m < H; m++)
        {
            int z = 0;
            for (int n = 0; n < H; n++)
            {
                if (n != i) {
                    temp.setAt(m - 1, z, getAt(n, m));
                    z++;
                }
            }
        }

        result += signe * getAt(0, i) * temp.getDet();
        signe = -signe;
    }



    return result;
   
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
    delete[] tab;
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



