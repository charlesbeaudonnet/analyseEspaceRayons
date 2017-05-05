#include "logBE.h"
#include <map>

std::ofstream fileBE;
OptionsBE logOptions;

std::map<pbrt::Shape*,int> shapeMap;

void logInit(std::string filename, pbrt::Options opt){
	if(opt.log) logOptions|=LOG_LOGGING;
	if(opt.normal) logOptions|=LOG_NORMAL;
	if(opt.obj) logOptions|=LOG_OBJECT;
	if(opt.dif) logOptions|=LOG_DIFFUSE;
	if(opt.spec) logOptions|=LOG_SPECULAR;
	if(opt.path) logOptions|=LOG_PATH;
	if(opt.col) logOptions|=LOG_PATHCOLOR; 
	fileBE.open(filename);
	fileBE << "[";
	log(LOG_NORMAL, "n");
	log(LOG_OBJECT, "o");
	log(LOG_DIFFUSE, "d");
	log(LOG_SPECULAR, "s");
	log(LOG_PATH, "p");
	log(LOG_PATHCOLOR, "P");
	fileBE << "]\n";
	shapeMap.clear();
}

void log(const char* text){
	fileBE << text;
}
void log(pbrt::Point3f p){
	fileBE << "[" << p[0] << "," << p[1] << "," << p[2] << "]";
}
void log(pbrt::Point2i p){
	fileBE << "[" << p[0] << "," << p[1] << "]";
}
void log(pbrt::Vector3f p){
	fileBE << "(" << p[0] << "," << p[1] << "," << p[2] << ")";
}
void log(pbrt::Normal3f p){
	fileBE << "(" << p[0] << "," << p[1] << "," << p[2] << ")";
}
void log(pbrt::RGBSpectrum s){
	static float rgb[3];
	s.ToRGB(rgb);
	fileBE << "[" << rgb[0] << "," << rgb[1] << "," << rgb[2] << "]";
}
void log(pbrt::Shape *s){
	if(shapeMap.find(s) == shapeMap.end()){
		printf("vroum\n");
		shapeMap[s]=7;
	}
	fileBE << shapeMap[s];
}

void logClose(){
	fileBE.close();
}