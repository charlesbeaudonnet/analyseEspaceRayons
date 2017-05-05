#ifndef LOGBE_H
#define LOGBE_H


/**
{[pixelX,pixelY][pointX,pointY,pointZ](dirX,dirY,dirZ)->(nX,nY,nZ)OBJ; ... ;RGB:(r,g,b)}
*/


#include <fstream>
#include "geometry.h"
#include "spectrum.h"
#include "shape.h"
#include "pbrt.h"

#define LOG_LOGGING     1
#define LOG_NORMAL      2
#define LOG_OBJECT      4
#define LOG_DIFFUSE     8
#define LOG_SPECULAR   16
#define LOG_PATH       32
#define LOG_PATHCOLOR  64

extern std::ofstream fileBE;
typedef unsigned char OptionsBE;
extern OptionsBE logOptions;

void logInit(std::string filename, pbrt::Options opt);

void log(const char* text);
void log(pbrt::Point3f p);
void log(pbrt::Point2i p);
void log(pbrt::Vector3f p);
void log(pbrt::Normal3f p);
void log(pbrt::RGBSpectrum s);
template<typename T>
void log(T data){
	if(!std::is_same<T,OptionsBE>::value)
		fileBE << data;
}
template<typename T,typename... Args>
void log(OptionsBE opt, T first, Args... args){
	if(opt & logOptions){
		log(first);log(opt,args...);
	}
}

void logClose();

#endif