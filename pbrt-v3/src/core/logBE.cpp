#include "logBE.h"
#include <map>
#include <iostream>

std::ofstream logBE::fileBE;
logBE::OptionsBE logBE::logOptions;
std::vector<const pbrt::Shape*> logBE::shapesBE;

void logBE::logInit(std::string filename, pbrt::Options opt){
	if(opt.log) logBE::logOptions|=LOG_LOGGING;
	if(opt.normal) logBE::logOptions|=LOG_NORMAL;
	if(opt.obj) logBE::logOptions|=LOG_OBJECT;
	if(opt.mat) logBE::logOptions|=LOG_MATERIAL;
	if(opt.path) logBE::logOptions|=LOG_PATH;
	if(opt.dir) logBE::logOptions|=LOG_PATHDIR;
	logBE::fileBE.open(filename);
	logBE::fileBE << "[";
	logBE::log(LOG_NORMAL, "n");
	logBE::log(LOG_OBJECT, "o");
	logBE::log(LOG_MATERIAL, "m");
	logBE::log(LOG_PATH, "p");
	logBE::log(LOG_PATHDIR, "P");
	logBE::fileBE << "]\n";
	logBE::shapesBE.clear();
}

void logBE::logClose(){
	logBE::fileBE.close();
}