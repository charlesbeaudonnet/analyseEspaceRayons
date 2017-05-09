#include "logBE.h"
#include <map>
#include <iostream>
std::ofstream fileBE;
OptionsBE logOptions;

std::map<const pbrt::Shape*,int> shapeMap;

void logInit(std::string filename, pbrt::Options opt){
	if(opt.log) logOptions|=LOG_LOGGING;
	if(opt.normal) logOptions|=LOG_NORMAL;
	if(opt.obj) logOptions|=LOG_OBJECT;
	if(opt.dif) logOptions|=LOG_DIFFUSE;
	if(opt.spec) logOptions|=LOG_SPECULAR;
	if(opt.path) logOptions|=LOG_PATH;
	if(opt.dir) logOptions|=LOG_PATHDIR; 
	fileBE.open(filename);
	fileBE << "[";
	log(LOG_NORMAL, "n");
	log(LOG_OBJECT, "o");
	log(LOG_DIFFUSE, "d");
	log(LOG_SPECULAR, "s");
	log(LOG_PATH, "p");
	log(LOG_PATHDIR, "P");
	fileBE << "]\n";
	shapeMap.clear();
}

void logClose(){
	fileBE.close();
}