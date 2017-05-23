#include "matrix.h"
#include <QDebug>

/**
 * @brief Matrix Creer et initialiser la matrice
 * @param Nbrow Nombre de lignes de la matrice
 * @param Nbcol Nombre de colonnes de la matrice
 */
Matrix::Matrix(int Nbrow, int Nbcol){
    col = Nbcol;
    row = Nbrow;
    m_matrix = new int*[row];
    for(int i = 0; i < row; m_matrix[i++] = new int[col]){}
}

/*
 * Input : Deux int, les indices de ligne et colonne de la valeur souhaité
 * Role  : Acceder à l'élément (i,j) de la matrice
 * Output: Un entier
*/
/**
 * @brief get Retourne la valeur de l'élément (i,j) de la matrice
 * @param i Indice de ligne de l'élément voulut
 * @param j Indice de colonne de l'élément voulut
 * @return  la valeur de l'élément (i,j) de la matrice
 */
int Matrix::get(int i, int j){
    return m_matrix[i][j];
}

/**
 * @brief set Donne la valeur value à l'élément (i,j)
 * @param i Indice de ligne de l'élément voulut
 * @param j Indice de colonne de l'élément voulut
 * @param value la valeur à donner à l'élément (i,j)
 */
void Matrix::set(int i, int j, int value){
    m_matrix[i][j] = value;
}

/**
 * @brief display Affiche la matrice dans la console
 */
void Matrix::display(){
    for(int i = 0;i < row;++i){
        for(int j = 0;j < col;++j){
           qDebug() << m_matrix[i][j] << " ";
        }
        qDebug() << "\n";
    }
    qDebug() << "\n";
}

/**
 * @brief display Ecrit la matrice dans le fichier de nom filename.txt
 * @param filename
 */
void Matrix::display(QString filename){
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
