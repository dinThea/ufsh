#include "worker.h"
#include "handlers.h"
#include <string>
#include <vector>
#include <map> 

#ifndef REGISTRY_H
#define	REGISTRY_H
using namespace std;
typedef int (*pfunc)(vector<string> arg, table tabl);

class registry_worker : public worker{
public:

    registry_worker();
    ~registry_worker();
    virtual int run(vector<string> arg);

private:

    map<string, pfunc> *functions;
    table *tbl;

};

#endif