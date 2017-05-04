#include "logBE.h"

std::ofstream fileBE;
OptionsBE logOptions;

void logInit(std::string filename, pbrt::Options opt){
	if(opt.log) logOptions|=LOG_LOGGING;
	if(opt.normal) logOptions|=LOG_NORMAL;
	if(opt.obj) logOptions|=LOG_OBJECT;
	if(opt.dif) logOptions|=LOG_DIFFUSE;
	if(opt.spec) logOptions|=LOG_SPECULAR;
	if(opt.path) logOptions|=LOG_PATH;
	if(opt.col) logOptions|=LOG_PATHCOLOR; 
	fileBE.open(filename);
	fileBE << "["
	<< (((logOptions & LOG_NORMAL)==LOG_NORMAL)?"n":"")
	<< (((logOptions & LOG_OBJECT)==LOG_OBJECT)?"o":"")
	<< (((logOptions & LOG_DIFFUSE)==LOG_DIFFUSE)?"d":"")
	<< (((logOptions & LOG_SPECULAR)==LOG_SPECULAR)?"s":"")
	<< (((logOptions & LOG_PATH)==LOG_PATH)?"p":"")
	<< (((logOptions & LOG_PATHCOLOR)==LOG_PATHCOLOR)?"P":"")
	<< "]\n";
}

void log(const char* text){
	fileBE << text;
}
void log(pbrt::Point3f p){
	fileBE << "(" << p[0] << "," << p[1] << "," << p[2] << ")";
}
void log(pbrt::Point2i p){
	fileBE << "[" << p[0] << "," << p[1] << "]";
}
void log(pbrt::Vector3f p){
	fileBE << "(" << p[0] << "," << p[1] << "," << p[2] << ")";
}
void log(pbrt::RGBSpectrum s){
	static float rgb[3];
	s.ToRGB(rgb);
	fileBE << "[" << rgb[0] << "," << rgb[1] << "," << rgb[2] << "]";
}

void logClose(){
	fileBE.close();
}