#ifndef LOGBE_H
#define LOGBE_H

#include <fstream>
#include "geometry.h"
#include "spectrum.h"
#include "pbrt.h"

#define LOG_LOGGING     1
#define LOG_NORMAL      2
#define LOG_OBJECT      4
#define LOG_DIFFUSE     8
#define LOG_SPECULAR   16
#define LOG_PATH       32
#define LOG_PATHCOLOR  64

extern std::ofstream fileBE;
extern unsigned char logOptions;

void logInit(const char* filename, pbrt::Options opt);

void log(const char* text);
void log(pbrt::Point3f p);
void log(pbrt::Point2i p);
void log(pbrt::Vector3f p);
void log(pbrt::RGBSpectrum s);
template<typename T>
void log(T data){
	fileBE << data;
}
template<typename... Args>
void log(unsigned char opt, Args... args){
	if(opt == (opt & logOptions))
		log(args...);
}
template<typename T,typename... Args>
void log(T first, Args... args){
	log(first); log(args...);
}

void logClose();

#endif