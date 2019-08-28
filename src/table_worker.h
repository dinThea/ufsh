#include "worker.h"
#include "table.h"
#include <string>
#include <vector>
#include <map> 
#ifndef TABLE_H
#define	TABLE_H
using namespace std;
typedef int (*tfunc)(vector<string> arg);

class table_worker : public worker{
public:

    table_worker();
    ~table_worker();
    virtual int run(vector<string> arg);

private:

    map<string, tfunc> *functions;

};

#endif