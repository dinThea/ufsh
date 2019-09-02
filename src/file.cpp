#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <boost/algorithm/string.hpp>
#include "file.h"
#include <vector>
using namespace std;

metafile::metafile(string file_path) {
    
    this->_file = new ifstream;
    this->file_name = file_path;
    if (metafile::verify_file_existence(file_path)) {
        ofstream new_file;
        new_file.open(file_path);
        new_file.close();
    }
    (*this->_file).open(file_path);


}

metafile::~metafile() {
    if (verify_file_existence(this->file_name)) {
        (*this->_file).close();
        delete this->_file;
    }
}

bool metafile::insert_line(string line) { 
    ofstream file;
    file.open(file_name, ios_base::app);
    file << line << '\n';
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
    // cout << test.good();
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
            cout<<"Campos : "<<endl;
            for(int i=1;i<splitted_input.size();i++){ //enquanto tiver campos a serem mostrados
                cout<<splitted_input[i]<<endl;
            
            }
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
    bool found;
    while (!(this->_file->eof()) && (!found)) {
        getline((*this->_file), line);
        if (line.find(query)) {
            found=true;
            }
        }
    
    return line;
}

vector<string> metafile::find_all(string query){ 
    (*this->_file).seekg(0);
    string line;
    vector<string> result;
    bool found;
   while (!(this->_file->eof()) && (!found)) {
        getline((*this->_file), line);
        if (line.find(query)) {
            found=true;
            result.push_back(line);
            }
        }
    
    return result;
}