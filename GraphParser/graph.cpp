#include"graph.h"

state::state(char *index, int nslite, int ewlite, bool nsreq, bool ewreq){
	this->index=index, NSlite=nslite; EWlite=ewlite; NSreq=nsreq; EWreq=ewreq;
}
state::state(char *index, int nslite, int ewlite, bool nsreq, bool ewreq, bool ns, bool ew){
	this->index=index, NSlite=nslite; EWlite=ewlite, NSreq=nsreq; EWreq=ewreq, NS=ns, EW=ew; 
}
string state::getIndex(){
	return string(index);
}
bool state::getNS(){
	return NS;
}
bool state::getEW(){
	return EW;
}
int state::getNSlite(){
	return NSlite;
}
int state::getEWlite(){
	return EWlite;
}
bool state::getNSreq(){
	return NSreq;
}
bool state::getEWreq(){
	return EWreq;
}
string state::getTag(){
	string res;
	res.append("NSlite_");
	res.append(getLight(NSlite));
	res.append("__EWlite_");
	res.append(getLight(EWlite));
	string req;
	if(NSreq) req="_TRUE_";
	else req="_FALSE_";
	res.append(req);
	if(EWreq) req="_TRUE_";
	else req="_FALSE_";
	res.append(req);

	return res;
}
bool state::operator==(state *s){
	if(NSlite!=s->NSlite)
		return false;
	if(EWlite!=s->EWlite)
		return false;
	if(NSreq!=s->NSreq)
		return false;
	if(EWreq!=s->EWreq)
		return false;
	return true;
}


transition::transition(state *a, state *b, string label, string options){
	p=a; r=b; Label=label, this->options=options;
}
state* transition::getP(){
	return p;
}
state* transition::getR(){
	return r;
}
string transition::getLabel(){
	return Label;
}
transition& transition::setLabel(string label){
	Label=label;
	return *this;
}
void transition::insertTransition(ofstream *out){
	string line;
	//line.append(p->getTag());
	//line.append(" -> ");
	//line.append(r->getTag());
	line.append(p->getIndex());
	line.append(" -> ");
	line.append(r->getIndex());
	line.append("[label=\"");
	line.append(Label);
	line.append("\"");
	if(options.compare("")!=0){
		line.append(",");
		line.append(options);
	}
	line.append("];");
	(*out)<<line<<endl;
}

//Utilities
state** setUpStates(){
	state** s = new state*[STATES];
	s[0]=new state("0",green,red,0,0);
	s[1]=new state("1",yellow,red,0,1);
	s[2]=new state("2",red, green,0,0);
	s[3]=new state("3",red, green,1,0);
	s[4]=new state("4",red,yellow,1,0);
	s[5]=new state("5",green, red,0,1);
	cout<<"States set up"<<endl;
	return s;
}
void dropStates(state **s){
	for(int i=0; i<STATES; ++i)
		delete s[i];
	delete [] s;
	cout<<"States dropped"<<endl;
}

void setUpTransitions(vector<transition*> *trans, state **s){
	transition *t;
	t=new transition(s[0],s[1],"EW","");
	(*trans).push_back(t);
	t=new transition(s[1],s[2],"/NS","");
	(*trans).push_back(t);
	t=new transition(s[1],s[3],"NS","");
	(*trans).push_back(t);
	t=new transition(s[2],s[4],"NS","");
	(*trans).push_back(t);
	t=new transition(s[3],s[4],"","");
	(*trans).push_back(t);
	t=new transition(s[4],s[5],"EW","");
	(*trans).push_back(t);
	t=new transition(s[4],s[0],"/EW","");
	(*trans).push_back(t);
	t=new transition(s[5],s[1],"","");
	(*trans).push_back(t);
}

void dropTransitions(vector<transition> t){
	t.clear();
	cout<<"Transitions dropped"<<endl;
}

string getLight(int lite){
	string res;
	switch(lite){
	case 0:
		res="green";
		break;
	case 1:
		res="yellow";
		break;
	case 2:
		res="red";
		break;
	default:
		res="";
		break;
	}
	return res;
}

int getLight(string lite){
	if(lite.compare("green")==0) return 0;
	else if(lite.compare("yellow")==0) return 1;
	else if(lite.compare("red")==0) return 2;
	else return -1;
}

bool getReq(string req){
	if(req.compare("TRUE")==0)
		return true;
	else 
		return false;
}

int findState(state target, state **s){
	for(int i=0; i<STATES; ++i)
		if(target==s[i]){
			return i;
		}
	return -1;
}

void parseTrace(string trace, state **s, char *outfile){
	ofstream out(outfile,ios::app);
	istringstream iss(trace);
	int a=-1, b=-1, nslite=-1, ewlite=-1;
	bool change=false, newstate=false, nsreq, ewreq, end_of_trace=false;

	while(!end_of_trace){
		string checkline,line;
		if(iss.peek()!=EOF){	//Read from trace
			char aux[200];
			iss.getline(aux,200);
			line=string(aux);
			checkline=line.substr(2,6);
		}
		else{	//End of trace reached
			checkline="End of trace";
			end_of_trace=true;
		}
		if(change && (checkline.compare(" State")==0 || checkline.compare(" speci")==0 || checkline.compare("End of trace")==0)){
			newstate=true;
		}
		if(checkline.compare("NSlite")==0){
			string lite=line.substr(11,string::npos);
			nslite=getLight(lite);
			change=true;
		}
		if(checkline.compare("EWlite")==0){
			string lite=line.substr(11,string::npos);
			ewlite=getLight(lite);
			change=true;
		}
		if(checkline.compare("NSreq ")==0){
			string req=line.substr(10,string::npos);
			nsreq=getReq(req);
			change=true;
		}
		if(checkline.compare("EWreq ")==0){
			string req=line.substr(10,string::npos);
			ewreq=getReq(req);
			change=true;
		}

		if(newstate){
			state p("0", nslite, ewlite, nsreq, ewreq);
			int index=findState(p,s);
			newstate=false;
			change=false;
			if(a==-1){a=index;}
			else {b=index;}
		}
		if(a>-1 && b>-1){
			transition t(s[a], s[b], "", "color=blue,penwidth=3.0");
			t.insertTransition(&out);
			a=b;
			b=-1;
		}


	}
}