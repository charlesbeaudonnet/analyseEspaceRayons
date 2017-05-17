#ifndef __MATRIX_H__
#define __MATRIX_H__

#include <QString>
#include <QFile>
#include <QTextStream>

class Matrix{

public :
    int col;
    int row;
    int ** m_matrix;

    Matrix(){}
    Matrix(int row, int col);
    int get(int i, int j);
    void set(int i, int j, int value);
    void display();
    void display(QString filename);
};

#endif // __MATRIX_H__
