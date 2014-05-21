#pragma once
#include <math.h>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <fstream>

using namespace std;

class confloader{
private:
	fstream conffile;
public:
	confloader(){
	}
	confloader(string file){
		conffile.open(file);
	}

};