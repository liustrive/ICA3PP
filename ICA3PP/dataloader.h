#pragma once
#include "jobtracker.h"
#include "conf.h"
using namespace std;


//this class generate data due to the configure, generate data according to the scale and percentage of conf
class dataloader{
private:
	vector<confSet> *confs;
	vector<job*> jobs;
	vector<mapMachine*> mm;
	vector<redMachine*> rm;
	jobtracker * jt;

public:
	dataloader(){
		confs = new vector<confSet>();
		defaultConf();
		generatedata();
	}
	virtual ~dataloader(){
	}
	void defaultConf();
	void generatedata();

	void run();
};