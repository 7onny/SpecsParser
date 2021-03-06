#ifndef DOT_H
#define DOT_H
#include"graph.h"
//Forward class declarations
class testCase;
class testSet;

void printHeader(string title,string filename);
void printHeader(string title, ofstream &out);
void printFml(state **s, vector<transition*> *t,string filename);
void printFml(state **s, vector<transition*> *t, ofstream& out);
void wrapUp(string filename);
void wrapUp(ofstream &out);
void parseSpecs(state **s, vector<transition*> *t, string infile, testSet *ts);
void createScript(bool *selected, int n, string filename);
bool clearWD();


#endif