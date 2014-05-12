#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include "job.h"
#include "machine.h"
#include "cluster.h"
#include "resultcollector.h"
#include "scheduler.h"
using namespace std;

class jobtracker{
private:
	vector<job*> jobqueue;
	vector<mapMachine*> mapM;
	vector<redMachine*> redM;
	testcase tc;
public:
	jobtracker(){}
	virtual ~jobtracker(){}
	jobtracker(const vector<job> & jobs,const vector<mapMachine*> &mm,const vector<redMachine*> & rm){
		jobqueue.assign(jobs.begin(),jobs.end());
		mapM.assign(mm.begin(),mm.end());
		redM.assign(rm.begin(),rm.end());
	}
	void assignJobs(scheduler* s,testcase & tc);

};