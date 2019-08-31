#include <string>
#include <fstream>
#include <iostream>
#include "file.h"

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
    (*this->_file).close();
    delete this->_file;
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
    _new.open("meta/temp.meta");
    (*this->_file).seekg(0);
    string line;
    while (getline((*this->_file), line)) {
        if (line.find(query)) {
            _new << line << endl; // line.replace(line.find(query), query.length(), "NULL") << endl;
        }
    }
    _new.close();
    (*this->_file).close();
    remove(this->file_name.c_str());
    rename("meta/temp.meta", this->file_name.c_str());
    (*this->_file).open(this->file_name);

}

bool metafile::verify_file_existence(string file_path) {
    
    ifstream test;
    test.open(file_path);
    cout << test.good();
    return !test.good();

}