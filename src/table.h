#include <vector>
#include <map>
#include <string>
#include <variant>


#ifndef HANDLERS_H
#define	HANDLERS_H
using namespace std;


class registry {
public:
    registry(string args, vector<string> types, vector<string> fields);
    map<string, variant<float, string, int, char>> get_row();
private:
    map<string, variant<float, string, int, char>> row; 
};

class table {
public:
    table(string name);
    table(vector<string> args);
    ~table();
    bool del();
    bool insert_one(string args); //insere novo elemento da tabela no arquivo
    bool save_to_file();
    bool show_last();
    bool show();
    string query_one(string query); //busca pela primeira ocorrência na tabela
    vector<string> query_many(string query); //busca por todas as ocorrências na tabela
    string get_name();
private:
    bool insert_field(string field);
    vector<string> fields;
    vector<string> type_fields;
    vector<registry> tree_array;
    vector<registry*> last_registries;
    string name;
    bool has_changes;
};

#endif