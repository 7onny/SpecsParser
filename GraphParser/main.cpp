#include"graph.h"
#include"dotUtilities.h"
#include"coverage.h"
#define OUT "diagram.dot"
#define SCRIPT "script.vbs"

int main(){
	cout<<"Allocating memory"<<endl;
	state** s = setUpStates();
	vector<transition*> t;
	testSet ts, *prioSet=0, *extendSet=0;

	if(!clearWD()){
		cerr<<"Error preping up WD\n";
		return 1;
	}
	setUpTransitions(&t,s);
	printHeader("Traffic Lights FSM",OUT);
	printFml(s,&t,OUT);
	wrapUp(OUT);

	parseSpecs(s,&t,"specs",&ts);
	vector<transitionPair*> *tp=getTPairs(s,&t);	

	cout<<"Test cases parsed.\nSelect operation to perform:\n";
	cout<<"\t1 - Generate diagrams for the original test set\n";
	cout<<"\t2 - Generate diagrams for the minimized test set using Priority Cull\n";
	cout<<"\t3 - Generate diagrams for the minimized test set using Subgraph Cull\n";
	cout<<"\t0 - Exit application\n";
	cout<<"---------------------------------------------\n";

	int option=0;
	cin>>option;
	float tpc=0;
	bool *selectedTestCases=0;
	switch(option){
	case 1:
		ts.printTestSet(s,&t);
		cout<<"Test set size: "<<ts.getSize()<<endl;
		tpc=ts.computeTPairCoverage(tp);
		cout<<"Transition-Pair Coverage: "<<tpc*100<<"%"<<endl;
		selectedTestCases=ts.getSelectedCases(ts.getSize());
		break;
	case 2:
		prioSet=ts.priorityCull(tp);
		prioSet->printTestSet(s,&t);
		cout<<"Original Test set size: "<<ts.getSize()<<endl;
		cout<<"Final Test set size: "<<prioSet->getSize()<<endl;
		tpc=prioSet->computeTPairCoverage(tp);
		cout<<"Transition-Pair Coverage: "<<tpc*100<<"%"<<endl;
		selectedTestCases=prioSet->getSelectedCases(ts.getSize());
		break;
	case 3:
		extendSet=ts.subgraphCull(tp,s);
		extendSet->printTestSet(s,&t);
		cout<<"Original Test set size: "<<ts.getSize()<<endl;
		cout<<"Final Test set size: "<<extendSet->getSize()<<endl;
		tpc=extendSet->computeTPairCoverage(tp);
		cout<<"Transition-Pair Coverage: "<<tpc*100<<"%"<<endl;
		selectedTestCases=extendSet->getSelectedCases(ts.getSize());
		break;
	default:
		break;
	}

	createScript(selectedTestCases, ts.getSize(), string(SCRIPT));
	cout<<"---------------------------------------------\nJob Done.\n";
	
	cout<<"Freeing memory.."<<endl;
	tp->clear();
	if(prioSet) delete prioSet;
	if(extendSet) delete extendSet;
	if(selectedTestCases) delete [] selectedTestCases;
	dropStates(s);

	cout<<"Exiting application.\n";
	system("pause");
	return 0;
}