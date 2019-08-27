#include "worker.h"
#include <string>
#include <vector>
#include <map> 

#ifndef TABLE_H
#define	TABLE_H
using namespace std;
typedef int (*pfunc)(vector<string> arg);

class table_worker : public worker{
public:

    table_worker();
    ~table_worker();
    virtual int run(vector<string> arg);

private:

};

#endif