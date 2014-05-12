#include "jobtracker.h"
/*
void assignJobs(min_min_scheduler s);
void assignJobs(min_max_scheduler s);
void assignJobs(fifo_scheduler s);
*/

void jobtracker::assignJobs(scheduler *s,testcase & tc){
	s->assignMaps(jobqueue,mapM);
	s->assignReds(redM);
	
}