#include"coverage.h"
#include"dotUtilities.h"
#include<time.h>

//TESTCASE
testCase::testCase(int id, string name, string filename): id(id), name(name), filename(filename){}

testCase::testCase(int id, int size, state **s){
	this->id=id;
	name="stressTestcase";
	filename="";

	int k=0, a=0, b=1;
	while(k<size){
		transition *t=new transition(s[a],s[b],"","");
		addTransition(t);
		k++;
		if(a==STATES-1) a=0;
		else a++;
		if(b==STATES-1) b=0;
		else b++;
	}
}

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
bool testCase::searchState(state *s){
	for(vector<transition*>::iterator it=t.begin(); it!=t.end(); it++){
		state *a,*b;
		a=(*it)->getP();
		b=(*it)->getR();
		if((*a)==s) return true;
		if((*b)==s) return true;
	}
	return false;
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
	for(vector<transition*>::iterator it=t.begin(); it!=t.end(); it++){
		if((**it)==target->getA()){
			it++;
			if(it!=t.end() && (**it)==target->getB())
				return true;
			else 
				return false;
		}
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
bool testCase::contains(testCase *contained){
	//Checks whether or not testCase "contained" is included in (*this)
	if(contained->t.empty()) return false;
	if(t.size()<contained->t.size()) return false;
	if(id==contained->id) return false;

	bool start_found=false;
	vector<transition*>::iterator i, j;
	j=contained->t.begin();
	for(i=t.begin(); i!=t.end(), j!=contained->t.end(); ++i){
		if(start_found && !((**i)==(*j))){	//Sequence broken
			return false;
		}
		if(!start_found && (**i)==(*j)){	//Root found
			start_found=true;
		}
		//Any other case: Looking for root or next node in sequence
		++j;
	}
	if(start_found)	//Sequence was found in *this and not broken
		return true;
	else			//Root was not found
		return false;
}
bool testCase::operator<(testCase *contained){
	return this->contains(contained);
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
float testSet::computeStateCoverage(state **s){
	float coverage;
	bool covered[STATES];
	for(int i=0; i<STATES; ++i) covered[i]=false;

	vector<testCase*>::iterator i;
	vector<transition*>::iterator j;
	
	for(i=ts.begin();i!=ts.end();++i){
		for(int j=0; j<STATES; ++j){
			if(!covered[j] && (*i)->searchState(s[j]))
				covered[j]=true;
		}	
	}
	int count=0;
	for(int i=0; i<STATES; ++i)
		if(covered[i])
			count++;
	coverage=(float)count/STATES;
	
	return coverage;
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
testSet& testSet::prioritize(vector<transitionPair*> *tp) {
	//Reorders the testSet by TPCoverage
	comparisonOperator *comp=new comparisonOperator(tp);

	sort(ts.begin(),ts.end(),(*comp));
	return (*this);
}
testSet* testSet::priorityCull(vector<transitionPair*> *tp){
	testSet *result=new testSet(*this);
	result->prioritize(tp);

	float const initial_coverage=result->computeTPairCoverage(tp);	
	float coverage;
	bool repeat=true;

	while(repeat && !(result->ts.empty())){
		testCase *tc=result->ts.back();
		result->ts.pop_back();
		coverage=result->computeTPairCoverage(tp);
		if(coverage<initial_coverage){	//Maintain 100% coverage
			repeat=false;
			result->ts.push_back(tc);
		}
	}

	return result;
}
testSet* testSet::subgraphCull(vector<transitionPair*> *tp, state **s){
	this->prioritize(tp);	//ordering saves us a lot of useless comparisons
	testSet *result=new testSet();

	int const size=ts.size();
	float coverage=computeTPairCoverage(tp);	
	bool repeat=true;
	if(coverage<1.0 && (computeStateCoverage(s)<1.0)){
		//If we don't have state coverage we can't assume that testCases with no 
		//transitions are subgraphs trivially and must abort the operation
		return result;
	}
	bool *cull=new bool[size], *dom=new bool[size];
	for(int i=0; i<size; ++i) {
		cull[i]=false;
		dom[i]=false;
	}

	vector<testCase*>::iterator it;
	int n=size;
	while(n>0){
		n--;
		if((ts[n])->getSize()==0){		//Test cases with no transitions are subgraphs
			cull[n]=true;
		}
		int index=0;
		for(it=ts.begin(); !cull[n] && it!=ts.end(); ++it){		//Look for extended Test cases
			if((*it)->contains(ts[n]) && !dom[n]){ 
					dom[index]=true;
					cull[n]=true; 
			}
			index++;
		}
	}
	
	for(int i=0; i<size; ++i){
		if(dom[i] || !cull[i]){
			result->addTestCase(ts[i]);
		}
	}

	delete [] cull;
	delete [] dom;

	return result;
}
testSet* testSet::multiStageSubgraphCull(vector<transitionPair*> *tp, state **s){
	testSet *result=new testSet();
	int const size=ts.size();
	bool repeat=true;
	float coverage=computeTPairCoverage(tp);
	if(coverage<1.0 && (computeStateCoverage(s)<1.0)){
		//If we don't have state coverage we can't assume that testCases with no 
		//transitions are subgraphs trivially and must abort the operation
		return result;
	}

	int *dom_count=new int[size];
	bool *cull=new bool[size], *dom=new bool[size], *mask=new bool[size];
	for(int i=0; i<size; ++i)	
		mask[i]=true;

	vector<testCase*>::iterator it;
	while(repeat){
		//Initialize for the iteration
		for(int i=0; i<size; ++i) {
			if(mask[i]){
				cull[i]=false;
				dom[i]=false;
				dom_count[i]=0;
			}
		}
		//Begin subgraph cull
		for(int n=size-1; n>0; --n){
			if((ts[n])->getSize()==0){		//Test cases with no transitions are subgraphs
				cull[n]=true;
			}
			int index=0;
			for(it=ts.begin(); it!=ts.end(); ++it){		//Look for extended Test cases
				if((mask[index] && mask[n]) && (*it)->contains(ts[n]) && !dom[n]){
					if(!cull[n]){
						dom[index]=true;
						cull[n]=true; 
					}
					//In any case, add a dom count
					dom_count[index]+=1;
				}
				index++;
			}
		}
		//Check for further subgraphs
		int k=0;
		for(int i=0; i<size; ++i)
			if(mask[i] && dom_count[i]>k)
				k=dom_count[i];
		cout<<"k="<<k<<endl;

		//Check for end
		if(k<2)
			repeat=false;

		//Mask out finished cases
		if(repeat){
			for(int i=0; i<size; ++i)
				if(!dom[i])
					mask[i]=false;
		}

	}


	for(int i=0; i<size; ++i){
		if(dom[i] || !cull[i]){
			result->addTestCase(ts[i]);
		}
	}

	delete [] cull;
	delete [] dom;
	delete [] mask;
	delete [] dom_count;

	return result;
}
void testSet::printTestSet(state **s, vector<transition*> *t){
	vector<testCase*>::iterator it;
	for(it=ts.begin(); it!=ts.end(); ++it)
		(*it)->printTestCase(s,t);
}
bool* testSet::getSelectedCases(int n){
	bool *select=new bool[n];
	for(int i=0; i<n; ++i) select[i]=false;
	vector<testCase*>::iterator it;
	for(it=ts.begin(); it!=ts.end(); ++it)
		select[((*it)->getId())-1]=true;

	return select;
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
void parseTrace(string trace, state **s, testCase *tc){
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

void stressTest(int MAX_TESTS, int TEST_SIZE, vector<transitionPair*> *tp, state **s){
	cout<<"----------------------------------------\n";
	cout<<"Stress test results for "<<MAX_TESTS<<" test cases:\n";
	testCase *tc;
	double total_time=0.0, pc_time=0-0, sc_time=0.0, msc_time=0.0;    
	time_t start, finish, reference;
	time(&start);

	testSet ts=testSet();
	for(int i=0; i<MAX_TESTS; ++i){
		tc=new testCase(i,TEST_SIZE,s);
		ts.addTestCase(tc);
	}

	//PriorityCull
	time(&reference);
	testSet *pc=ts.priorityCull(tp);
	time(&finish);
	pc_time=difftime(finish,reference);
	cout<<pc->getSize()<<endl;

	//SubgraphCull
	time(&reference);
	testSet *sc=ts.subgraphCull(tp,s);
	time(&finish);
	sc_time=difftime(finish,reference);
	cout<<sc->getSize()<<endl;

	//Multi-Stage SubgraphCull
	time(&reference);
	testSet *msc=ts.subgraphCull(tp,s);
	time(&finish);
	msc_time=difftime(finish,reference);
	cout<<msc->getSize()<<endl;

	//time(&finish);
	total_time=difftime(finish,start);
	cout<<"priorityCull time: "<<pc_time<<endl;
	cout<<"subgraphCull time: "<<sc_time<<endl;
	cout<<"multisubgraphCull time: "<<msc_time<<endl;
	cout<<"Total time elapsed: "<<total_time<<endl;
	cout<<"----------------------------------------\n";

	delete pc;
	delete sc;
	delete msc;
}