#include "matrix.h"

Matrix::Matrix(int Nbcol, int Nbrow)
{
	col = Nbcol;
	row = Nbrow;
	m_matrix = new unsigned long long int*[row];
	for(int i = 0; i < row; m_matrix[i++] = new unsigned long long int[col]){}
}


unsigned long long int Matrix::get(int i, int j)
{
	return m_matrix[i][j];
}


void Matrix::set(int i, int j, unsigned long long int value)
{
	m_matrix[i][j] = value;
}

void Matrix::display()
{
	for(int i = 0;i < row;++i)
	{
		for(int j = 0;j < col;++j)
		{
			std::cout << m_matrix[i][j] << " ";
		}

		printf("\n");
	}

	printf("\n");
}

void Matrix::display(std::string filename)
{
	std::ofstream output(filename,std::ios::app);
	if( !output.is_open() ){
        perror( "Can't open file.txt" );
		exit( -2 );
	}
	output << "\n\n";
	for(int i = 0;i < row;++i)
	{
		for(int j = 0;j < col;++j)
		{
			output <<  std::to_string(m_matrix[i][j]) + " ";
		}
		output << "\n";
	}
	output <<"\n";
}
