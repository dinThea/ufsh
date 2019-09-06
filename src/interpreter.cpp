#include <sstream>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <boost/algorithm/string/join.hpp>

#include "registry_worker.h"
#include "table_worker.h"
#include "interpreter.h"
using namespace std;

interpreter::interpreter(string in) {

    this->_file = new ifstream;
    this->workers = new list<worker*>;
    this->_input = in;

    (*workers).push_back(new table_worker);
    (*workers).push_back(new registry_worker );

    if (in.compare("cin")) {
        (*this->_file).open(in);
    }
    
}

interpreter::~interpreter() {
    (*this->_file).close();
    delete workers;
}

vector<string> interpreter::read() { //lê comando do terminal
    string input;
    vector<string> splitted_input;
    cout << "[SHELL>$] ";
    do {
        if (!this->_input.compare("cin")){
            getline(cin, input);
        } else {
            getline((*this->_file), input);
        }
    } while (!input.compare(""));

    istringstream iss(input);

    if (input.compare("") != 0) {

        int found_separator = 0;
        for (string s; iss >> input;) {
            
            if (!found_separator) {
                splitted_input.push_back(input);
            } else {
                vector<string> temp; 
                temp.push_back(splitted_input.back());
                temp.push_back(input);
                splitted_input.pop_back();
                splitted_input.push_back(boost::algorithm::join(temp, " "));
            }

            size_t found = input.find(";");
            size_t found2 = input.find(":");

            if (found!=string::npos|| found2!=string::npos) {
                found_separator = 1;
            }
        }



        transform(splitted_input[0].begin(), splitted_input[0].end(), splitted_input[0].begin(),::toupper);

        // cout << splitted_input[0] << " " << endl;

        if ((!splitted_input[0].compare("CI")) || (!splitted_input[0].compare("BR"))) {

            transform(splitted_input[1].begin(), splitted_input[1].end(), splitted_input[1].begin(),::toupper);
            transform(splitted_input[2].begin(), splitted_input[2].end(), splitted_input[2].begin(),::toupper);
        }
    }
    
    return splitted_input;

}

void interpreter::exec() { //chama os métodos correspondentes
 
    vector<string> input;
    do {
        input = read();
        // cout << input[0] << endl;
    
    } while (run_all(input));

}

bool interpreter::run_all(vector<string> args) {

    if (args[0].compare("EB")) {
        for (list<worker*>::iterator it=(workers->begin()); it != (workers->end()); it++) {
            
            int ret = 2;

            worker *wk = *it; 
            table_worker *tw = dynamic_cast<table_worker*>(wk);
            registry_worker *rw = dynamic_cast<registry_worker*>(wk);
            if (tw) ret = tw->run(args);
            else if (rw) ret = rw->run(args);
            if (ret == 0) return true;
            else if (ret == 1) return false;
        }
        return false;
    } else {
        return false;
    }
}