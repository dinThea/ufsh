
using namespace std;

#ifndef FILE_T
#define FILE_T

class chunk;
#include <string>
#include <fstream>
#include <variant>
#include <tuple>

#define SEPARATOR_LINE "separatorline"
#define SEPARATOR_ITEM "separatoritem"

int bytes_to_int(string buff);

class metafile {
public:
    metafile(string file_path);
    ~metafile();
    bool insert_int(int a);
    bool index_entries(int idx, chunk tree, vector<int> deleted);
    unsigned long int read_bytes(unsigned long int addr);
    unsigned long int tellpos(); 
    bool insert_line(string line, vector<tuple<int,int>> available = vector<tuple<int, int>>()); //insere uma linha nova no arquivo
    bool remove_line(string line); //remove linha do arquivo
    bool change_line(string query, string new_line);
    bool invalidate_line(string line); //marca linha como inválida e disponível para inserção
    bool show_metadata(string name); //apresenta os metadados da tabela "query"
    void show(); //lista todas as tabelas criadas
    bool isnew() { if (new_file) {new_file = false; return true;} else return false; };
    string find_first(string query); //encontra primeira ocorrência do elemento "query"
    string find_first_binary(int index, string value, vector<string> type, int & address, int & size, vector<int> deleted); //encontra primeira ocorrência do elemento "query"
    vector<string> find_many_binary(int index, string value, vector<string> type, vector<int> & address, vector<int> & size, vector<int> deleted);
    bool index_entries_hash(int index, string file, vector<int> deleted);
    vector<string> find_all(string query); //encontra todas as ocorrências do elemento "query"
    static bool verify_file_existence(string file_path); //verifica a existência do arquivo
    long int find_hash(int key, string fl, vector<tuple<int,int>> deleted);
    vector <string> get_keys(); //retorna as chaves da tabela
    vector <string> get_types(); //retorna os tipos da tabela
private:
    string file_name;
    ifstream *_file;
    bool new_file;
    bool verify_line_existence(string line);
    bool rewrite_ints_without(int deleted, long int new_initial, vector<tuple<int,int>> available);
};

#endif