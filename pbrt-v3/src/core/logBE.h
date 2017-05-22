#ifndef LOGBE_H
#define LOGBE_H
/**
{[pixelX,pixelY]p[pointX,pointY,pointZ]d(dirX,dirY,dirZ)N(nX,nY,nZ)OxC(r,g,b)}
*/
#include <iostream>

#include <fstream>
#include "geometry.h"
#include "spectrum.h"
#include "shape.h"
#include "pbrt.h"

#define LOG_LOGGING		1<<0
#define LOG_NORMAL		1<<1
#define LOG_OBJECT		1<<2
#define LOG_MATERIAL	1<<3
#define LOG_PATH		1<<4
#define LOG_PATHDIR		1<<5

namespace logBE{

extern std::ofstream fileBE;
typedef unsigned char OptionsBE;
extern OptionsBE logOptions;
extern std::vector<const pbrt::Shape*> shapesBE;

/**
	Cette méthode prend en paramètre une chaine de caractères et les
	options de PBRT et initialise la variable logOptions, ouvre un nouveau
	fichier avec le nom passé en paramètre, met les options choisies
	à la première ligne du fichier et puis initialise le vecteur shapeBE
	qui contiendra tous les objets de la scène.
*/
void logInit(std::string filename, pbrt::Options opt);

template<typename T>
inline void log(T data){
	if(!std::is_same<T,OptionsBE>::value)
		fileBE << data;
}
template <>
inline void log(pbrt::RGBSpectrum s){
	static float rgb[3];
	s.ToRGB(rgb);
	fileBE << "[" << rgb[0] << "," << rgb[1] << "," << rgb[2] << "]";
}
template <>
inline void log(std::shared_ptr<pbrt::Material> s){
	(*s).operator<<(fileBE);
}
template <>
inline void log(const pbrt::Shape* s){
	unsigned i=0;
	for(;i<shapesBE.size();i++)
		if(shapesBE.at(i)==s)
			break;
	if(i!=shapesBE.size()) fileBE << i+1;
}
template<typename T,typename... Args>
inline void log(OptionsBE opt, T first, Args... args){
	if(opt & logOptions){
		log(first);log(opt,args...);
	}
}

/**
	Fermer le fichier log, vider la liste des objets de la scène
	et mettre les options log à 0.
*/
void logClose();

}// namespace
#endif