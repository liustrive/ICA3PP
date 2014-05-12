#pragma once
#include "task.h"
#include "job.h"


class comparator{
public:
	// nondecreasing order pri comparator of tasks
	bool operator() (const task* a, const task* b){
		return a->pri < b->pri;
	}
	// nondecreasing order mapfinishtime comparator of jobs
	bool operator() (const job* a,const job* b){
		return a->getMapDoneTime() < b->getMapDoneTime();
	}

	bool operator() (const machine * a, const machine *b){
		return a->getname() < b->getname();
	}
};