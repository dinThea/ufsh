#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <iterator>
#include <regex>
#include "file.h"
using namespace std;

metafile::metafile(string file_path) {
    
    this->_file = new ifstream;
    this->file_name = file_path;
    if (metafile::verify_file_existence(file_path)) {
        ofstream new_file;
        new_file.open(file_path, ios::binary);
        new_file.close();
    }
    (*this->_file).open(file_path, ios::binary);

}

metafile::~metafile() {
    if (verify_file_existence(this->file_name)) {
        (*this->_file).close();
        delete this->_file;
    }
}

bool metafile::insert_line(string line) { 
    ofstream file;
    vector<string> types;
    vector<string> splitted_input;
    file.open(file_name, ios::binary | ios_base::app);
    int n;
    float m;
    string input;

    boost::split(splitted_input, line, boost::is_any_of(";"));
    if(!file_name.compare("meta/tables.meta")){ //se arquivo a ser aberto for o de metadados insere como txt
        
        file<<line<<endl;

    } else{ //senão insere como binario
        file.seekp(0,file.end);
        int len=file.tellp();
        file.seekp(len);
        types=this->get_types(); //chama metodo que verifica os tipos da tabela
        for(int i=0;i<types.size();i++){
            if((!types[i].compare("STR")) || (!types[i].compare("BIN"))){
                input = splitted_input[i];
                file.write(input.c_str(), input.size());
            }else if(!types[i].compare("INT")){
                n=stoi(splitted_input[i]);
                file.write( (char*) &n, sizeof(n));
            }else if(!types[i].compare("FLT")){
                m=stof(splitted_input[i]);
                file.write((char*)&m, sizeof(m));
            }
            string stop = "separatoritem";
            file.write(stop.c_str(), stop.size());
        }
        string stop = "separatorline";
        file.write(stop.c_str(), stop.size());
    }

    file.close();
}

// string metafile::query(string query) {
//     regex _query(query);
//     (*this->_file).seekg(0);
//     do {
//         string reading;
//         getline((*this->_file), reading);
//         if (regex_match(reading, _query)) {
//             return reading;
//         }

//     } while (!(*this->_file).eof());

//     return "";
// }

bool metafile::remove_line(string query) { 

    ofstream _new;
    bool find = false;
    _new.open("meta/temp.meta");
    (*this->_file).seekg(0);
    string line;
    while (getline((*this->_file), line)) {
        if (line.find(query)) {
            _new << line << endl; // line.replace(line.find(query), query.length(), "NULL") << endl;
        } else {
            cout << query << endl; 
            find = true;
        }
    }
    _new.close();
    (*this->_file).close();
    remove(this->file_name.c_str());
    rename("meta/temp.meta", this->file_name.c_str());
    (*this->_file).open(this->file_name);

    return find;

}

bool metafile::verify_file_existence(string file_path) { 
    
    ifstream test;
    test.open(file_path);
    return !test.good();

}

bool metafile::show_metadata(string query) { 
    (*this->_file).seekg(0);
    string line;
    // stringstream iss;
    vector<string> splitted_input;
    bool found=false;
    while ((getline((*this->_file), line)) && (!found)) { //enquanto há linhas para serem lidas e não achou a tabela
        if (!line.find(query)) {
            found=true;
            boost::split(splitted_input, line, boost::is_any_of(":; ")); //separa os campos considerando os separadores
            cout<<"Tabela : "<<splitted_input[0]<<endl;
            cout<<"Campos : ";
            int couter = 0;
            for(int i=1;i<splitted_input.size();i++){ //enquanto tiver campos a serem mostrados
                cout<<splitted_input[i]<< (couter%2 ? ";"  : ":");
                couter++;
            }
            cout << endl;
        }
    }
    return found;
}

void metafile::show() { 
    (*this->_file).seekg(0);
    string line;
    // isstringstream iss;
    vector<string> splitted_input;
    while ((getline((*this->_file), line))) { //enquanto há linhas para serem lidas
            boost::split(splitted_input, line, boost::is_any_of(" ")); //separa a linha para pegar o nome da tabela
            cout<<"Tabela : "<<splitted_input[0]<<endl;
    }
}

string metafile::find_first(string query){ 
    (*this->_file).seekg(0);
    string line;
    bool found = false;

    while (!(this->_file->eof()) && (!found)) {
        getline((*this->_file), line);
        if (!line.find(query)) {
            found=true;
            }
        }
    
    return line;
}

