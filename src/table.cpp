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

    if (!args.empty()) {
        for (int unsigned i = 0; i < args.size(); i++) {
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

    this->load_fields();

}

table::table(vector<string> nam) {

    vector<string> fields;
    boost::split(fields, nam[2], boost::is_any_of(";"));

    for (vector<string>::iterator it = fields.begin(); it != fields.end(); it ++) {
        vector<string> type_name;
        boost::split(type_name, *it, boost::is_any_of(":"));
        this->type_fields.push_back(type_name[0]);
        this->fields.push_back(type_name[1]);
    }

    metafile _table("meta/tables.meta");

    string input;
    input = nam[1] + " " + nam[2];
    _table.insert_line(input);

    metafile _specific("meta/"+nam[1]+".meta");
    
    this->name = nam[1];

}

table::~table() {

}

void table::load_fields() {
    
    metafile _table("meta/tables.meta");
    string line = _table.find_first(this->name);

    vector<string> parts;

    boost::split(parts, line, boost::is_any_of("\t ")); //name and params
    vector<string> fields;
    boost::split(fields, parts[1], boost::is_any_of(";"));

    for (vector<string>::iterator it = fields.begin(); it != fields.end(); it ++) {
        vector<string> type_name;
        boost::split(type_name, *it, boost::is_any_of(":"));
        this->type_fields.push_back(type_name[0]);
        this->fields.push_back(type_name[1]);
    }

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

bool table::insert_one(string args, string reg_name) {
    this->name = reg_name;
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
    
    vector<string> key_value;
    boost::split(key_value, query, boost::is_any_of(":"));
    int idx = 0;
    for (idx = 0; idx < this->fields.size(); idx++) {
        if (this->fields[idx] == key_value[0]) break;
    }
    string line =  _specific.find_first_binary(idx, key_value[1], this->type_fields);
    
    if (this->verify_fields(line)) {
        return line;
    } else {
        return "";
    }
}

bool table::verify_fields(string result) {

    bool correct = true;
    vector<string> fields;
    boost::split(fields, result, boost::is_any_of(":"));
    for (int i = 0; i < fields.size(); i++) {
        if (this->type_fields[i].compare("INT")) {
            try {
                stoi(fields[i]);
            }
            catch (const std::invalid_argument& ia) {
                return false;
            }
        } else if (this->type_fields[i].compare("STR")) {
            correct = true;
        } else if (this->type_fields[i].compare("FLT")) {
            try {
                stof(fields[i]);
            }
            catch (const std::invalid_argument& ia) {
                return false;
            }
        } else if (this->type_fields[i].compare("BIN")) {
            correct=true;
            }
    }
    return correct;
} 


vector<string> table::query_many(string query) {

    metafile _specific("meta/"+this->name+".meta");
    
    vector<string> key_value;
    boost::split(key_value, query, boost::is_any_of(":"));
    int idx = 0;
    for (idx = 0; idx < this->fields.size(); idx++) {
        if (this->fields[idx] == key_value[0]) break;
    }
    vector<string> lines =  _specific.find_many_binary(idx, key_value[1], this->type_fields);
        
    for (vector<string>::iterator it = lines.begin(); it != lines.end(); it++) {
        if (this->verify_fields(*it)) {

        } 
        else{
            lines.erase(it);
        }
    }

    return lines;

}

// bool table::insert_field(string field) {
//     return true;
// }

