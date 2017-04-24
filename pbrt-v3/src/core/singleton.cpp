/*
 * singleton.cpp
 *
 *  Created on: 24 avr. 2017
 *      Author: 21303468
 */
#include "singleton.h"

namespace sglt {
Singleton* s_instance = nullptr;
}
Singleton::Singleton(std::string filename){
	str.open(filename.c_str(),std::ofstream::in | std::ofstream::out | std::ofstream::trunc);
	if(!str){
		std::cerr << "Impossible d'ouvrir le fichier" << filename;
		return;
	}
	this->filename = filename;
}
void closeFile(){
    str.close();
}

void reopenFile(){
    str.open(filename.c_str(),std::ios::in | std::ios::out | std::ios::app);//filename est une variable de la classe
}

Singleton* createInstance(std::string filename)
{
    replaceInstance(new Singleton(filename));
    return getInstance();
}
void Singleton::replaceInstance(singleton* p) {
    delete sglt::s_instance;
    sglt::s_instance = p;
}

Singleton* Singleton::getInstance() {
    return sglt::s_instance;
}

void Singleton::destroyInstance() {
    delete sglt::s_instance;
    sglt::s_instance = nullptr;
}

std::ofstream getOfstream(){
    return this.str;
}
