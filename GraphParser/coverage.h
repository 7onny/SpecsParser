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
	int getId();
	testCase& setId(int id);
	string getFilename();
	testCase& setFilename(string name);
	string getName();
	testCase& setName(string name);
	int getSize();
	void addTransition(transition *tr);
	void printTestCase(state **s, vector<transition*> *t);
	bool searchTransition(transition *target);
	float computeTransitionCoverage(vector<transition*> *mt);
	bool searchTPair(transitionPair *target);
	float computeTPairCoverage(vector<transitionPair*> *tp);
};

class testSet{
	
public:
	vector<testCase*> ts;
	testSet();
	int getSize();
	void addTestCase(testCase *tc);
	float computeTransitionCoverage(vector<transition*> *mt);
	bool searchTPair(transitionPair *target);
	float computeTPairCoverage(vector<transitionPair*> *tp);
};

class transitionPair{
	state *s;
	transition *a;
	transition *b;
public:
	transitionPair(state *s, transition *a, transition *b);
	state* getState();
	transitionPair& setState(state *s);
	transition* getA();
	transitionPair& setA(transition *a);
	transition* getB();
	transitionPair& setB(transition *b);
};


void parseTrace(string trace, state **s, char *outfile, testCase *tc);
vector<transitionPair*>* getTPairs(state **s, vector<transition*> *mt);

#endif