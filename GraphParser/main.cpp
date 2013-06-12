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

	cout<<"\n----\n"<<ts.checkTransitionCoverage(&t)<<"\n----\n\n";

	cout<<"Freeing memory"<<endl;
	dropStates(s);
	system("pause");
	return 0;
}