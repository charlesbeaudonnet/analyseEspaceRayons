#include "logBE.h"
#include <map>
#include <iostream>
std::ofstream fileBE;
OptionsBE logOptions;
std::vector<const pbrt::Shape*> shapesBE;

void logInit(std::string filename, pbrt::Options opt){
	if(opt.log) logOptions|=LOG_LOGGING;
	if(opt.normal) logOptions|=LOG_NORMAL;
	if(opt.obj) logOptions|=LOG_OBJECT;
	if(opt.mat) logOptions|=LOG_MATERIAL;
	if(opt.path) logOptions|=LOG_PATH;
	if(opt.dir) logOptions|=LOG_PATHDIR;
	fileBE.open(filename);
	fileBE << "[";
	log(LOG_NORMAL, "n");
	log(LOG_OBJECT, "o");
	log(LOG_MATERIAL, "m");
	log(LOG_PATH, "p");
	log(LOG_PATHDIR, "P");
	fileBE << "]\n";
	shapesBE.clear();
}

void logClose(){
	fileBE.close();
}