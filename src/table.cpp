#define BOOST_FILESYSTEM_VERSION 3
#define BOOST_FILESYSTEM_NO_DEPRECATED
#include <boost/filesystem.hpp>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <boost/variant.hpp>
#include <fstream>
#include <tuple>
#include <bits/stdc++.h>
#include <boost/algorithm/string.hpp>
#include "table.h"
#include "btree.h"
#include "file.h"

namespace fs = ::boost::filesystem;
using namespace std;
vector<string> split(string arg, string separator)
{

    vector<string> splitted_input;
    boost::split(splitted_input, arg, boost::is_any_of(separator));

    return splitted_input;
}

registry::registry(string arg, vector<string> types, vector<string> fields)
{

    vector<string> args = split(arg, ";");

    if (!args.empty())
    {
        for (int unsigned i = 0; i < args.size(); i++)
        {
            if (types[i].compare("INT") || types[i].compare("INT-H") || types[i].compare("INT-A"))
            {
                this->row[fields[i]] = stoi(args[i]);
            }
            else if (types[i].compare("STR"))
            {
                this->row[fields[i]] = args[i];
            }
            else if (types[i].compare("FLT"))
            {
                this->row[fields[i]] = stof(args[i]);
            }
            else if (types[i].compare("BIN"))
            {
                this->row[fields[i]] = (char)stoi(args[i]);
            }
        }
    }
}

map<string, boost::variant<float, string, int, char>> registry::get_row()
{

    return this->row;
}

bool table::fail()
{
    if (this->error == true)
    {
        return true;
    }
}

table::table(string nam, bool create_file)
{

    this->error = false;
    metafile _table("meta/tables.meta");
    if (metafile::verify_file_existence("meta/" + nam + ".meta") && !create_file)
    {
        cout << "file does not exists" << endl;
        this->error = true;
    }
    if (!this->error)
    {
        if (create_file)
        metafile _specific("meta/" + nam + ".meta");
        this->name = nam;

        this->load_fields();
    }
}

table::table(vector<string> nam)
{

    vector<string> fields;
    boost::split(fields, nam[2], boost::is_any_of(";"));

    for (vector<string>::iterator it = fields.begin(); it != fields.end(); it++)
    {
        vector<string> type_name;
        boost::split(type_name, *it, boost::is_any_of(":"));
        this->type_fields.push_back(type_name[0]);
        this->fields.push_back(type_name[1]);
    }

    metafile _table("meta/tables.meta");

    string input;
    input = nam[1] + " " + nam[2];
    _table.insert_line(input);

    metafile _specific("meta/" + nam[1] + ".meta");
    metafile _deleted_specific("meta/" + nam[1] + "_deleted" + ".meta");

    this->name = nam[1];
}

table::~table()
{
}

bool table::invalidate_line(int address, int num_bytes)
{

    metafile _specific("meta/" + this->name + "_deleted" + ".meta");

    _specific.insert_int(address);
    _specific.insert_int(num_bytes);
}

vector<tuple<int, int>> table::load_deleted()
{

    ifstream _deleted;
    vector<tuple<int, int>> results;
    if (metafile::verify_file_existence("meta/" + this->name + "_deleted.meta"))
    {   
        return results;
    }
    _deleted.open("meta/" + this->name + "_deleted.meta", ios::binary);
    char buff[sizeof(int)];
    _deleted.seekg(0, ios::beg);
    int count = 0;
    int last = 0;
    while (!_deleted.eof())
    {
        _deleted.read(buff, sizeof(int));
        if (count % 2)
        {
            if (last != bytes_to_int(string(buff)))
            {
                tuple<int, int> values(last, bytes_to_int(string(buff)));
                results.push_back(values);
            }
        }
        count++;
        last = bytes_to_int(string(buff));
    }

    _deleted.close();
    return results;
}

void table::load_fields()
{

    metafile _table("meta/tables.meta");
    string line = _table.find_first(this->name);
    cout << " LINE " << line << endl;

    vector<string> parts;

    boost::split(parts, line, boost::is_any_of("\t ")); //name and params
    vector<string> fields;
    boost::split(fields, parts[1], boost::is_any_of(";"));

    for (vector<string>::iterator it = fields.begin(); it != fields.end(); it++)
    {
        vector<string> type_name;
        boost::split(type_name, *it, boost::is_any_of(":"));
        this->type_fields.push_back(type_name[0]);
        this->fields.push_back(type_name[1]);
    }
}

string table::get_name()
{

    return this->name;
}

void get_all(const fs::path &root, const string &ext, vector<fs::path> &ret)
{
    if (!fs::exists(root) || !fs::is_directory(root))
        return;

    fs::recursive_directory_iterator it(root);
    fs::recursive_directory_iterator endit;

    while (it != endit)
    {
        if (fs::is_regular_file(*it) && it->path().extension() == ext)
            ret.push_back(it->path().filename());
        ++it;
    }
}

bool table::del()
{

    metafile _tables("meta/tables.meta");

    bool result = _tables.remove_line(this->name + " ");
    vector<fs::path> files_to_remove;
    fs::path root("meta/");
    string extension = ".meta";
    get_all(root, extension, files_to_remove);

    for (auto fl : files_to_remove)
    {
        if (!fs::exists(fl)) // does fl actually exist?
        {
            string _name = fl.string();
            if (_name.find(this->name) != string::npos)
            {
                remove(("meta/" + string(fl.string())).c_str());
            }
        }
    }

    return result;
}

