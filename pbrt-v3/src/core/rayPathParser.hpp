/*
 * rayPathParser.h
 *
 *  Created on: 26 avr. 2017
 *      Author: todo
 */

#ifndef PBRT_V3_SRC_CORE_RAYPATHPARSER_HPP_
#define PBRT_V3_SRC_CORE_RAYPATHPARSER_HPP_
#include <regex>
#include <sys/types.h>
#include <dirent.h>
#include <fstream>

#include <matrix.h>

using namespace std;
class rayPathParser {
public:
	DIR * directory;
	ofstream rayPath;
	std::string filename;
	rayPathParser(std::string fileName);
	virtual ~rayPathParser();
	void parseFile();
};
rayPathParser::rayPathParser(std::string fileName) {

	directory = opendir( "." );
	if( directory == NULL ){
		perror( "Can't open executable's directory" );
		exit( -2 );
	}

	filename = fileName;
}

rayPathParser::~rayPathParser() {
    closedir(directory);
}
/*
 * réécrit le fichier rayPath.txt avec la syntaxe voulut
 */
void rayPathParser::parseFile() {
	//On lit et ecrit dans rayPath
	ifstream reader(filename.c_str());

	//Contient les cordonnées du pixel [x,y]
	std::string pixel;
	int x,y;

	//char en cours de traitement et position dans le texte
	char current;
	unsigned long long int octet = 0;

	/* On créé la matrice des pixels (il faudra aménager tout ca quand on aura mit la taille
	 * de l'image dans le fichier txt)
	 */
	Matrix mat(200,200);


	if( !reader.is_open() ){
		std::cerr << "Can't open " << filename<<"\n";
		exit( -2 );
	}

	//On lit char par char
	do{
		//Si c'est la fin du fichier on sort
		if(reader.eof())
			break;

		//Sinon on prend un char
		reader.get(current);
		//Si on a une accolade alors on est au début d'un chemin
		if(current == '{'){
			//Sauvegarde le decalage dans le fichier suite à la recupération des coordonnees du pixel
			int offset = 0;

			//On recupère les char jusqu'à ] (marqueur de fin de pixel)
			do{
				reader.get(current);
				pixel += current;
				offset++;
            }while(current != ']' || reader.eof());

            sscanf(pixel.c_str(),"[%d,%d]",&x,&y);

            //On ecrit sa position dans la matrice
			mat.set(x,y,octet);
			octet += offset;
			pixel = "";
		}
		octet++;
	}while(true);

	mat.display(filename);
}

#endif /* PBRT_V3_SRC_CORE_RAYPATHPARSER_HPP_ */
