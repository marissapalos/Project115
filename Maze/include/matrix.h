#ifndef _MATRIX
#define _MATRIX
#include <iostream>
#include <vector>
using namespace std;

template <class T> class matrix{
    private:
            vector <T> mat;
            int width;

    public:
            matrix();
            void redosize(int n);
            matrix(int n);
            void insert(int x, int y, T input);
            T getVal(int x, int y);

};
template <class T>
matrix<T>::matrix(){//def constructor
    vector<T> mat;
    width = 0;
}

template <class T>
void matrix<T>::redosize(int n){//for after deff
    mat.resize(n*n,false);
    width = n;
}

template <class T>
matrix<T>::matrix(int n){ //copy constructor
    mat.resize(n*n);
    width = n;
}

template<class T>
void matrix<T>::insert(int x, int y, T input){
    mat.at(x*width+y) = input;
}
template <class T>
T matrix<T>::getVal(int x, int y){
    return mat.at(x*width+y);
}


#endif

