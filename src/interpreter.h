#include "worker.h"
#include <string>
#include <vector>
#include <list>

#ifndef INTERPRETER_H
#define	INTERPRETER_H
using namespace std;

class interpreter {
public:
    interpreter(string in);
    virtual ~interpreter();
    vector<string> read();
    list<worker*> *workers;
    void exec();
    bool run_all(vector<string> comando);

private:
    ifstream *_file;
    string _input;
};

#endif	/* interpreter_H */

