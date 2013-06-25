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
bool testCase::contains(testCase *contained){
	//Checks whether or not testCase "contained" is included in (*this)
	if(contained->t.empty()) return false;
	if(t.size()>contained->t.size()) return false;
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
	//Sequence was found in *this and not broken
	return true;
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

	float coverage=result->computeTPairCoverage(tp);	
	bool repeat=true;
	if(coverage<1.0) repeat=false;

	while(repeat && !(result->ts.empty())){
		testCase *tc=result->ts.back();
		result->ts.pop_back();
		coverage=result->computeTPairCoverage(tp);
		if(coverage<1.0){	//Maintain 100% coverage
			repeat=false;
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
testSet* testSet::subgraphCull(vector<transitionPair*> *tp){
	this->prioritize(tp);	//ordering saves us a lot of useless comparisons
	testSet *result=new testSet();

	int const size=ts.size();
	float coverage=computeTPairCoverage(tp);	
	bool repeat=true;
	if(coverage<1.0) repeat=false;
	bool *cull=new bool[size];
	for(int i=0; i<size; ++i) cull[i]=false;

	vector<testCase*>::iterator it;
	int n=size;
	while(n>0){
		n--;
		if((ts[n])->getSize()==0){		//Test cases with no transitions are subgraphs
			cull[n]=true;
		}
		for(it=ts.begin(); !cull[n] && it!=ts.end(); ++it){		//Look for extended Test cases
			if(((*it)->getSize()>=ts[n]->getSize()) && (*it)->contains(ts[n])){ 
					cull[n]=true; 
			}
		}
		bool watch=cull[n];
		int dummy=1+1;
	}
	
	for(int i=0; i<size; ++i){
		if(!cull[i]){
			result->addTestCase(ts[i]);
		}
	}

	delete [] cull;

	return result;
}
void testSet::printTestCase(state **s, vector<transition*> *t){
	vector<testCase*>::iterator it;
	for(it=ts.begin(); it!=ts.end(); ++it)
		(*it)->printTestCase(s,t);
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

