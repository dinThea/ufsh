#include "registry_worker.h"
#include "table.h"
#include <iostream>

int IR(vector<string> args, table tbl) { //insere registro na tabela. EX: IR TABELA CAMPO_DE_DADOS

    cout << "Inserindo Registro na tabela: " << args[1] << endl;
    cout << "Com os Registros: " << args[2];
    
    //  table tbl(args[1]);
    //  tbl.insert_one(args[2]);
    // tbl.close();

    return 0;

}

int BR(vector<string> args, table tbl) {//realiza busca na tabela

    if(args[1].compare("N")) { //busca por todas as ocorrências na tabela
        cout << "Registros da tabela " << args[0] << endl; //BR N TABELA BUSCA
        // vector<string> result;
        // result = tbl.query_many(args[2]);
        // if (result.empty()) {
        //     cout << "Nenhum registro encontrado" << endl;
        // } else {
        //     cout << "Registros encontrados:" << endl;
        //     for (vector<string>::iterator it = result.begin(); it != result.end(); it++) {
        //         cout << *it << endl;
        //     }
        // }
    }
    else if(args[1].compare("U")) { //busca pela primeira ocorrência na tabela
        cout << "Busca em " << args[2] << " pelo critério " << args[3] << endl; // Exemplo: BR U TABELA BUSCA
        // string result;
        // result = tbl.query_one(args[2]);
        // if (result.compare("")) {
        //     cout << "Nenhum registro encontrado" << endl;
        // } else {
        //     cout << "Registro encontrado:" << endl;
        //     cout << result << endl;
        // }
    }
    else
        cout << "syntax error" << endl;
    return 0;
}

int AR(vector<string> args, table tbl) {

    cout << "Registros da última busca: " << endl;    
    // tbl.show_last();

    return 0;
}

int RR(vector<string> args, table tbl) { //remove registro da tabela
    cout << "Registro " << args[1] << " removida!";
    return 0;
}

registry_worker::registry_worker() {
    
    this->functions = new map<string, pfunc>; 
    (*this->functions)["IR"] = (pfunc) IR;
    (*this->functions)["BR"] = (pfunc) BR;
    (*this->functions)["AR"] = (pfunc) AR;
    (*this->functions)["RR"] = (pfunc) RR;
    
}


registry_worker::~registry_worker() {
    
    free(this->functions); 
    
}

int registry_worker::run(vector<string> args) {

    if (this->tbl && (*this->tbl).get_name().compare(args[1])) { 
    
    } else {
        this->tbl = new table(args[1]);
    }
    
    if ((*this->functions).find(args[0]) != (*this->functions).end()) {
        return (*(*this->functions)[args[0]])(args, *this->tbl);
    } else { 
        return 1;
    }   
    
}