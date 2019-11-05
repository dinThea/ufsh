#include <vector>
#include <map>
#include <boost/variant.hpp>
#include <string>
#include <tuple>

#ifndef HANDLERS_H
#define	HANDLERS_H
using namespace std;

class registry {
public:
    registry(string args, vector<string> types, vector<string> fields);
    map<string, boost::variant<float, string, int, char>> get_row();
private:
    map<string, boost::variant<float, string, int, char>> row; 
};

class table {
public:
    table(string name, bool create_file=true);
    table(vector<string> args);
    ~table();
    bool del();
    bool insert_one(string args, string reg_name); //insere novo elemento da tabela no arquivo
    bool save_to_file();
    bool show_last();
    bool show();
    string generate_table_line();
    bool add_index(string field, string type);
    bool rm_index(string field);
    bool invalidate_line(int adress, int num_bytes);
    vector<string> get_fields();
    vector<string> get_type_fields();
    string query_one(string query, int & inital_address, int & final_address); //busca pela primeira ocorrência na tabela
    vector<string> query_many(string query, vector<int> & inital_address, vector<int> & final_address); //busca por todas as ocorrências na tabela
    string get_name();
    vector<tuple<int,int>> load_deleted();
private:
    bool insert_field(string field);
    void load_fields();
    bool verify_fields(string result);
    vector<string> fields;
    vector<string> type_fields;
    vector<registry> tree_array;
    vector<registry*> last_registries;
    string name;
    bool has_changes;
};

#endif