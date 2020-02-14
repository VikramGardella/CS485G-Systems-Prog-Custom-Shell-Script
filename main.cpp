#include <iostream>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fstream>

using namespace std;

class myVariable{
	string name, value;
public:
	void print();
	void changeValue(string);
	myVariable(string, string);
	string getName();
	string getValue();	
};

void myVariable::print(){
	cout << "variable name is: " << name << ", value is: " << value << endl;
}

void myVariable::changeValue(string val){
	value = val;
}


myVariable::myVariable(string nam, string val){
	name = nam;
	value = val;
}

string myVariable::getName(){return name;}

string myVariable::getValue(){return value;}

string remQuotes(string inq){
	string res;
	if(inq[0]=='"'){
		for(int j=0; j<inq.length(); j++)
			if(inq[j]!='"')
				res+=inq[j];
		return res;
	}
	else
		return inq;
}

int searchVar(string varName, vector<myVariable> *vpo){
	int index = -1;
	for(int i=0; i<vpo->size(); i++){
		if((*vpo)[i].getName()==varName){
			cout << "Variable already exists." << endl;
			index = i;
		}
	}
	return index;
}

string checkSub(string wor, vector<myVariable> *vp){
	int ind = -1;
	if(wor[0]=='$'){
		string varN = "";
		for(int i=1; i<wor.length(); i++)
			varN+=wor[i];
		ind = searchVar(varN,vp);
	}
	if(ind>=0)
		return (*vp)[ind].getValue();
	else
		return wor;
}

void setVarVal(string varName, string val, vector<myVariable> *vpoint){
	cout << "Setting " << varName << " equal to " << val << endl;
	int ind = searchVar(varName, vpoint);
	if(ind >= 0){
		(*vpoint)[ind].changeValue(val);
	}
	else{
		cout << "This variable does not exist yet, will create." << endl;
		myVariable toAdd = myVariable(varName, val);
		toAdd.print();
		vpoint->push_back(toAdd);
	}
}

void printVars(vector<myVariable> *vpoint){
	cout << "Will print out all variables created during this session." << endl;
	for(int i=0; i<vpoint->size(); i++){
		cout << (*vpoint)[i].getName() << " = " << (*vpoint)[i].getValue() << endl;
	}

}

void setNewPrompt(string* oldP, string newP){
	cout << "Changing prompt from " << *oldP << " to " << newP << endl;
	*oldP = (newP+"> ");
}

void changeDir(string newDir, vector<myVariable> *vpoint){
	char* orig;
	char buff[PATH_MAX + 1];
	orig = getcwd(buff, PATH_MAX + 1);
	cout << "Changing directory from " << orig << " to " << newDir << endl;
	int ind = searchVar(newDir, vpoint);
	const char* nd;
	if(ind>=0){
		cout << "Found a variable by that name, potentially with directory name as value." << endl;
		string v = (*vpoint)[ind].getValue();
		nd = v.c_str();
	}
	else{
		cout << "no variable with that name, assuming that's desired new dir's name." << endl;
		nd = newDir.c_str();
	}
	chdir(nd);
}

void printProcs(vector<pid_t> *prp){
	cout << "Printing a list of all processess currently running." << endl;
	cout << "All currently existing PIDs:" << endl;
	for(int i=0; i<prp->size(); i++)
		cout << (*prp)[i] << endl;
	cout << endl;
}

void doCom(vector<string> w, vector<pid_t> *procp){
	cout << endl;
	pid_t pid = fork();
	int status;
	cout << "pid is " << pid << endl;
	procp->push_back(pid);

	if(pid==-1){
		cout << "Could not fork child process." << endl;
		exit(1);
	}
	else if(pid==0){
		//executes as child process
		cout << "Attempting to execute child process and adding to list of processes." << endl;
		//turn command into char*
		char **argv = new char*[w.size()-1];
		for(int g=1; g<w.size()-1; g++){
			char *temp = &w[g][0];
			argv[g-1] = temp;
		}
		if(execvp(argv[0], argv)<0){
			cout << "Error in execution." << endl;
			exit(0);
		}

	}
	else{//wait for my child, use waitpid()
		cout << "waiting for child, my pid is: " << pid << endl;
		if(waitpid(pid, 0, 0)<0){
			perror("Failed waiting for child");
		}
	}
}


void doBackCom(vector<string> w, vector<pid_t> *procp, string* prom){
	cout << "about to start in the background" << endl;
	cout << endl;
	pid_t pid = fork();
	int status;
	cout << "pid is " << pid << endl;
	procp->push_back(pid);

	if(pid==-1){
		cout << "Could not fork child process." << endl;
		exit(1);
	}
	else if(pid==0){
		//executes as child process
		cout << "Attempting to execute child process and adding to list of processes." << endl;
		//turn command into char*
		char **argv = new char*[w.size()-1];
		for(int g=1; g<w.size()-1; g++){
			char *temp = &w[g][0];
			argv[g-1] = temp;
		}
		int suc = execvp(argv[0],argv);
		if(suc<0){
			cout << "Error in execution." << endl;
			exit(0);
		}

	}
	else{//wait for my child, use waitpid()
		cout << "not waiting for child, my pid is: " << pid << endl;
//		cout << *prom;
	}
}


