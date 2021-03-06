#include"dotUtilities.h"
#include"coverage.h"

void printHeader(string title,string filename){
	ofstream out(filename, ios::trunc);
	out<<"digraph {\nsubgraph cluster_0 {\n";
	out<<"label=\"";
	out<<title<<"\";\n"<<endl;
}

void printHeader(string title, ofstream &out){
	out<<"digraph {\nsubgraph cluster_0 {\n";
	out<<"label=\"";
	out<<title<<"\";\n"<<endl;
}

void printFml(state **s, vector<transition*> *t,string filename){
	ofstream out(filename, ios::app);
	vector<transition*>::iterator it;
	for(it=(*t).begin();it!=(*t).end();it++){
		out<<(**it);
	}
}

void printFml(state **s, vector<transition*> *t, ofstream& out){
	vector<transition*>::iterator it;
	for(it=(*t).begin();it!=(*t).end();it++){
		out<<(**it);
	}
}

void wrapUp(string filename){
	ofstream out(filename, ios::app);
	out<<"}}"<<endl;
}

void wrapUp(ofstream &out){
	out<<"}}"<<endl;
}

void parseSpecs(state **s, vector<transition*> *t, string infile, testSet *ts){
	/*	s - states in model
		t - transitions in model
		infile - specs filename
		testSet - set of test cases from specs file
	*/
	ifstream in(infile);
	char fileline[800];
	int k=0;
	string casename;

	while(in.getline(fileline, 255)){
		string line(fileline),aux;
		aux=line.substr(0,17);
		if(aux.compare("-- specification ")==0){
			casename=line.substr(17,string::npos);
			
		}
		else if(line.compare("Trace Type: Counterexample ")==0){
			k++;
			//cout<<"Test case "<<k<<": "<<casename<<endl;
			char fname[20];
			sprintf(fname,"CEdiagram%d.dot", k);
			string filename(fname);

			testCase *tc=new testCase(k,casename,filename);

			bool building_trace=true;
			string trace="";

			while(building_trace && in.getline(fileline, 800)){	
				//Build trace from spec file
				aux=string(fileline);
				trace.append(aux+"\n");
				//Check for end of trace
				aux=aux.substr(0,17);
				if(aux.compare("-- specification ")==0){
					casename=(string(fileline)).substr(17,string::npos);
					building_trace=false;
				}
			}
			
			parseTrace(trace,s, tc);
			ts->addTestCase(tc);
		}
	}
}

void createScript(bool *selected, int n, string filename){
	ofstream out(filename, ios::trunc);
	out<<"Set objShell = CreateObject(\"Wscript.Shell\")"<<endl;
	out<<"objShell.CurrentDirectory = \"C:\\Users\\Jonny\\Projects\\GraphParser\\GraphParser\\wd\""<<endl;
	//out<<"objShell.run \"cmd /K ";
	//out<<"DEL CEdiagram*\""<<endl;
	out<<"objShell.run \"cmd /K ";
	out<<"dot -Tbmp diagram.dot > diagram.bmp";
	if(n>0)
		out<<" &";
	for(int i=0; i<n; ++i){
		if(selected[i]){
			out<<" dot -Tbmp CEdiagram"<<i+1<<".dot > CEdiagram"<<i+1<<".bmp & ";
		}
	}
	out<<"exit\"";
}
bool clearWD(){
	char *cmd="DEL CEdiagram*";
	FILE* pipe=_popen(cmd,"r");
	if(!pipe) return false;
	return true;
}