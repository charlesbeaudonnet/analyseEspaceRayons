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
using namespace std;
class rayPathParser {
public:
	DIR * directory;
	ofstream rayPath;
	std::string filename;
	rayPathParser();
	virtual ~rayPathParser();
	char * findFile();
	int renameFile();
	void parseFile();
};
rayPathParser::rayPathParser() {

	directory = opendir( "." );
	if( directory == NULL ){
		perror( "Can't open executable's directory" );
		exit( -2 );
	}

	rayPath.open("rayPath.txt",std::ios::trunc);
	if( !rayPath.is_open() ){
		perror( "Can't open rayPath.txt" );
		exit( -2 );
	}

	filename = this->findFile();
}

rayPathParser::~rayPathParser() {
	closedir(directory);
	rayPath.close();
	filename.~basic_string();
}

/*
 * Cette fonction à pour but trouver un fichier nommé rayPath.WARNING*******
 * retourne le nom du fichier ou la chaine vide si il n'existe pas
 */
char * rayPathParser::findFile() {
	std::regex re("rayPath\\.WARNING*");
	std::cmatch m;
	// Read the directory, and pull in every file that doesn't start with '.'
	struct dirent *entry;
	while( NULL != ( entry = readdir(directory) ) )
	{
		if( entry->d_name[0] != '.'  )
		{
			if(std::regex_search(entry->d_name,m,re))
				break;
		}
	}
	return entry->d_name;
}

/*
 * réécrit le fichier rayPath.txt avec la syntaxe voulut
 */
void rayPathParser::parseFile() {
	//On ouvre les deux fichiers
	//On ecrit dans rayPath
	//On lit dans reader ( rayPath.WARNING* )
	ifstream reader(filename);
	if( !reader.is_open() ){
		std::cerr << "Can't open " << filename<<"\n";
		exit( -2 );
	}

	//On elimine le blabla du début du fichier
	std::string line;
	int numlig =0;
	do{
		getline(reader,line);
	}while(line.substr(std::max<int>(line.size()- 4,0)).compare("[0,0") != 0);

	std::string path = "[0,0]";
	std::string info;
	int i,size;
	do{
		//On check si on est pas à la fin du fichier
		if(reader.eof())
			break;

		getline(reader,line);

		//On prend le dernier mot de la phrase
		stringstream ss(line);
		while (ss >> info){};

		//Si info == ] alors on est à la fin d'un chemin donc on l'écrit dans rayPath
		if( info.compare("]") == 0){
			//std::cout << path << info << "\n";
			rayPath << path << info << "\n";
			path = "";
		}else{
			//Sinon on rajoute info au chemin

			//Si path est vide alors info est une coordonnee de pixel
			//Donc on l'écrit [[x,y]
			if( path.compare("") == 0 )
				path += "[" + info.substr(0,info.size()) + "]";
			else
				path += info.substr(0,info.size());
		}

	}while(true);
	std::remove("UNKNOWN.WARNING");
	std::remove(filename.c_str());
}

#endif /* PBRT_V3_SRC_CORE_RAYPATHPARSER_HPP_ */
