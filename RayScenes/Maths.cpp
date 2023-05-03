#include "Maths.h"


// Matrix implementation //

template<int H, int W>
float* Matrix <H,W>::getTab() const{
    return this->tab;
}

template<int H, int W>
void Matrix <H,W>::setAt(int i, int j, float v) {
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
    for (int r = 0; r < n; r++)
    {
        for (int c = 0; c < n; c++)
        {
            if (r != p && c != q) {
                temp.setAt(i, j++, getAt(r, c));
                if (j == n - 1) {
                    j = 0;
                    i++;
                }
            }
        }
    }

    return temp;
}

template<int H, int W>
float Matrix<H, W>::getDet(int n)
{
    if (H != W) {
        throw std::exception("la matrix doit être carré");
    }

    float determinant = 0;
    if (n == 1) {
        return getAt(0, 0);
    }

    if (n == 2) {
      
       // std::cout << "( " <<getAt(0, 0) << " * " << getAt(1, 1) << ")" << " -  ( " << getAt(0, 1) << " * " << getAt(1, 0) << " )" << std::endl;
        return (getAt(0,0) * getAt(1,1)) - (getAt(0, 1) * getAt(1,0));
    }

    Matrix < H, W > temp;
    int sign = 1;

    for (int  i = 0; i < n; i++)
    {
        temp = subMatrix(0, i, n);
        determinant += sign * getAt(0, i) * temp.getDet(n - 1);
      
        sign = -sign;
    }

    return determinant;
 
}

template<int H, int W>
Matrix<H, W> Matrix<H, W>::transpose()
{
    Matrix mat = (*this);
    for (int i = 0; i < H; i++)
    {
        for (int j = 0; j < W; j++)
        {
            mat.setAt(i, j, (*this)(j, i));
        }
    }

    return mat;
}

template<int H, int W>
Matrix<H,W> Matrix<H, W>::getCofactor()
{
    if (H != W) {
        throw std::exception("la matrix doit être carré");
    }

    Matrix<H, W> matrix;
    Matrix<H - 1, W - 1> subMatrix;

    for (int i = 0; i < W; i++)
    {
        for (int j = 0; j < W; j++)
        {
            int p = 0;
            for (int x = 0; x < W; x++)
            {
                if (i == x) continue;

                int q = 0;

                for (int y = 0; y < W; y++)
                {
                    if (y == j) continue;

                    subMatrix.setAt(p, q, getAt(x, y));
                    q++;
                }
                p++;

            }

            matrix.setAt(i, j, std::powf(-1, i + j) * subMatrix.getDet());

        }
    }



    return matrix;
}

template<int H, int W>
Matrix < H,W> Matrix<H, W>::getInverse()
{
    float det = getDet();

    if (det == 0) {
        throw std::exception("la matrice n'est pas inversible");
    }

    float d = 1.0 / det;
    Matrix < H, W > result;


    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            result.setAt(i, j, getAt(i, j));
        }
    }

    result = result.getCofactor();


    for (int i = 0; i < H; i++)
    {
        for (int j = 0; j < W; j++)
        {
            float v = result.getAt(i, j) * d;
            result.setAt(i, j,v);
        }
    }
    return result.transpose();
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

// Vector implementation /
template<int N>
Vector<N>::Vector() {

    for (int i = 0; i < N; i++)
    {
        tab[i] = 0.0;
    }

    
}

template<int N>
Vector<N>::Vector(float t[N]) {

    for (int i = 0; i < N; i++)
    {
        tab[i] = t[i];
    }

}

template<int N>
Vector<N>::Vector(const Vector<N>& vec)
{
    
    tab = new float[N];
    for (int i = 0; i < N; i++) {
        tab[i] = vec.tab[i];
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

template<int N>
Vector<N>::~Vector()
{

    delete[] tab;
}

template<int N>
float Vector<N>::getNorme() {

    float sum = 0.0;
    for (int i = 0; i < N; i++)
    {
        sum += tab[i] * tab[i];
    }

    return std::sqrtf(sum);
}

template<int N>
Vector<N> Vector<N>::normalized() {
    float norme = getNorme();

    for (int i = 0; i < N; i++)
    {
        tab[i] = tab[i] / norme;
    }

    return (*this);
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