void toVarCom(vector<string> w, vector<pid_t> *procp){
	cout << endl;
	string varName = w[1];

	cout << "Redirecting output to the variable: " << varName << endl;
	const char* vn = varName.c_str();
	cout << "vn is " << vn << endl;
	ofstream out(vn);
	streambuf *coutbuf = cout.rdbuf();
	cout.rdbuf(out.rdbuf());

	pid_t pid = fork();
	int status;
	cout << "pid is " << pid << endl;
	procp->push_back(pid);

	if(pid==-1){
		cout << "Could not fork child process." << endl;
		exit(1);
	}
	else if(pid==0){
		//executes as child process
		cout << "Attempting to execute child process and adding to list of processes." << endl;
		//turn command into char*
		char **argv = new char*[w.size()-1];
		for(int g=2; g<w.size()-1; g++){
			char *temp = &w[g][0];
			argv[g-2] = temp;
		}
		if(execvp(argv[0], argv)<0){
			cout << "Error in execution." << endl;
			exit(0);
		}

	}
	else{//wait for my child, use waitpid()
		cout << "waiting for child, my pid is: " << pid << endl;
		if(waitpid(pid, 0, 0)<0){
			perror("Failed waiting for child");
		}
	}
	cout.rdbuf(coutbuf); //changing back to standard output

}


string igComments(string check){
	string res = "";
	bool inComment = false;
	for(int i=0; i<check.length(); i++){
		if(check[i]=='%'){
			inComment = !inComment;
		}
		else if(inComment==false)
			res+=check[i];
	}
	return res;
}


int main(int argc, char* argv[]){
	string prompt = "nsh> ";
	string* pp = &prompt;
	string userin;
	vector<myVariable> vars;
	vector<myVariable> *vp = &vars;
	vector<pid_t> processes;
	vector<pid_t> *prop = &processes;


	cout << "Root process' PID = " << (int)getpid() << endl;

/*	pid_t mypid = fork();
	cout << "mypid is: " << mypid << endl;
	if(mypid == 0){
		cout << "I am the child and I will exit" << endl;
	}
*/
	cout << endl << "Welcome to my shell, please enter a command or 'help' for a list of possible commands or 'done' to exit." << endl << endl;
	cout << prompt;
	getline(cin,userin);
	while(userin != "done"){
		//parse here and store in vector of substrings, "words"
		vector<string> words;

		userin = igComments(userin);

		stringstream ssin(userin);
		while(ssin){
			string temp;
			ssin >> temp;
			words.push_back(temp);
		}


		//first deal with quotes if any word starts/begins with them
		//and treat it as if they're not there; will go thru all
		for(int t=0; t<words.size(); t++){
			if(words[t][0] == '"')
		//		cout << "one word at " << t << " begins with quotes." << endl;
			words[t] = remQuotes(words[t]);
			words[t] = checkSub(words[t],vp);
		}


		if(words[0] == "help"){
			cout << endl << "Here is a list of possible commands:" << endl << "(capitalized words indicate an unknown that you determine)" << endl << endl;
			cout << "'set VARIABLE VALUE' - alters said variable's value." << endl << endl;
			cout << "'prompt NEWPROMPT' - changes prompt (initially 'nsh> ')." << endl << endl;
			cout << "'dir NEWDIRECTORY' - changes the current working directory." << endl << endl;
			cout << "'procs' - will list all processes currently running." << endl << endl;
			cout << "'do CMD PARAM*' - will perform the command, CMD with all of the parameters mentioned afterwards if any, separated by a space each (PARAM*)." << endl << endl;
			cout << "'back CMD PARAM*' - does the same as the command described above, except process is run in the background instead." << endl << endl;
			cout << "'tovar VARIABLE CMD PARAM*' - similar to the do command as well, performs command, CMD, with as many parameters mentioned if any, PARAM*, but stores output in the variable, VARIABLE instead." << endl << endl;
			cout << "Also, take note that you can store strings like directory names in variables and use them at your leisure later on." << endl << endl;
		}

		else if(words[0] == "set" && words.size()>=3)
			setVarVal(words[1], words[2], vp);
		else if (words[0] == "vars")
			printVars(vp);
		else if(words[0] == "prompt" && words.size()>=2)
			setNewPrompt(pp, words[1]);
		else if(words[0] == "dir" && words.size()>=2)
			changeDir(words[1], vp);
		else if(words[0] == "procs")
			printProcs(prop);
		else if(words[0] == "do" && words.size()>=2){//need to store all parameters
			vector<string> params;
			for(int t=2; t<words.size();t++){
				params.push_back(words[t]);
			}
			doCom(words, prop);
		}
		else if(words[0] == "back" && words.size()>=2){//need to store all parameters
			vector<string> params;
			if(words.size()>2)
				doBackCom(words, prop, pp);
		}
		else if(words[0] == "tovar" && words.size()>=3){//need to store all parameters
			vector<string> params;
			if(words.size()>3)
				for(int t=4; t<words.size();t++){
					params.push_back(words[t]);
				}
			toVarCom(words, prop);
		}
		else
			cout << "Invalid command or insufficient arguments. Please try again. For a list of viable commands with parameters accepted, enter 'help'." << endl << endl;

		cout << prompt;
		getline(cin,userin);
	}
	cout << "Exiting, have a nice day." << endl << endl;
	exit(0);
}
