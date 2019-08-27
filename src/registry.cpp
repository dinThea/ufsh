#include "registry.h"
#include <iostream>


int IR(vector<string> args) {

    cout << "Inserindo Registro na tabela: " << args[1] << endl;
    cout << "Com os Registros: " << args[2];

    return 0;

}

int BR(vector<string> args) {
    if(args[1].compare("N"))
        cout << "Registros da tabela " << args[2] << endl; //BR N TABELA BUSCA
    else if(args[1].compare("U"))
        cout << "Busca em " << args[2] << " pelo critério " << args[3] << endl; // Exemplo: BR U TABELA BUSCA
    else
        cout << "syntax error" << endl;
    return 0;
}

int AR(vector<string> args) {
    cout << "Registros da última busca: " << endl;
    return 0;
}

int RR(vector<string> args) {
    cout << "Tabela " << args[1] << " removida!";
    return 0;
}

registry_worker::registry_worker() {
    
    this->functions = new map<string, pfunc>; 
    (*this->functions)["IR"] = IR;
    (*this->functions)["BR"] = BR;
    (*this->functions)["AR"] = AR;
    (*this->functions)["RR"] = RR;
    
}


registry_worker::~registry_worker() {
    
    free(this->functions); 
    
}

int registry_worker::run(vector<string> args) {

    if ((*this->functions).find(args[0]) != (*this->functions).end()) {
        return (*(*this->functions)[args[0]])(args);
    } else { 
        return 1;
    }
}