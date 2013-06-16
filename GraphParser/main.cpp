#include"graph.h"
#include"dotUtilities.h"
#include"coverage.h"

#define OUT "diagram.dot"
#define SCRIPT "script.vbs"

int main(){
	cout<<"Allocating memory"<<endl;
	state** s = setUpStates();
	vector<transition*> t;
	testSet ts;

	setUpTransitions(&t,s);
	printHeader("Traffic Lights FSM",OUT);
	printFml(s,&t,OUT);
	wrapUp(OUT);

	int diagrams=parseSpecs(s,&t,"specs",&ts);
	createScript(diagrams, string(SCRIPT));

	vector<transitionPair*> *tp=getTPairs(s,&t);

	//tests-------------
	transitionPair *test=new transitionPair(s[1],&transition(s[0],s[1],"",""),&transition(s[1],s[2],"",""));
	cout<<"\n----\n"<<ts.ts[2]->searchTPair(test)<<"\n----\n\n";
	cout<<"\n----TC_TPcoverage\n"<<ts.ts[2]->computeTPairCoverage(tp)<<"\n----\n\n";
	cout<<"\n----TS_Tcoverage\n"<<ts.computeTransitionCoverage(&t)<<"\n----\n\n";
	cout<<"\n----TS_TPcoverage\n"<<ts.computeTPairCoverage(tp)<<"\n----\n\n";
	//------------------

	cout<<"Freeing memory"<<endl;
	dropStates(s);
	system("pause");
	return 0;
}