#include "scheduler.h"

//void updateava(multimap<int,machine*> & ava, machine * m,int new_ava){
//	multimap<int,machine*>::iterator it = ava.begin();
//	for (;it!=ava.end();++it)
//	{
//		if (it->second==m)
//		{
//			ava.insert(make_pair(new_ava,it->second));
//			ava.erase(it);
//			break;
//		}
//	}
//}
void min_max_scheduler::assignMaps(vector<job*> & j,vector<mapMachine*> & mm){
	//set all map machines ava to 0
	for (size_t i = 0;i<mm.size();++i)
	{
		m_ava.insert(pair<int,machine*>(0,mm[i]));
	}
	vector<job*> jobque(j.begin(),j.end());

	//heuristic min-max
	while(!jobque.empty()){

		size_t i = 0;
		mapMachine fitmachine;
		vector<job*>::iterator it = jobque.begin();
		vector<job*>::iterator minjob = it;
		int max_time = (*it)->getMinMapTime(m_ava,fitmachine);
		for (++it;it!=jobque.end();++it)
		{
			mapMachine mc;
			int max = jobque[i]->getMinMapTime(m_ava,mc);
			if(max > max_time)
			{
				minjob = it;
				max_time = max;
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
	// min-max done

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