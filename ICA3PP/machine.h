#pragma once
#include <vector>
#include <string>
#include <set>
#include <unordered_map>
#include <algorithm>
#include "types.h"
#include "job.h"


using namespace std;


class comparator;

class machine{
protected:
	string name;
	int ava;
	unordered_map<string,double> Vij;
	vector<task*> assigned;
public:
	machine(){}
	machine(string nm,int avtime) : name(nm),ava(avtime){
		
	}
	machine(string nm,int avt,unordered_map<string,double>&vij, vector<task*> as) : name(nm),ava(avt){
		Vij = vij;
		assigned = as;
	}
	machine(const machine& m){
		name = m.name;
		ava = m.ava;
		assigned = m.assigned;
		Vij = m.Vij;
	}
	virtual ~machine(){}

	machine & operator= (const machine &m)
	{
		if(&m==this)
			return *this;
		name = m.name;
		ava = m.ava;
		assigned = m.assigned;
		Vij = m.Vij;
		return *this;
	}
	//return ava
	virtual int assigntask(task &t) = 0;
	
	int getAvaiableTime(){
		return ava;
	}
	void loadSpeedFactor(unordered_map<string,double> &vij);

	const vector<task*> & getassigned(){
		return assigned;
	}
	string getname() const{
		return name;
	}
	int getavg(){
		return ava;
	}
	const unordered_map<string,double>& getVij(){
		return Vij;
	}
};

class mapMachine : public machine
{
private:
	vector<task*> sequenced;
	bool hassequenced;
public:
	mapMachine(){
		hassequenced = false;
	}
	mapMachine(mapMachine & mm) : machine(mm.name,mm.ava,mm.Vij,mm.assigned){
		hassequenced = mm.hassequenced;
		sequenced = mm.sequenced;
	}
	~mapMachine(){}
	void sequencing();

	vector<task*> & getsequenced(){
		if(!hassequenced)
		{	
			sequencing();
		}
		return sequenced;
	}
	int assigntask(task &t);
};

class redMachine : public machine
{
public:
	redMachine(){}
	~redMachine(){}

	redMachine(redMachine& rm) : machine(rm.name,rm.ava,rm.Vij,rm.assigned){
	}

	int assigntask(task &t);

};
