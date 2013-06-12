#ifndef COVERAGE_H
#define COVERAGE_H
#include"graph.h"

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
	float checkTransitionCoverage(vector<transition*> *mt);
};

class testSet{
	vector<testCase*> ts;
public:
	testSet();
	int getSize();
	void addTestCase(testCase *tc);
	float checkTransitionCoverage(vector<transition*> *mt);
};

void parseTrace(string trace, state **s, char *outfile, testCase *tc);

#endif