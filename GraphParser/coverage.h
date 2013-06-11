#pragma once
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
};

void parseTrace(string trace, state **s, char *outfile);