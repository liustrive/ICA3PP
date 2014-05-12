#include "job.h"

int job::computeAvg(unordered_map<machine*,double> &vij,Type tp){
	unordered_map<machine*,double>::iterator it = vij.begin();
	int M_num = vij.size();
	
	int ret = 0;
	if(tp==MAP){
		double factorsum = 0;
		for(;it!=vij.end();++it){
			Vij_m_time[it->first] = it->second * maptasktime;
			factorsum+=it->second;
		}
		ret = (maptasktime*factorsum)/(M_num*maptasknum);
	}
	else{
		double factorsum = 0;
		for(;it!=vij.end();++it){
			Vij_r_time[it->first] = it->second * redtasktime;
			factorsum+=it->second;
		}
		ret = (redtasktime*factorsum)/(M_num*redtasknum);
	}
	return ret;
}

void job::computePri(unordered_map<machine*,double> & vij_m, unordered_map<machine*,double> & vij_r){

	avgmaptime = computeAvg(vij_m,MAP);
	avgredtime = computeAvg(vij_r,REDUCE);
	pri = (avgmaptime>avgredtime)? ((avgmaptime-avgredtime)/avgredtime) : ((avgredtime-avgmaptime)/avgmaptime);

}

int job::getMinMapTime(multimap<int,machine*> & machineAva, machine & mc){
	multimap<int,machine*>::iterator it = machineAva.begin();
	int earlist = Vij_m_time[it->second] + it->first;
	machine * fitmachine = it->second;
	for(++it;it!=machineAva.end();++it)
	{
		int av = Vij_m_time[it->second] + it->first;
		if (av < earlist)
		{
			earlist = av;
			fitmachine = it->second;
		}
		// no need to compute min-time if available time even later.
		if (it->first > earlist)
		{
			break;
		}
	}
	mc = *fitmachine;
	return earlist;
}


int job::getMinRedTime(multimap<int,machine*> & machineAva, machine & mc){
	multimap<int,machine*>::iterator it = machineAva.begin();
	int earlist = Vij_r_time[it->second] + max(it->first,getMapDoneTime());
	machine * fitmachine = it->second;
	for(++it;it!=machineAva.end();++it)
	{
		int av = Vij_r_time[it->second] + max(it->first,getMapDoneTime());
		if (av < earlist)
		{
			earlist = av;
			fitmachine = it->second;
		}
		// no need to compute min-time if available time even later.
		if (it->first > earlist)
		{
			break;
		}
	}
	mc = *fitmachine;
	return earlist;
}

task& job::assignTasktoMachine(machine & m,int starttime,Type tp){
	if (tp==MAP)
	{
		task t = pendingmap.back();
		pendingmap.pop_back();
		t.starttime = starttime;
		if(this->starttime==-1)
			this->starttime = starttime;
		t.endtime = starttime + Vij_m_time[&m];
		runningmaptask++;
		return t;
	}
	else{
		task t = pendingred.back();
		pendingred.pop_back();
		t.starttime = starttime;
		t.endtime = starttime + Vij_r_time[&m];
		runningredtask++;
		finishedred.push_back(t);
		return t;
	}
}

int job::getProcessTime(machine* m, Type tp){
	if (tp==MAP)
	{
		return Vij_m_time[m];
	}
	else
	{
		return Vij_r_time[m];
	}
}