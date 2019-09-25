#include <string>
#include <fstream>

using namespace std;

#ifndef FILE_T
#define FILET

class metafile {
public:
    metafile(string file_path);
    ~metafile();
    bool insert_line(string line); //insere uma linha nova no arquivo
    bool remove_line(string line); //remove linha do arquivo
    bool invalidate_line(string line); //marca linha como inválida e disponível para inserção
    bool show_metadata(string name); //apresenta os metadados da tabela "query"
    void show(); //lista todas as tabelas criadas
    string find_first(string query); //encontra primeira ocorrência do elemento "query"
    vector<string> find_all(string query); //encontra todas as ocorrências do elemento "query"
    static bool verify_file_existence(string file_path); //verifica a existência do arquivo
    vector <string> get_types(); //retorna os tipos da tabela
private:
    string file_name;
    ifstream *_file;
    bool verify_line_existence(string line);
};

#endif