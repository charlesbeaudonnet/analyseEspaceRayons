#include "matrix.h"
#include <QDebug>

Matrix::Matrix(int Nbcol, int Nbrow){
    col = Nbcol;
    row = Nbrow;
    m_matrix = new int*[row];
    for(int i = 0; i < row; m_matrix[i++] = new int[col]){}
}


int Matrix::get(int i, int j){
    return m_matrix[i][j];
}


void Matrix::set(int i, int j, int value){
    m_matrix[i][j] = value;
}

void Matrix::display(){
    for(int i = 0;i < row;++i){
        for(int j = 0;j < col;++j){
           qDebug() << m_matrix[i][j] << " ";
        }
        qDebug() << "\n";
    }
    qDebug() << "\n";
}

void Matrix::display(QString filename)
{
    QFile outputFile(filename);
    if (outputFile.open(QIODevice::Append)) {
        QTextStream out(&outputFile);
        for(int i = 0;i < row;++i){
            for(int j = 0;j < col;++j){
                out <<"["<<i<<","<<j<<"]:" << m_matrix[i][j] << " ";
            }
            out << "\n";
        }
        out <<"\n";
    }
}
