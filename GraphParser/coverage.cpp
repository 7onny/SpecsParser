#include"coverage.h"

#include"dotUtilities.h"

testCase::testCase(int id, string name, string filename): id(id), name(name), filename(filename){}

int testCase::getId(){
	return id;
}

testCase& testCase::setId(int id){
	this->id=id;
	return *this;
}

string testCase::getFilename(){
	return filename;
}

testCase& testCase::setFilename(string name){
	this->filename=filename;
	return *this;
}

string testCase::getName(){
	return name;
}

testCase& testCase::setName(string name){
	this->name=name;
	return *this;
}

int testCase::getSize(){
	return t.size();
}

void testCase::addTransition(transition *tr){
	t.push_back(tr);
}

void testCase::printTestCase(state **s, vector<transition*> *t){
	ofstream out(this->filename, ios::trunc);
	printHeader(this->name,out);
	printFml(s,t,out);
	vector<transition*>::iterator it;
	for(it=(this->t).begin(); it!=(this->t).end(); it++)
		out<<(**it);
	wrapUp(out);
}

bool testCase::searchTransition(transition *target){
	for(vector<transition*>::iterator it=t.begin(); it!=t.end(); it++)
		if((**it)==target)
			return true;
	return false;
}

float testCase::checkTransitionCoverage(vector<transition*> *mt){
	float coverage;
	bool covered[TRANSITIONS];
	for(int i=0; i<TRANSITIONS; ++i) covered[i]=false;

	vector<transition*>::iterator i;
	vector<transition*>::iterator j;
	int index;
	for(i=mt->begin(), index=0; i!=mt->end(); i++, index++)
		for(j=t.begin(); j!=t.end(); j++)
			if((**i)==(*j)){
				covered[index]=true;
			}	
	int count=0;
	for(int i=0; i<TRANSITIONS; ++i)
		if(covered[i])
			count++;
	coverage=(float)count/TRANSITIONS;
	return coverage;
}


testSet::testSet(){}

int testSet::getSize(){
	return ts.size();
}

void testSet::addTestCase(testCase *tc){
	ts.push_back(tc);
}

float testSet::checkTransitionCoverage(vector<transition*> *mt){
	float coverage;
	bool covered[TRANSITIONS];
	for(int i=0; i<TRANSITIONS; ++i) covered[i]=false;

	vector<transition*>::iterator i;
	vector<testCase*>::iterator j;
	int index;
	for(i=mt->begin(),index=0; i!=mt->end(); i++,index++)
		for(j=ts.begin(); j!=ts.end(),!covered[index]; j++)
			if((*j)->searchTransition(*i))
				covered[index]=true;

	int count=0;
	for(int i=0; i<TRANSITIONS; ++i)
		if(covered[i])
			count++;
	coverage=(float)count/TRANSITIONS;
	
	return coverage;
}


//-------------------------------------------------------------------------
void parseTrace(string trace, state **s, char *outfile, testCase *tc){
	ofstream out(outfile,ios::app);
	istringstream iss(trace);
	int a=-1, b=-1, nslite=-1, ewlite=-1;
	bool change=false, newstate=false, nsreq, ewreq, end_of_trace=false;

	while(!end_of_trace){
		string checkline,line;
		if(iss.peek()!=EOF){	//Read from trace
			char aux[200];
			iss.getline(aux,200);
			line=string(aux);
			checkline=line.substr(2,6);
		}
		else{	//End of trace reached
			checkline="End of trace";
			end_of_trace=true;
		}
		if(change && (checkline.compare(" State")==0 || checkline.compare(" speci")==0 || checkline.compare("End of trace")==0)){
			newstate=true;
		}
		if(checkline.compare("NSlite")==0){
			string lite=line.substr(11,string::npos);
			nslite=getLight(lite);
			change=true;
		}
		if(checkline.compare("EWlite")==0){
			string lite=line.substr(11,string::npos);
			ewlite=getLight(lite);
			change=true;
		}
		if(checkline.compare("NSreq ")==0){
			string req=line.substr(10,string::npos);
			nsreq=getReq(req);
			change=true;
		}
		if(checkline.compare("EWreq ")==0){
			string req=line.substr(10,string::npos);
			ewreq=getReq(req);
			change=true;
		}

		if(newstate){
			state p("0", nslite, ewlite, nsreq, ewreq);
			int index=findState(&p,s);
			newstate=false;
			change=false;
			if(a==-1){a=index;}
			else {b=index;}
		}
		if(a>-1 && b>-1){
			transition *tr=new transition(s[a], s[b], "", "color=blue,penwidth=3.0");
			//t.printTransition(&out);
			tc->addTransition(tr);
			a=b;
			b=-1;
		}


	}
}