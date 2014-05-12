#include "machine.h"

void mapMachine::sequencing(){
	
	sequenced.assign(assigned.begin(),assigned.end());
	// sort tasks by pri nondecreasing
	sort(sequenced.begin(),sequenced.end(),comparator());
	hassequenced = true;
}

int mapMachine::assigntask(task &t){
	int processtime = t.endtime-t.starttime;
	ava+=processtime;
	assigned.push_back(&t);
	return ava;
}

int redMachine::assigntask(task &t){

	ava = t.endtime;
	assigned.push_back(&t);
	return ava;
}