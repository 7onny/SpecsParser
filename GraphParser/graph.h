#ifndef GRAPH_H
#define GRAPH_H

#include<string>
#include<vector>
#include<iostream>
#include<sstream>
#include<fstream>
#include<cstdlib>
#include<algorithm>

using namespace std;

//Total States and transitions in model
#define STATES 6
#define TRANSITIONS 8

enum {green, yellow , red};

class state{
	char *index;
	bool NS;
	bool EW;
	int NSlite;
	int EWlite;
	bool NSreq;
	bool EWreq;
public:
	state(char *index, int nslite, int ewlite, bool nsreq, bool ewreq);
	state(char *index, int nslite, int ewlite, bool nsreq, bool ewreq, bool ns, bool ew);
	string getIndex();
	bool getNS();
	bool getEW();
	int getNSlite();
	int getEWlite();
	bool getNSreq();
	bool getEWreq();
	string getTag();
	bool operator==(state *s);
};

class transition{	//p -> r
	state *p;
	state *r;
	string Label;
	string options;
public:
	transition(state *a, state *b, string label, string options);
	state* getP();
	state* getR();
	string getLabel();
	transition& setLabel(string label);
	bool operator==(transition *t);
	friend ofstream& operator<<(ofstream &out, transition& t);
};

//Utilities
state** setUpStates();
void dropStates(state **s);
void setUpTransitions(vector<transition*> *trans, state **s);
void dropTransitions(vector<transition> t);
string getLight(int lite);
int getLight(string lite);
bool getReq(string req);
int findState(state *target, state **s);

#endif