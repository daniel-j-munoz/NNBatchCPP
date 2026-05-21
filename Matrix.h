#include <iostream> 
#include <vector> 
#include <fstream>
#include <random>
#include <string>
using namespace std;

#pragma once
class Matrix {
    public: 
        int M; 
        int N; 
        vector<float> data; 
        Matrix();
        Matrix(int M, int N, const vector<float>& data); 
        Matrix(int M, int N, float value); 
        Matrix(int M, int N); 

        // ........................ Matrix .........................................................
        Matrix T(); 

        // ............... Row & Col ................................................
        float& get(int i); // this would get data and treat like just regular array
        float& get(int m, int n); 
        Matrix get(int m, int n, int M, int N); // get column and rows from here? and general things?...
        Matrix column(int n);
        void column_set(const Matrix& other, int j);
    

        vector<Matrix> list_rows();
        vector<Matrix> list_columns(int a, int b);
        // column_list
        // row_list

        // ....... Arithemtic ...................................................
        Matrix element_wise(const Matrix& other, char operation);
        Matrix operator*(float);
        Matrix operator*=(float);
        void scale(float); // keep scale as well... i think... both are helpful i belive...
        Matrix operator*(const Matrix&); // Mat Mult
        Matrix operator*=(const Matrix&); 
        Matrix operator+(const Matrix&);
        Matrix operator+=(const Matrix&);
        Matrix operator-(const Matrix&);
        Matrix operator-=(const Matrix&);
        float dot(Matrix&);  

        void set(int t, int l, int b, int r, const Matrix&);
        void set_rows(int t, int b, const Matrix&);
        void set_columns(int l, int r, const Matrix&);

        void stack_down(const Matrix&);
        void stack_right(const Matrix&);


        // reshape?

        /**
         * print to x many decimal places
         */
        void print(int x) const;

        void encode(ofstream& ofs);
        void decode(ifstream& ifs);
};