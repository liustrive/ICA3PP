#include "scheduler.h"
// the order of jobs should be decided outside
void fifo_scheduler::assignMaps(vector<job*> & j,vector<mapMachine*> & mm){
	
	// set all map machine ava to zero
	for (size_t i =0;i<mm.size();i++)
	{
		m_ava.insert(pair<int,machine*>(0,mm[i]));
	}

	// assign all jobs' tasks in the original order
	vector<job*> jobque(j.begin(),j.end());
	while(!jobque.empty()){
		job * first_job = jobque.front();
		multimap<int,machine*>::iterator first_m_ava = m_ava.begin();
		task t = first_job->assignTasktoMachine(*(first_m_ava->second),first_m_ava->first,MAP);
		int new_ava = first_m_ava->second->assigntask(t);
		updateava(m_ava,first_m_ava->second,new_ava);

		first_job->addfinishedMap(t);
		if (first_job->mapdone())
		{
			jobque.erase(jobque.begin());
		}

	}
	// order job in nondecreasing order of map finish time (Ai)
	ordered_jobs.assign(j.begin(),j.end());
	sort(ordered_jobs.begin(),ordered_jobs.end(),comparator());
}


void fifo_scheduler::assignReds(vector<redMachine*> & rm){
	// set all red machine ava time to zero
	for (size_t i = 0;i<rm.size();++i)
	{
		r_ava.insert(pair<int,machine*>(0,rm[i]));
	}

	// already ordered by map finish time
	vector<job*> jobs(ordered_jobs.begin(),ordered_jobs.end());
	
	// for each loop, pick job from the queue front, then:
	// if there are some machines available by the time map tasks finished, assign to the machine which is the slowest;
	// else just assign to the first machine available.

	job* first_job = jobs.front();
	
	while(!jobs.empty()){
		//find all available machines by the earliest map tasks finish time, and select a slowest one;
		int A_min = first_job->getMapDoneTime();
		int max_time = 0;
		machine * fitmachine = NULL;
		for (multimap<int,machine*>::iterator it = r_ava.begin();it!=r_ava.end();it++)
		{
			if (it->first < A_min)
			{
				int process_time = first_job->getProcessTime(it->second,REDUCE);
				if (process_time > max_time)
				{
					fitmachine = it->second;
					max_time = process_time;
				}
			}
			else
				break;
		}
		if (fitmachine==NULL)
		{
			// assign to a earliest available machine
			multimap<int,machine*>::iterator first_ava_red = r_ava.begin();
			machine * rdm = first_ava_red->second;
			
			task t = first_job->assignTasktoMachine(*rdm,rdm->getAvaiableTime(),REDUCE);
			int ava = rdm->assigntask(t);
			updateava(r_ava,rdm,ava);
			if (first_job->reddone())
			{
				jobs.erase(jobs.begin());
			}
		}
		else{
			task t = first_job->assignTasktoMachine(*fitmachine,A_min,REDUCE);
			int ava = fitmachine->assigntask(t);
			updateava(r_ava,fitmachine,ava);
			if (first_job->reddone())
			{
				jobs.erase(jobs.begin());
			}
		}
		
	}

}