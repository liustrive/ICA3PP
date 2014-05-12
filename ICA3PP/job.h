#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <map>
#include <sstream>
#include "task.h"
#include "machine.h"

using namespace std;

class job{
private:
	const string jobname;
	const int maptasknum;
	const int maptasktime;
	const int redtasknum;
	const int redtasktime; 
	int avgmaptime;
	int avgredtime;
	double pri;

	int runningmaptask;
	int runningredtask;



	unordered_map<machine*,int> Vij_m_time;
	unordered_map<machine*,int> Vij_r_time;
//	vector<task> maptasks;
	vector<task> pendingmap;
	vector<task> finishedmap;
//	vector<task> redtasks;
	vector<task> pendingred;
	vector<task> finishedred;
public:
	int starttime;
	int mapDoneTime;
	int finishtime;
	job(string name,int mapnum,int maptime,int rednum,int redtime,unordered_map<machine*,double> &vij_m,unordered_map<machine*,double> & vij_r) : jobname(name), maptasknum(mapnum),maptasktime(maptime), redtasknum(rednum),redtasktime(redtime){
		starttime = -1;
		//set avgmap(red)time , set pri, set vij_m, set vij_r;
		computePri(vij_m,vij_r);
		runningredtask = runningmaptask = 0;
		for (int i = 0;i<mapnum;i++)
		{
			stringstream ss(name);
			ss<<"_m_"<<i;
			task *t = new task(ss.str(),this,maptime,MAP,pri);
			pendingmap.push_back(*t);
		}
		for (int i = 0;i<rednum;i++)
		{
			stringstream ss(name);
			ss<<"_r_"<<i;
			task *t = new task(ss.str(),this,redtime,REDUCE,pri);
			pendingred.push_back(*t);
		}
	}
	virtual ~job(){
	}

	void computePri(unordered_map<machine*,double> &, unordered_map<machine*,double> &);
	int computeAvg(unordered_map<machine*,double>&,Type);
	int getMinMapTime(multimap<int,machine*> & machineAva, machine &);
	int getMinRedTime(multimap<int,machine*> & machineAva, machine &);
	task & assignTasktoMachine(machine & m,int starttime,Type tp);
	bool mapdone() const
	{
		return pendingmap.size()==0;
	}
	bool reddone() const
	{
		return pendingred.size()==0;
	}
	void computeMapDoneTime(){
		vector<task>::const_iterator it = finishedmap.begin();
		int max_fin = it->endtime;
		for (++it;it!=finishedmap.end();++it)
		{
			if(it->endtime > max_fin){
				max_fin = it->endtime;
			}
		}
		cout<<"job: "<<jobname<<" map stage done at "<<max_fin<<endl;
		mapDoneTime = max_fin;
	}
	int getMapDoneTime() const{
		if (!mapdone())
		{
			return -1;
		}
				
		return mapDoneTime;
	}
	int getmapnum(){
		return maptasknum;
	}
	int getmaptime(){
		return maptasktime;
	}
	int getrednum(){
		return redtasknum;
	}
	int getredtime(){
		return redtasktime;
	}
	void addfinishedMap(task & t){
		finishedmap.push_back(t);
		if (finishedmap.size()==maptasknum)
		{
			computeMapDoneTime();
		}
	}
	int getProcessTime(machine* m, Type tp);
};