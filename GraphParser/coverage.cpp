#include"coverage.h"

#include"dotUtilities.h"

//TESTCASE
testCase::testCase(int id, string name, string filename): id(id), name(name), filename(filename){}
int testCase::getId() const{
	return id;
}
testCase& testCase::setId(int id){
	this->id=id;
	return *this;
}
string testCase::getFilename() const{
	return filename;
}
testCase& testCase::setFilename(string name){
	this->filename=filename;
	return *this;
}
string testCase::getName() const{
	return name;
}
testCase& testCase::setName(string name){
	this->name=name;
	return *this;
}
int testCase::getSize() const{
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
float testCase::computeTransitionCoverage(vector<transition*> *mt){
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
bool testCase::searchTPair(transitionPair *target){
	bool first_piece=false;
	for(vector<transition*>::iterator it=t.begin(); it!=t.end(); it++){
		if((**it)==target->getA())
			first_piece=true;
		if(first_piece && (**it)==target->getB())
			return true;
	}
	return false;
}
float testCase::computeTPairCoverage(vector<transitionPair*> *tp){
	float coverage;
	int n=tp->size();
	bool *covered=new bool[n];
	for(int i=0; i<n; i++) covered[i]=false;

	int index=0;
	vector<transitionPair*>::iterator i;
	for(i=tp->begin();i!=tp->end();i++,index++){
		if(searchTPair(*i))
			covered[index]=true;
	}
	int count=0;
	for(int i=0; i<n; i++)
		if(covered[i])
			count++;
	coverage=(float)count/n;

	delete [] covered;
	return coverage;
}



//TESTSET
testSet::testSet(){}
int testSet::getSize() const{
	return ts.size();
}
void testSet::addTestCase(testCase *tc){
	ts.push_back(tc);
}
float testSet::computeTransitionCoverage(vector<transition*> *mt){
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
bool testSet::searchTPair(transitionPair *target){
	vector<testCase*>::iterator it;
	for(it=ts.begin(); it!=ts.end(); it++)
		if((*it)->searchTPair(target))
			return true;
	return false;
}
float testSet::computeTPairCoverage(vector<transitionPair*> *tp){
	float coverage;
	int n=tp->size();
	bool *covered=new bool[n];
	for(int i=0; i<n; ++i) covered[i]=false;

	int index=0;
	vector<transitionPair*>::iterator i;
	vector<testCase*>::iterator j;
	for(i=tp->begin(); i!=tp->end(); i++,index++){
		if(!covered[index]){
			for(j=ts.begin(); j!=ts.end(); j++){
				if((*j)->searchTPair(*i))
					covered[index]=true;
			}
		}
	}

	int count=0;
	for(int i=0; i<n; ++i)
		if(covered[i])
			count++;
	coverage=(float)count/n;
	delete [] covered;
	return coverage;
}
void testSet::prioritize(vector<transitionPair*> *tp, testSet *result) const{
	//Reorders the testSet by TPCoverage
	comparisonOperator *comp=new comparisonOperator(tp);

	sort(result->ts.begin(),result->ts.end(),(*comp));

	//if(VB){
	//	for(vector<testCase*>::iterator it=result->ts.begin(); it!=result->ts.end(); ++it)
	//		cout<<(*it)->computeTPairCoverage(tp)<<endl;
	//	cout<<"----------------------\n";
	//}
}
testSet* testSet::priorityCull(vector<transitionPair*> *tp) const{
	testSet *result=new testSet(*this);
	prioritize(tp,result);

	float coverage=result->computeTPairCoverage(tp);	
	bool cull=true;
	if(coverage<1.0) cull=false;

	while(cull && !(result->ts.empty())){
		testCase *tc=result->ts.back();
		result->ts.pop_back();
		coverage=result->computeTPairCoverage(tp);
		if(coverage<1.0){	//Maintain 100% coverage
			cull=false;
			result->ts.push_back(tc);
		}
	}

	if(VB){
		for(vector<testCase*>::iterator it=result->ts.begin(); it!=result->ts.end(); ++it)
			cout<<(*it)->computeTPairCoverage(tp)<<endl;
		cout<<"----------------------\n";
	}

	return result;
}


//TRANSITION-PAIR
transitionPair::transitionPair(state *s, transition *a, transition *b): s(s), a(a), b(b) {}
state* transitionPair::getState() const{
	return s;
}
transitionPair& transitionPair::setState(state *s){
	this->s=s;
	return (*this);
}
transition* transitionPair::getA() const{
	return a;
}
transitionPair& transitionPair::setA(transition *a){
	this->a=a;
	return (*this);
}
transition* transitionPair::getB() const{
	return b;
}
transitionPair& transitionPair::setB(transition *b){
	this->b=b;
	return (*this);
}


//ComparisonOperator
comparisonOperator::comparisonOperator(const vector<transitionPair*> *tp){ 
	this->tp=(vector<transitionPair*>*)tp; 
}
bool comparisonOperator::operator()(testCase *a, testCase *b) const{
	float ta=a->computeTPairCoverage(tp);
	float tb=b->computeTPairCoverage(tp);
	return ta>tb;
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

vector<transitionPair*>* getTPairs(state **s, vector<transition*> *mt){
	vector<transitionPair*> *tpairs=new vector<transitionPair*>;
	vector<transition*> incoming;
	vector<transition*> outgoing;
	vector<transition*>::iterator i, j;
	for(int k=0; k<STATES; ++k){
		for(i=mt->begin(); i!=mt->end(); i++){
			if((*s[k])==(*i)->getR())
				incoming.push_back(*i);
			if((*s[k])==(*i)->getP())
				outgoing.push_back(*i);
		}
		for(i=incoming.begin(); i!=incoming.end(); i++){
			for(j=outgoing.begin(); j!=outgoing.end(); j++){
				transitionPair *newtp=new transitionPair(s[k], (*i), (*j));
				tpairs->push_back(newtp);
			}
		}
		incoming.clear();
		outgoing.clear();
	}
	return tpairs;
}

