#include "table.h"
#include <iostream>

int RT(vector<string> args) {

    cout << "Deletando tabela" << args[1] << endl;

    int res = 0;

    // operation

    if (!res) {
        cout << "Deletado com sucesso" << endl;
    } else {
        cout << "Falha deletando a tabela " << endl;          
    }

    return 0;

}

int CT(vector<string> args) {

    cout << "Criando tabela: " << args[1] << endl;
    cout << "Com os campos: " << endl;
   
    // result_Fields(argv[2]);

    return 0;
}

int AT(vector<string> args) {


    return 0;
}

int LT(vector<string> args) {

    cout << "Lista das tabelas criadas: " << endl;
    return 0;
}

table_worker::table_worker() {
    
    this->functions = new map<string, pfunc>; 
    (*this->functions)["CT"] = CT;
    (*this->functions)["RT"] = RT;
    (*this->functions)["AT"] = AT;
    (*this->functions)["LT"] = LT;
    
}

table_worker::~table_worker() {
    
    delete this->functions; 
    
}

int table_worker::run(vector<string> args) {

    if ((*this->functions).find(args[0]) != (*this->functions).end()) {
        return (*(*this->functions)[args[0]])(args);
    } else { 
        return 1;
    }
}
