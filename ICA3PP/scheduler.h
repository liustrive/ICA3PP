#pragma once
#include <unordered_set>
#include <algorithm>
#include "job.h"
#include "machine.h"

using namespace std;

class scheduler{
protected:
	multimap<int,machine*> m_ava;
	multimap<int,machine*> r_ava;
	vector<mapMachine*> sequencedMapMachines;
	vector<job*> ordered_jobs;
public:
	scheduler(){}
	virtual ~scheduler(){}
	virtual void assignMaps(vector<job*> & j,vector<mapMachine*> & mm) = 0;
	virtual void assignReds(vector<redMachine*> & rm) = 0;
	void updateava(multimap<int,machine*> & ava, machine * m,int new_ava);
};

class min_min_scheduler : public scheduler
{
public:
	virtual void assignMaps(vector<job*> & j,vector<mapMachine*> & mm);
	void assignReds(vector<redMachine*> & rm);
};

class min_max_scheduler : public min_min_scheduler
{
public:
	void assignMaps(vector<job*> & j,vector<mapMachine*> & mm);

};

class fifo_scheduler : public scheduler
{
public:
	void assignMaps(vector<job*> & j,vector<mapMachine*> & mm);
	void assignReds(vector<redMachine*> & rm);
};