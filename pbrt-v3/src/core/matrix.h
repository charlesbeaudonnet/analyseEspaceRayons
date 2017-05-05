#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
class Matrix
{
	unsigned long long int ** m_matrix;

public :
	int col;
	int row;
	Matrix(int row, int col);
	unsigned long long int get(int i, int j);
	void set(int i, int j, unsigned long long int value);
	void display ();
	void display(std::string filename);
};

#endif