bool table::insert_one(string args, string reg_name)
{
    this->name = reg_name;
    for (auto type : type_fields)
    {
        if (!type.compare("INT-H"))
        {
            cout << "não é possível inserir em tabelas com hash" << endl;
            return true;
        }
    }
    vector<tuple<int, int>> addresses = load_deleted();
    metafile _specific("meta/" + this->name + ".meta");
    _specific.insert_line(args, addresses);

    return true;
}

bool table::show_last()
{
    return true;
}

bool table::show()
{
    return true;
}

string table::query_one(string query, int &initial_address, int &final_address)
{

    metafile _specific("meta/" + this->name + ".meta");

    vector<tuple<int, int>> addresses = load_deleted();
    vector<int> initial_addresses;
    for (auto addr : addresses)
    {
        int init;
        int end;
        tie(init, end) = addr;
        initial_addresses.push_back(init);
    }
    vector<string> key_value;
    boost::split(key_value, query, boost::is_any_of(":"));
    int idx = 0;
    for (idx = 0; idx < this->fields.size(); idx++)
    {
        if (this->fields[idx] == key_value[0])
        {
            break;
        }
    }

    if (!type_fields[idx].compare("INT-A"))
    {
        cout << "is tree" << endl;
        chunk tree(this->name + "_" + key_value[0]);
        long int res = tree.query_address((long int)stoi(key_value[1]), -1, 0, addresses);
        if (res != -1)
        {
            return to_string(res);
        }
        else
        {
            return "";
        }
    }
    string line = _specific.find_first_binary(idx, key_value[1], this->type_fields, initial_address, final_address, initial_addresses);
cout << "ad" << endl;

    if (this->verify_fields(line))
    {
        return line;
    }
    else
    {
        return "";
    }
}

bool table::verify_fields(string result)
{

    bool correct = true;
    vector<string> fields;
    boost::split(fields, result, boost::is_any_of(":"));
    for (int i = 0; i < fields.size(); i++)
    {
        if (this->type_fields[i].compare("INT") || this->type_fields[i].compare("INT-H") || this->type_fields[i].compare("INT-B"))
        {
            try
            {
                stoi(fields[i]);
            }
            catch (const std::invalid_argument &ia)
            {
                return false;
            }
        }
        else if (this->type_fields[i].compare("STR"))
        {
            correct = true;
        }
        else if (this->type_fields[i].compare("FLT"))
        {
            try
            {
                stof(fields[i]);
            }
            catch (const std::invalid_argument &ia)
            {
                return false;
            }
        }
        else if (this->type_fields[i].compare("BIN"))
        {
            correct = true;
        }
    }
    return correct;
}

vector<string> table::query_many(string query, vector<int> &initial_address, vector<int> &final_address)
{

    metafile _specific("meta/" + this->name + ".meta");

    vector<tuple<int, int>> addresses = load_deleted();
    vector<int> initial_addresses;
    for (auto addr : addresses)
    {
        int init;
        int end;
        tie(init, end) = addr;
        initial_addresses.push_back(init);
    }

    vector<string> key_value;
    boost::split(key_value, query, boost::is_any_of(":"));
    int idx = 0;
    for (idx = 0; idx < this->fields.size(); idx++)
    {
        if (this->fields[idx] == key_value[0])
            break;
    }

    if (!type_fields[idx].compare("INT-A"))
    {
        cout << "is tree" << endl;
        chunk tree(this->name + "_" + key_value[0]);
        long int res = tree.query_address((long int)stoi(key_value[1]), -1, 0, addresses);
        vector<string> results;
        if (res != -1)
        {
            results.push_back(to_string(res));
        }

        return results;
    }

    vector<string> lines = _specific.find_many_binary(idx, key_value[1], this->type_fields, initial_address, final_address, initial_addresses);

    for (vector<string>::iterator it = lines.begin(); it != lines.end(); it++)
    {
        if (this->verify_fields(*it))
        {
        }
        else
        {
            lines.erase(it);
        }
    }

    return lines;
}

vector<string> table::get_fields()
{
    return this->fields;
}
vector<string> table::get_type_fields()
{
    return this->type_fields;
}

string table::generate_table_line()
{
    string line = this->get_name() + " ";
    for (int i = 0; i < this->fields.size(); i++)
    {
        line += this->type_fields[i] + ":" + this->fields[i];
        if (i != this->fields.size() - 1)
        {
            line += ";";
        }
    }

    return line;
}

bool table::add_index(string field, string type)
{
    vector<string>::iterator it;

    string old_line = this->generate_table_line();
    it = find(this->fields.begin(), this->fields.end(), field);
    int position = it - this->fields.begin();
    this->type_fields[position] = type;

    string new_line = this->generate_table_line();
    metafile _metafile("meta/tables.meta");

    _metafile.change_line(old_line, new_line);
}

bool table::rm_index(string field)
{
    vector<string>::iterator it;

    string old_line = this->generate_table_line();
    it = find(this->fields.begin(), this->fields.end(), field);
    int position = it - this->fields.begin();
    this->type_fields[position] = "INT";

    string new_line = this->generate_table_line();
    metafile _metafile("meta/tables.meta");

    _metafile.change_line(old_line, new_line);
}

// bool table::insert_field(string field) {
//     return true;
// }
