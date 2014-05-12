#pragma once
#include <vector>
#include <map>
#include "job.h"
#include "scheduler.h"
using namespace std;

class testcase{
public:
	int id;
	int min_min_time;
	vector<job> min_min_jobsumary;
	int min_max_time;
	vector<job> min_max_jobsumary;
	int FIFO_time;
	vector<job> fifo_jobsumary;
	void collectResults(scheduler *s);

};
class resultcollector{
private:
	vector<testcase> results;
};