#ifndef COVERAGE_H
#define COVERAGE_H
#include"graph.h"

class transitionPair;

class testCase{
	int id;
	string name;
	string filename;
	vector<transition*> t;
public:
	testCase(int id, string name, string filename);
	int getId() const;
	testCase& setId(int id);
	string getFilename() const;
	testCase& setFilename(string name);
	string getName() const;
	testCase& setName(string name);
	int getSize() const;
	void addTransition(transition *tr);
	void printTestCase(state **s, vector<transition*> *t);
	bool searchState(state *s);
	bool searchTransition(transition *target);
	float computeTransitionCoverage(vector<transition*> *mt);
	bool searchTPair(transitionPair *target);
	float computeTPairCoverage(vector<transitionPair*> *tp);
	bool contains(testCase *contained);
	bool operator<(testCase *contained);	// a < b -> a contains b
};

class testSet{
	vector<testCase*> ts;
public:
	testSet();
	int getSize() const;
	void addTestCase(testCase *tc);
	float computeTransitionCoverage(vector<transition*> *mt);
	bool searchTPair(transitionPair *target);
	float computeStateCoverage(state **s);
	float computeTPairCoverage(vector<transitionPair*> *tp);
	testSet& prioritize(vector<transitionPair*> *tp);
	testSet* priorityCull(vector<transitionPair*> *tp);
	testSet* subgraphCull(vector<transitionPair*> *tp, state **s);
	void printTestSet(state **s, vector<transition*> *t);
	bool* getSelectedCases(int n);
};

class transitionPair{
	state *s;
	transition *a;
	transition *b;
public:
	transitionPair(state *s, transition *a, transition *b);
	state* getState() const;
	transitionPair& setState(state *s);
	transition* getA() const;
	transitionPair& setA(transition *a);
	transition* getB() const;
	transitionPair& setB(transition *b);
};

class comparisonOperator{	//Used for the sort operation
	vector<transitionPair*> *tp;
public:
	comparisonOperator(const vector<transitionPair*> *tp);
	bool operator()(testCase *a, testCase *b) const;
};

void parseTrace(string trace, state **s, testCase *tc);
vector<transitionPair*>* getTPairs(state **s, vector<transition*> *mt);


#endif