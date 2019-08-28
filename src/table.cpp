#include <vector>
#include <map>
#include <string>
#include <variant>
#include <fstream>
#include <bits/stdc++.h>
#include <boost/algorithm/string.hpp>
#include "handlers.h"

using namespace std;

vector<string> split(string arg, string separator) {
    
    vector<string> splitted_input;
    boost::split(splitted_input, arg, boost::is_any_of(separator));

    return splitted_input;

}

registry::registry(string arg, vector<string> types, vector<string> fields) {

    vector<string> args = split(arg, ";");

    if (!args.empty()) {
        for (int i = 0; i < args.size(); i++) {
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

table::table(string nam) {

    fstream file;
    file.open("meta/" + nam, ios::binary);
    if (file) {

    } else {

    }

    this->name = nam;

}

table::~table() {

}

string table::get_name() {
    return this->name;
}

bool table::close(bool save) {

}

bool table::insert_one(string args) {

}

bool table::save_to_file() {

}

bool table::show_last() {

}

bool table::show() {

}

string table::query_one(string query) {

}

vector<string> table::query_many(string query) {

}

bool table::insert_field(string field) {

}