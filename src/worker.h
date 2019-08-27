#include <string>
#include <vector>
#include <map> 
#include <iostream>

#ifndef WORKER_H
#define	WORKER_H
using namespace std;

typedef int (*pfunc)(vector<string> arg);

class worker {
public:
    map<string, pfunc> *functions;
    virtual int run(vector<string> arg) { cout << "worker";  return 0; }
};

#endif