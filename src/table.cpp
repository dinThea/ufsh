#include <vector>
#include <map>
#include <string>
#include <variant>
#include <fstream>
#include <bits/stdc++.h>
#include <boost/algorithm/string.hpp>
#include "table.h"
#include "file.h"

using namespace std;

vector<string> split(string arg, string separator) {
    
    vector<string> splitted_input;
    boost::split(splitted_input, arg, boost::is_any_of(separator));

    return splitted_input;

}

registry::registry(string arg, vector<string> types, vector<string> fields) {

    vector<string> args = split(arg, ";");
    int size = args.size();
    if (!args.empty()) {
        for (int i = 0; i < size; i++) {
            if (types[i].compare("INT")) {
                this->row[fields[i]] = stoi(args[i]);                
            } else if (types[i].compare("STR")) {
                this->row[fields[i]] = args[i];                
            } else if (types[i].compare("FLT")) {
                this->row[fields[i]] = stof(args[i]);                
            } else if (types[i].compare("BIN")) {
                this->row[fields[i]] = (char)stoi(args[i]);            
            }
        }
    }

}

map<string, variant<float, string, int, char>> registry::get_row() {

    return this->row;

}

table::table(string nam, bool create_file) {

    metafile _table("meta/tables.meta");
    if (create_file) metafile _specific("meta/"+nam+".meta");
    this->name = nam;

}

table::table(vector<string> nam) {

    metafile _table("meta/tables.meta");

    string input;
    input = nam[1] + " " + nam[2];
    _table.insert_line(input);

    metafile _specific("meta/"+nam[1]+".meta");
    
    this->name = nam[1];

}

table::~table() {

}

string table::get_name() {
    
    return this->name;

}

bool table::del() {
    
    metafile _tables("meta/tables.meta");

    bool result = _tables.remove_line(this->name+" ");
    
    remove(("meta/"+this->name+".meta").c_str());
    return result;

}

bool table::insert_one(string args) {
    metafile _specific("meta/"+this->name+".meta");
    _specific.insert_line(args);

    return true;
}

bool table::show_last() {
    return true;
}

bool table::show() {
    return true;
}

string table::query_one(string query) {
    metafile _specific("meta/"+this->name+".meta");
    
    return _specific.find_first(query);
}

// vector<string> table::query_many(string query) {
//     metafile _specific("meta/"+this->name+".meta");
    
//     return _specific.find_all(query);
// }

// bool table::insert_field(string field) {
//     return true;
// }