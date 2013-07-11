#include"graph.h"
#include"dotUtilities.h"
#include"coverage.h"
#define OUT "diagram.dot"
#define SCRIPT "script.vbs"

int main(){
	cout<<"Allocating memory"<<endl;
	state** s = setUpStates();
	vector<transition*> t;
	testSet ts, *prioSet=0, *extendSet=0, *multiextendSet=0;

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
	cout<<"\t4 - Generate diagrams for the minimized test set using Multi-Stage Subgraph Cull\n";
	cout<<"\t9 - Perform Stress Test\n";
	cout<<"\t0 - Exit application\n";
	cout<<"---------------------------------------------\n";

	int option=0;
	cin>>option;
	float tpc=0;
	bool *selectedTestCases=0;
	int MAX_TESTS=0;
	switch(option){
	case 1:
		ts.printTestSet(s,&t);
		cout<<"Test set size: "<<ts.getSize()<<endl;
		tpc=ts.computeTPairCoverage(tp);
		cout<<"Transition-Pair Coverage: "<<tpc*100<<"%"<<endl;
		selectedTestCases=ts.getSelectedCases(ts.getSize());
		createScript(selectedTestCases, ts.getSize(), string(SCRIPT));
		cout<<"---------------------------------------------\nJob Done.\n";
		break;
	case 2:
		prioSet=ts.priorityCull(tp);
		prioSet->printTestSet(s,&t);
		cout<<"Original Test set size: "<<ts.getSize()<<endl;
		cout<<"Final Test set size: "<<prioSet->getSize()<<endl;
		tpc=prioSet->computeTPairCoverage(tp);
		cout<<"Transition-Pair Coverage: "<<tpc*100<<"%"<<endl;
		selectedTestCases=prioSet->getSelectedCases(ts.getSize());
		createScript(selectedTestCases, ts.getSize(), string(SCRIPT));
		cout<<"---------------------------------------------\nJob Done.\n";
		break;
	case 3:
		extendSet=ts.subgraphCull(tp,s);
		extendSet->printTestSet(s,&t);
		cout<<"Original Test set size: "<<ts.getSize()<<endl;
		cout<<"Final Test set size: "<<extendSet->getSize()<<endl;
		tpc=extendSet->computeTPairCoverage(tp);
		cout<<"Transition-Pair Coverage: "<<tpc*100<<"%"<<endl;
		selectedTestCases=extendSet->getSelectedCases(ts.getSize());
		createScript(selectedTestCases, ts.getSize(), string(SCRIPT));
		cout<<"---------------------------------------------\nJob Done.\n";
		break;
	case 4:
		multiextendSet=ts.multiStageSubgraphCull(tp,s);
		multiextendSet->printTestSet(s,&t);
		cout<<"Original Test set size: "<<ts.getSize()<<endl;
		cout<<"Final Test set size: "<<multiextendSet->getSize()<<endl;
		tpc=multiextendSet->computeTPairCoverage(tp);
		cout<<"Transition-Pair Coverage: "<<tpc*100<<"%"<<endl;
		selectedTestCases=multiextendSet->getSelectedCases(ts.getSize());
		createScript(selectedTestCases, ts.getSize(), string(SCRIPT));
		cout<<"---------------------------------------------\nJob Done.\n";
		break;
	case 9:
		cout<<"Enter number of test cases: ";
		cin>>MAX_TESTS;
		stressTest(MAX_TESTS,10,tp,s);
		break;
	default:
		break;
	}
	
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