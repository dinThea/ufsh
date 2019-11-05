#include "index_worker.h"
#include "btree.h"
#include <iostream>
#include <tuple>

index_worker::index_worker() {
    
    (this->num_fields)["CI"] = 4;
    (this->num_fields)["RI"] = 3;
    (this->num_fields)["GI"] = 3;

}

index_worker::~index_worker() {

}

int index_worker::run(vector<string> args) {

    if (args.size() == this->num_fields[args[0]]) {
        
        if (!args[0].compare("CI")) {
            return this->CI(args);
        }
        else if (!args[0].compare("RI")) {
            return this->RI(args);
        }
        else if (!args[0].compare("GI")) {
            return this->GI(args);
        } else { 
            return 2;
        }   
    } else {
        cout << "Número incorreto de argumentos" << endl;
        return 1;
    }
}

int index_worker::get_num_fields(string func) {
    map<string,int>::iterator it = (this->num_fields).find(func);
    if(it != (this->num_fields).end()) {
        return (this->num_fields[func]);
    } else {
        return 0;
    }
}

int index_worker::CI(vector<string> arg) {
    cout << "Criando um índice para a tabela " << arg[2] << endl;
    table * _table = new table(arg[2]);
    vector<string> fields = _table->get_fields();
    vector<string> type_fields = _table->get_type_fields();
    vector <string>::iterator it;
    it = find(fields.begin(), fields.end(), arg[3]);

    if (it == fields.end()) {
        cout << "Erro: não existe campo com este nome" << endl;
        return 1;
    } else {
        int idx = it - fields.begin();
        if (!type_fields[idx].compare("INT")) {
            if (!arg[1].compare("A")) {
                if (metafile::verify_file_existence("meta/" + arg[2] + ".meta")) {
                    cout << "Erro: tabela não existente" << endl;
                    return 1;
                }
                else{
                    _table->add_index(arg[3], "INT-A");
                    chunk tree(arg[2] + "_" + arg[3]);

                    vector<tuple<int, int>> addresses = _table->load_deleted();
                    vector<int> initial_addresses;
                    for (auto addr: addresses) {
                        int init; int end;
                        tie (init, end) = addr; 
                        initial_addresses.push_back(init);
                    }
                    
                    metafile _file("meta/"+arg[2]+".meta");

                    _file.index_entries(idx, tree, initial_addresses);
                    return 0;
                }
            } else if (!arg[1].compare("H")) {
                cout << "Hashing " << endl;
            }
        } else if (!type_fields[idx].compare("INT-H") || !type_fields[idx].compare("INT-A")) {
            cout << "Este campo já é índice" << endl;
            return 0;
        } else {
            cout << "Tipo errado, o indice deve ser do tipo INT" << endl;
            return 1;
        }
    }
    
    
    return 0;
}

int index_worker::RI(vector<string> arg) {
    cout << "Removendo o indice da tabela " << arg[1] << " cuja chave e " << arg[2] << endl;
    return 0;
}

int index_worker::GI(vector<string> arg) {
    cout << "Gerando novamente o indice da tabela " << arg[1] << " cuja chave e " << arg[2] << endl;
    return 0;
}
