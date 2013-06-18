#ifndef COVERAGE_H
#define COVERAGE_H
#include"graph.h"

#define VB 1

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
	bool searchTransition(transition *target);
	float computeTransitionCoverage(vector<transition*> *mt);
	bool searchTPair(transitionPair *target);
	float computeTPairCoverage(vector<transitionPair*> *tp);
};

class testSet{
	vector<testCase*> ts;
public:
	testSet();
	int getSize() const;
	void addTestCase(testCase *tc);
	float computeTransitionCoverage(vector<transition*> *mt);
	bool searchTPair(transitionPair *target);
	float computeTPairCoverage(vector<transitionPair*> *tp);
	void prioritize(vector<transitionPair*> *tp, testSet *result) const;
	testSet* priorityCull(vector<transitionPair*> *tp) const;
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

void parseTrace(string trace, state **s, char *outfile, testCase *tc);
vector<transitionPair*>* getTPairs(state **s, vector<transition*> *mt);


#endif