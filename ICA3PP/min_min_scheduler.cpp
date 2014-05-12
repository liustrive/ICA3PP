#include "scheduler.h"

void scheduler::updateava(multimap<int,machine*> & ava, machine * m,int new_ava){
	multimap<int,machine*>::iterator it = ava.begin();
	for (;it!=ava.end();++it)
	{
		if (it->second==m)
		{
			ava.insert(make_pair(new_ava,it->second));
			ava.erase(it);
			break;
		}
	}
}
void min_min_scheduler::assignMaps(vector<job*> & j,vector<mapMachine*> & mm){
	//set all map machines ava to 0
	for (size_t i = 0;i<mm.size();++i)
	{
		m_ava.insert(pair<int,machine*>(0,mm[i]));
	}
	vector<job*> jobque(j.begin(),j.end());

	//heuristic min-min
	while(!jobque.empty()){
		
		size_t i = 0;
		mapMachine fitmachine;
		vector<job*>::iterator it = jobque.begin();
		vector<job*>::iterator minjob = it;
		int min_time = (*it)->getMinMapTime(m_ava,fitmachine);
		for (++it;it!=jobque.end();++it)
		{
			mapMachine mc;
			int min = jobque[i]->getMinMapTime(m_ava,mc);
			if(min < min_time)
			{
				minjob = it;
				min_time = min;
				fitmachine = mc;
			}
		}
		task t = (*minjob)->assignTasktoMachine(fitmachine,fitmachine.getAvaiableTime(),MAP);
		int ava = fitmachine.assigntask(t);
		updateava(m_ava,&fitmachine,ava);
		// if job's map stage is finished, remove from queue
		if ((*minjob)->mapdone())
		{
			jobque.erase(minjob);
		}
	}
	// min-min done

	// sequencing
	for (multimap<int,machine*>::iterator it = m_ava.begin();it!=m_ava.end();++it)
	{
		mapMachine * mmc = dynamic_cast<mapMachine*>(it->second);
		if (mmc==NULL)
		{
			cout<<"cast fail during sequencing of min_min"<<endl;
		}
		mmc->sequencing();
		sequencedMapMachines.push_back(mmc);
	}
		// update task time and job time after sequenced
	for (vector<mapMachine*>::iterator it = sequencedMapMachines.begin();it!=sequencedMapMachines.end();++it)
	{
		int timeclock = 0;
		for (vector<task*>::iterator tit = (*it)->getsequenced().begin();tit!=(*it)->getsequenced().end();++tit)
		{
			int processtime = (*tit)->endtime-(*tit)->starttime;
			(*tit)->starttime = timeclock;
			if(timeclock < (*tit)->j_ptr->starttime)
			{
				(*tit)->j_ptr->starttime = timeclock;
			}
			(*tit)->endtime = timeclock+processtime;
			(*tit)->j_ptr->addfinishedMap(**tit);
			timeclock+=processtime;

		}
	}
		// maintain the ordered jobs by nondecreasing mapfinishedtime (Ai)
	ordered_jobs.assign(j.begin(),j.end());
	sort(ordered_jobs.begin(),ordered_jobs.end(),comparator());

	// map stage done
}

void min_min_scheduler::assignReds(vector<redMachine*> & rm){
	// set all red machine ava time to zero
	for (size_t i = 0;i<rm.size();++i)
	{
		r_ava.insert(pair<int,machine*>(0,rm[i]));
	}
	// already ordered by map finish time
	vector<job*> jobs(ordered_jobs.begin(),ordered_jobs.end());
	
	vector<job*> job_to_dispatch;
	// job with minimum Ai
	while(!jobs.empty() || !job_to_dispatch.empty()){
		// EAT <- min(Ava_i)
		int EVA = r_ava.begin()->first;
		for (vector<job*>::iterator it = jobs.begin();it!=jobs.end();)
		{
			if ((*it)->getMapDoneTime()<EVA)
			{
				jobs.erase(it++);
				job_to_dispatch.push_back(*it);
			}
			else
				it++;
		}
		if (job_to_dispatch.empty())
		{
			job * j = jobs.front();
			jobs.erase(jobs.begin());
			job_to_dispatch.push_back(j);
		}
		vector<job*>::iterator itt = job_to_dispatch.begin();
		vector<job*>::iterator min_job = itt;
		redMachine rdm;
		int min_time = (*itt)->getMinRedTime(r_ava,rdm);
		for (itt++;itt!=job_to_dispatch.end();itt++)
		{
			redMachine tmp;
			int mtime = (*itt)->getMinRedTime(r_ava,rdm);
			if (min_time > mtime)
			{
				rdm = tmp;
				min_time = mtime;
				min_job = itt;
			}
		}
		task t = (*min_job)->assignTasktoMachine(rdm,max((*min_job)->getMapDoneTime(),rdm.getAvaiableTime()),REDUCE);
		int ava = rdm.assigntask(t);
		updateava(r_ava,&rdm,ava);
		if ((*min_job)->reddone())
		{
			job_to_dispatch.erase(min_job);
		}

	}




}