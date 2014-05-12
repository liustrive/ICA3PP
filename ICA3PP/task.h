#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>
#include "types.h"

using namespace std;
class job;

class task{
public:
	const string taskid;
	job * j_ptr;
	const int tasktime;
	const double pri;
	int starttime;
	int endtime;

	Type type;
	task(string tid, job* j, int ttime,Type tp,double p) : taskid(tid), j_ptr(j),tasktime(ttime),type(tp),pri(p){

	}
	task(const task& t) : taskid(t.taskid),j_ptr(t.j_ptr),tasktime(t.tasktime),type(t.type),pri(t.pri){}

	~task(){}
	// sequence task in pri nondecreasing order
	bool operator< (const task & t)const
	{
		return pri < t.pri;
	}
	task & operator= (const task & t){
		if(this==&t)
			return *this;
		new (this)task(t);
		return *this;
	}
};