string metafile::find_first_binary(int index, string value, vector<string> type){ 

    (*this->_file).seekg(0);
    string line;
    bool found = false;
    const string s((istreambuf_iterator<char>((*this->_file))), istreambuf_iterator<char>());
    
    std::regex ws_re(SEPARATOR_LINE);
    std::sregex_token_iterator end;

    for (std::sregex_token_iterator i(s.begin(), s.end(), ws_re, -1); i != end; ++i) {
        
        std::regex ws_re(SEPARATOR_ITEM);
        string line = *i;
        bool find_in_line = false;
        string result = "";

        std::sregex_token_iterator item_i(line.begin(), line.end(), ws_re, -1);

        if (index <= distance(item_i, end)) {
            int idx = 0;
            for (; item_i != end; ++item_i) {
                if ((!type[idx].compare("STR")) || (!type[idx].compare("BIN"))){
                    string input = *item_i;
                    if (!value.compare(input)) {
                        find_in_line = true;
                    }
                    result += input + " | ";
                } else if(!type[idx].compare("INT")){
                    int entry = 0;
                    int multiplier = 0;
                    string a = (*item_i);
                    for (auto c: a) {
                        entry = int(entry | (unsigned char)(c) << multiplier);
                        multiplier += 8;
                    }
                    if (!value.compare(to_string(entry))) {
                        find_in_line = true;
                    }
                    result += to_string(entry) + " | ";
                } else if(!type[idx].compare("FLT")){
                    // m=stof(splitted_input[index]);
                }
                idx++;
            }

            if (find_in_line) {
                return result;
            }
        }
    }

    return "";
}

vector<string> metafile::find_many_binary(int index, string value, vector<string> type){ 

    (*this->_file).seekg(0);
    string line;
    bool found = false;
    vector<string> results;
    const string s((istreambuf_iterator<char>((*this->_file))), istreambuf_iterator<char>());
    
    std::regex ws_re(SEPARATOR_LINE);
    std::sregex_token_iterator end;

    for (std::sregex_token_iterator i(s.begin(), s.end(), ws_re, -1); i != end; ++i) {
        
        std::regex ws_re(SEPARATOR_ITEM);
        string line = *i;
        bool find_in_line = false;

        std::sregex_token_iterator item_i(line.begin(), line.end(), ws_re, -1);

        if (index <= distance(item_i, end)) {
            int idx = 0;
            string result = "";
            for (; item_i != end; ++item_i) {
                if ((!type[idx].compare("STR")) || (!type[idx].compare("BIN"))){
                    string input = *item_i;
                    if (!value.compare(input)) {
                        find_in_line = true;
                    }
                    result += input + " | ";
                } else if(!type[idx].compare("INT")){
                    int entry = 0;
                    int multiplier = 0;
                    string a = (*item_i);
                    for (auto c: a) {
                        entry = int(entry | (unsigned char)(c) << multiplier);
                        multiplier += 8;
                    }
                    if (!value.compare(to_string(entry))) {
                        find_in_line = true;
                    }
                    result += to_string(entry) + " | ";
                } else if(!type[idx].compare("FLT")){
                    // m=stof(splitted_input[index]);
                }
                idx++;
            }

            if (find_in_line) {
                results.push_back(result);
            }
        }
    }

    return results;
}

vector<string> metafile::find_all(string query){ 
    (*this->_file).seekg(0);
    string line;
    vector<string> result;
    bool found = false;
    while (!(this->_file->eof())) {
        getline((*this->_file), line);
        if (line.find(query) <= 3) {
            found=true;
            result.push_back(line);
        }
    }
    
    return result;
}

vector<string> metafile::get_types(){
    vector<string>types; // vetor que recebe os tipos existentes na tabela
    vector<string>table;
    vector<string>splitted_input;
    fstream file;
    file.open("meta/tables.meta");
    string line;
    bool found =false; 

    boost::split(table, file_name, boost::is_any_of("/."));  //pega o nome da tabela a ser acessada
    while ((!(file.eof())) && (!found) ) {
        getline((file), line);
        if (!line.find(table[1])) {
            found=true;
            boost::split(splitted_input, line, boost::is_any_of(":; "));
        }
    }

    for(int i=0;i<splitted_input.size();i++){ //coloca os tipos no vector types
        if((!splitted_input[i].compare("STR")) || (!splitted_input[i].compare("FLT")) || (!splitted_input[i].compare("BIN")) || (!splitted_input[i].compare("INT"))){
            types.push_back(splitted_input[i]);
        }
    }

    return types;
}