#pragma once
#include"graph.h"
#include"coverage.h"

void printHeader(string title,string filename);
void printFml(state **s, vector<transition*> *t,string filename);
void wrapUp(string filename);
int parseSpecs(state **s, vector<transition*> *t, string infile);
void createScript(int diagrams, string filename);