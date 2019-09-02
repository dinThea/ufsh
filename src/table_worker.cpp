#include "table_worker.h"
#include "table.h"
#include "file.h"
#include <iostream>

int RT(vector<string> args) { //remove tabela. EX: RT CLIENTES

    cout << "Deletando tabela " << args[1] << endl;

    int res = 0;
    table _table(args[1]);
    _table.del();
    // operation

    if (!res) {
        cout << "Deletado com sucesso" << endl;
    } else {
        cout << "Falha deletando a tabela " << endl;          
    }

    return 0;

}

int CT(vector<string> args) { //cria tabela. EX: CT TABELA CAMPO_DE_DADOS

    cout << "Criando tabela: " << args[1] << endl;
    cout << "Com os campos: "<< args[2] << endl;
    table _table(args[2]);
    // result_Fields(argv[2]);

    return 0;
}

int AT(vector<string> args) { //lista metadados de uma tabela. EX: AT CLIENTES
     metafile m_table("meta/tables.meta");
     m_table.show_metadata(args[1]);   

    return 0;
}

int LT(vector<string> args) { //lista todas as tabelas existentes. EX: LT
    cout << "Lista das tabelas criadas: " << endl;
    metafile metadata_table("meta/tables.meta");
    metadata_table.show();
    
    return 0;
}

table_worker::table_worker() {
    
    this->functions = new map<string, tfunc>; 
    (*this->functions)["CT"] = (tfunc) CT;
    (*this->functions)["RT"] = (tfunc) RT;
    (*this->functions)["AT"] = (tfunc) AT;
    (*this->functions)["LT"] = (tfunc) LT;
    
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
