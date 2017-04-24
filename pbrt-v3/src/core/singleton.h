/*
 * singleton.h
 *
 *  Created on: 24 avr. 2017
 *      Author: 21303468
 */

#ifndef SINGLETON_H_
#define SINGLETON_H_
#include <fstream>
#include <string>

#define PINFO Singleton::getInstance()->getOfstream()

class Singleton {

public:
	std::ofstream str;
	std::string filename;
	Singleton(std::string filename);
	void closeFile();
	void reopenFile();
	static void replaceInstance(Singleton* sglt);
	static Singleton* getInstance();
	static Singleton* createInstance(std::string filename);
	static void destroyInstance();
	std::ofstream getOfstream();

protected:
	Singleton(const Singleton&) = delete;
	void operator=(const Singleton&) = delete;

};
#endif /* SINGLETON_H_ */
