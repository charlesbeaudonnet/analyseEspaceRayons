#ifndef LOGBE_H
#define LOGBE_H
/**
{[pixelX,pixelY][pointX,pointY,pointZ](dirX,dirY,dirZ)->(nX,nY,nZ)OBJ; ... ;RGB:(r,g,b)}
*/
#include <iostream>
#include <map>

#include <fstream>
#include "geometry.h"
#include "spectrum.h"
#include "shape.h"
#include "pbrt.h"

#define LOG_LOGGING		1
#define LOG_NORMAL		2
#define LOG_OBJECT		4
#define LOG_DIFFUSE		8
#define LOG_SPECULAR	16
#define LOG_PATH		32
#define LOG_PATHDIR		64

//namespace LogBE{

extern std::ofstream fileBE;
typedef unsigned char OptionsBE;
extern OptionsBE logOptions;
extern std::map<const pbrt::Shape*,int> shapeMap;

void logInit(std::string filename, pbrt::Options opt);

template<typename T>
inline void log(T data){
	if(!std::is_same<T,OptionsBE>::value)
		fileBE << data;
}
template <>
inline void log(const pbrt::Shape* s){
	if(shapeMap.find(s) == shapeMap.end()){
		shapeMap[s]=shapeMap.size();
	}
	fileBE << shapeMap[s];
}
template<typename T,typename... Args>
inline void log(OptionsBE opt, T first, Args... args){
	if(opt & logOptions){
		log(first);log(opt,args...);
	}
}

void logClose();
//}
#endif