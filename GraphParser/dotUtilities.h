#ifndef DOT_H
#define DOT_H
#include"graph.h"
class testCase;

void printHeader(string title,string filename);
void printHeader(string title, ofstream &out);
void printFml(state **s, vector<transition*> *t,string filename);
void printFml(state **s, vector<transition*> *t, ofstream& out);
void wrapUp(string filename);
void wrapUp(ofstream &out);
int parseSpecs(state **s, vector<transition*> *t, string infile, vector<testCase*> *testSet);
void createScript(int diagrams, string filename);

#endif