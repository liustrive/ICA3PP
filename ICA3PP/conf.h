#pragma once
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>

using namespace std;
// joblength for map/red task time, jobtype for job task num, machine factor type for different speed factor machine
// percent, 
enum confType{
	JOBLENGTH,
	JOBTYPE,
	MACHINE_FACTOR_TYPE,
	MAP_RED_PERCENTAGE,

};

class confElement{
public:
	const string confName;
	double scaleUP;
	double scaleBOTTOM;
	double confValue;
	bool operator==(const confElement & e) const{
		return (e.confName==this->confName) && (e.confValue==this->confValue);
	}
	confElement(string cn, double cv, double su, double sb) : confName(cn),confValue(cv),scaleUP(su),scaleBOTTOM(sb){}
};

class confSet{
public:
	vector<confElement> *elements;
	confType type;
	string setname;
	double scaleUP;
	double scaleBOTTOM;
	confSet(string name,vector<confElement> &ce, confType t, double up, double bottom) : setname(name),type(t),scaleUP(up),scaleBOTTOM(bottom){
		elements = &ce;
	}
	// This constructor divide scale averagely, and each scale's value due to percentage
	// please make sure that no element in this vector is zero
	confSet(string name,vector<double> & percentage, confType t,double up, double bottom) : setname(name),type(t),scaleUP(up),scaleBOTTOM(bottom){
		double sum = 0;
		for (size_t i = 0;i<percentage.size();++i)
		{
			sum+=percentage[i];
		}
		int scalenum = percentage.size();
		//divide scale averagely
		const double addup = (scaleUP - scaleBOTTOM)/scalenum;
		double scalesplit = scaleUP;
		for (size_t	i = 0;i<scalenum;i++)
		{
			stringstream ss(name);
			ss<<"_"<<scalesplit<<"-"<<scalesplit+addup;
			confElement ce(ss.str(),percentage[i]/sum,scalesplit,scalesplit+addup);
			scalesplit+=addup;
			elements->push_back(ce);
		}
	}
};
