#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
class Matrix
{
    int col;
    int row;
    unsigned long long int ** m_matrix;

    public :
        Matrix(int row, int col);
        unsigned long long int get(int i, int j);
        void set(int i, int j, unsigned long long int value);
        void display ();
        void display(std::string filename);
};

#endif
