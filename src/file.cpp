#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <iterator>
#include <cstdio>
#include <regex>
#include "file.h"
#include "btree.h"
using namespace std;

long int char_to_int(string item) {
    int add = 0;
    int multiplier = 0;
    string a = item;
    for (auto c: a) {
        add = int(add | (unsigned char)(c) << multiplier);
        multiplier += 8;
    }

    return add;
}
metafile::metafile(string file_path) {
    
    this->_file = new ifstream;
    this->file_name = file_path;
    if (metafile::verify_file_existence(file_path)) {
        ofstream new_file;
        new_file.open(file_path, ios::binary);
        new_file.close();
        this->new_file = true;
    }
    (*this->_file).open(file_path, ios::binary);

}

metafile::~metafile() {
    if (!verify_file_existence(this->file_name)) {
        (*this->_file).close();
        delete this->_file;
    } 
}

bool metafile::rewrite_ints_without(int deleted, long int new_initial, vector<tuple<int,int>> available) {
    
    int i = 0, init, end;
    ofstream _deleted;
    string name = file_name;
    for (int j = 0; j < 5; j++) name.pop_back();
    remove(string(name+"_deleted.meta").c_str());

    _deleted.open(name+"_deleted.meta", ios::binary);
    _deleted.seekp(0);
    
    for (auto addrs: available) {
        tie (init, end) = addrs;
        if (deleted != i) {
            _deleted.write((char*) &init, sizeof(init));
            _deleted.write((char*) &end, sizeof(end));        
        } else if (new_initial != -1) {
            int newi = new_initial;
            _deleted.write((char*) &(newi), sizeof(newi));
            _deleted.write((char*) &end, sizeof(end));
        }

        i++;
    }

    _deleted.close();

    return true;

}

bool metafile::insert_line(string line, vector<tuple<int,int>> available) { 
    
    ofstream file;
    vector<string> types;
    vector<string> keys;
    vector<string> splitted_input;
    file.open(file_name, fstream::binary | fstream::out | fstream::in);
    int n;
    float m;
    string input;
    boost::split(splitted_input, line, boost::is_any_of(";"));
    if(!file_name.compare("meta/tables.meta")){ //se arquivo a ser aberto for o de metadados insere como txt
        
        file.close();
        file.open(file_name, fstream::app | fstream::out);
        file << line+"\n";
        file.close();
        return true;

    } else{ 
        //senão insere como binario
        file.seekp(0,file.end);
        long len = file.tellp();
        file.seekp(len);
        types=this->get_types(); //chama metodo que verifica os tipos da tabela
        keys=this->get_keys();
        long addr = file.tellp();
        string stop = "separatoritem";
        //size verification loop
        int size_bytes = 0;
        size_bytes+=sizeof(addr) + stop.size();
        for(int i=0;i<types.size();i++){
            if((!types[i].compare("STR"))){
                input = splitted_input[i];
                size_bytes+=input.size();
            } else if (!types[i].compare("BIN")) {
                
                cout << splitted_input[i] << endl;
                FILE *f = fopen(splitted_input[i].c_str(), "r+b");
                // fseek(f, 0, SEEK_SET);
                
                char c;
                input = "";
                unsigned char buffer[1];
                
                while (fread(buffer,sizeof(buffer),1,f) != NULL) {
                    input.push_back(buffer[0]);
                }

                // cout << input << endl;
                
                fclose( f );
            } else if (!types[i].compare("INT") || !types[i].compare("INT-A")) {
                n=stoi(splitted_input[i]);
                size_bytes+=sizeof(n);
            } else if (!types[i].compare("INT-H")) {
                cout << "inserção não permitida" << endl;
                file.close();
                return true;
            } else if(!types[i].compare("FLT")){
                m=stof(splitted_input[i]);
                size_bytes+=sizeof(m);
            }
            size_bytes+=stop.size();        
        }
        size_bytes+=stop.size();
        int deleted = -1, i = 0;
        long int new_initial;
        for (auto tpl: available) {
            int init, end;
            tie (init, end) = tpl;
            if (size_bytes <= (end - init + 1)) {
                file.seekp(init);
                deleted = i;
                new_initial = init+size_bytes-1 == end ? init+size_bytes : -1; 
                break;
            }
            i++;
        }

        if (deleted != -1) {
            rewrite_ints_without(deleted, new_initial, available);
        } else {
            file.seekp(len);        
        }
        addr = file.tellp();
        //writting loop

        file.write((char *) &addr, sizeof(addr));
        file.write(stop.c_str(), stop.size());
        for(int i=0;i<types.size();i++){
            if((!types[i].compare("STR")) || (!types[i].compare("BIN"))){
                input = splitted_input[i];
                file.write(input.c_str(), input.size());
            }else if(!types[i].compare("INT")){
                n=stoi(splitted_input[i]);
                file.write( (char*) &n, sizeof(n));
            }else if(!types[i].compare("INT-A")){
                string name = this->file_name;
                name.erase(0,5);
                name.erase(name.end() - 5, name.end());
                chunk tree(name + "_" + keys[i]);
                show_tree(tree);
                n=stoi(splitted_input[i]);
                file.write( (char*) &n, sizeof(n));
                long int root = 0;
                tree.insert(root, n, addr);
            }else if(!types[i].compare("INT-H")){
                n=stoi(splitted_input[i]);
                file.write( (char*) &n, sizeof(n));
            }else if(!types[i].compare("FLT")){
                m=stof(splitted_input[i]);
                file.write((char*)&m, sizeof(m));
            }
            file.write(stop.c_str(), stop.size());
        }
        stop = "separatorline";
        file.write(stop.c_str(), stop.size());
    }

    file.close();
}

bool metafile::insert_int(int a) {

    ofstream file;

    file.open(file_name, ios::binary | ios_base::app);
    file.write((char*)&a, sizeof(a));

    file.close();
}

unsigned long int metafile::read_bytes(unsigned long int addr) {
    (*this->_file).seekg(addr);
    unsigned long int res;
    (*this->_file).read(reinterpret_cast<char *>(&res), sizeof(res));
    return res;
}

unsigned long int metafile::tellpos() {
    return (*this->_file).tellg();
} 


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

bool metafile::change_line(string query, string new_line) { 

    ofstream _new;
    bool find = false;
    _new.open("meta/temp.meta");
    (*this->_file).seekg(0);
    string line;
    while (getline((*this->_file), line)) {
        if (line.find(query) == string::npos) {
            _new << line << endl; // line.replace(line.find(query), query.length(), "NULL") << endl;
        } else {
            _new << new_line << endl;
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

    bool good = !test.good();
    test.close();
    return good;

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
            // cout<<"Tabela : "<<splitted_input[0]<<endl;
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

int bytes_to_int(string bytes) {
    int entry = 0;
    int multiplier = 0;
    for (auto c: bytes) {
        entry = int(entry | (unsigned char)(c) << multiplier);
        multiplier += 8;
    }

    return entry;
}

string metafile::find_first_binary(int index, string value, vector<string> type, long int & initial_address, long int & final_address, vector<int> deleted){ 

    (*this->_file).seekg(0);
    string line;
    
    string buff_item;
    string buff_line;
    vector<string> items_line;

    char b;
    int idx = 0;
    items_line.push_back("");
    long int entry;
    long int add;

    int test_indx = 0;
    while ((*this->_file).get(b)) {

        items_line[idx].push_back(b);
        buff_item.push_back(b);
        buff_line.push_back(b);
        string sep_item(SEPARATOR_ITEM);
        string sep_line(SEPARATOR_LINE);

        if (!buff_item.compare(sep_item)) {

            items_line[idx].erase(items_line[idx].end()-sep_item.size(), items_line[idx].end());

            items_line.push_back("");
            idx++;
            buff_item.erase(buff_item.begin(), buff_item.end());
            buff_line.erase(buff_line.begin(), buff_line.end());
        } 
        
        if (!buff_line.compare(SEPARATOR_LINE)) {
            
            items_line[idx].erase(items_line[idx].end()-sep_line.size(), items_line[idx].end());
            int counter = 0;
            
            long int addr = char_to_int(items_line[0]);
            if ((!type[index].compare("STR")) || (!type[index].compare("BIN"))){
                string input = items_line[index+1];
                if (!value.compare(input)) {
                    return to_string(addr);
                }
            } else if(!type[index].compare("INT")){
                int entry = 0;
                int multiplier = 0;
                string a = (items_line[index+1]);
                for (auto c: a) {
                    entry = int(entry | (unsigned char)(c) << multiplier);
                    multiplier += 8;
                }
                if (!value.compare(to_string(entry))) {
                    return to_string(addr);
                }
            } else if(!type[index].compare("FLT")){
                // m=stof(splitted_input[index]);
            }

            idx = 0;
            items_line.clear();
            items_line.push_back("");
        }

        if (buff_item.size() == sep_item.size()) {
            buff_item.erase(0,1);
        }

        if (buff_line.size() == sizeof(SEPARATOR_LINE)/sizeof(char)) {
            buff_line.erase(0,1);            
        }

    }
    // (*this->_file).seekg(0);
    // string line;
    // bool found = false;
    // const string s((istreambuf_iterator<char>((*this->_file))), istreambuf_iterator<char>());
    
    // std::regex ws_re(SEPARATOR_LINE);
    // std::sregex_token_iterator end;

    // for (std::sregex_token_iterator i(s.begin(), s.end(), ws_re, -1); i != end; ++i) {
        
    //     std::regex ws_re(SEPARATOR_ITEM);
    //     string line = *i;
    //     bool find_in_line = false;
    //     string result = "";

    //     std::sregex_token_iterator item_i(line.begin(), line.end(), ws_re, -1);

    //     if (index <= distance(item_i, end)) {
    //         int idx = 0;
    //         string addr = *item_i;
    //         int addr_value = bytes_to_int(addr);
    //         ++item_i;
    //         result = to_string(addr_value) + " | ";
    //         for (; item_i != end; ++item_i) {
    //             if ((!type[idx].compare("STR")) || (!type[idx].compare("BIN"))){
    //                 string input = *item_i;
    //                 if (!value.compare(input)) {
    //                     find_in_line = true;
    //                 }
    //                 result += input + " | ";
    //             } else if(!type[idx].compare("INT")){
    //                 int entry = 0;
    //                 int multiplier = 0;
    //                 string a = (*item_i);
    //                 for (auto c: a) {
    //                     entry = int(entry | (unsigned char)(c) << multiplier);
    //                     multiplier += 8;
    //                 }
    //                 if (!value.compare(to_string(entry))) {
    //                     find_in_line = true;
    //                 }
    //                 result += to_string(entry) + " | ";
    //             } else if(!type[idx].compare("FLT")){
    //                 // m=stof(splitted_input[index]);
    //             }
    //             idx++;
    //         }

    //         vector<int>::iterator it = find(deleted.begin(), deleted.end(), addr_value);
    //         if (find_in_line && it == deleted.end()) {
    //             ++i;
    //             initial_address = addr_value;
    //             if (i != end) {
    //                 std::regex ws_re(SEPARATOR_ITEM);
    //                 string line = *i;
    //                 string result = "";
            
    //                 std::sregex_token_iterator item_i(line.begin(), line.end(), ws_re, -1);
            
    //                 int idx = 0;
    //                 string addr = *item_i;
    //                 end_address = bytes_to_int(addr) - 1;
    //             } else {
    //                 end_address = (int) (*this->_file).tellg() - 1;
    //             }
    //             return result;
    //         }
    //     }
    // }

    return "";
}

vector<string> metafile::find_many_binary(int index, string value, vector<string> type, vector<int> & address, vector<int> & size, vector<int> deleted){ 

    (*this->_file).seekg(0);
    string line;
    
    vector<string> results;
    string buff_item;
    string buff_line;
    vector<string> items_line;

    char b;
    int idx = 0;
    items_line.push_back("");
    long int entry;
    long int add;

    int test_indx = 0;
    while ((*this->_file).get(b)) {

        items_line[idx].push_back(b);
        buff_item.push_back(b);
        buff_line.push_back(b);
        string sep_item(SEPARATOR_ITEM);
        string sep_line(SEPARATOR_LINE);

        if (!buff_item.compare(sep_item)) {

            items_line[idx].erase(items_line[idx].end()-sep_item.size(), items_line[idx].end());

            items_line.push_back("");
            idx++;
            buff_item.erase(buff_item.begin(), buff_item.end());
            buff_line.erase(buff_line.begin(), buff_line.end());
        } 
        
        if (!buff_line.compare(SEPARATOR_LINE)) {
            
            items_line[idx].erase(items_line[idx].end()-sep_line.size(), items_line[idx].end());
            int counter = 0;
            
            long int addr = char_to_int(items_line[0]);
            if ((!type[index].compare("STR")) || (!type[index].compare("BIN"))){
                string input = items_line[index+1];
                if (!value.compare(input)) {
                    results.push_back(to_string(addr));
                }
            } else if(!type[index].compare("INT")){
                int entry = 0;
                int multiplier = 0;
                string a = (items_line[index+1]);
                for (auto c: a) {
                    entry = int(entry | (unsigned char)(c) << multiplier);
                    multiplier += 8;
                }
                if (!value.compare(to_string(entry))) {
                    results.push_back(to_string(addr));
                }
            } else if(!type[index].compare("FLT")){
                // m=stof(splitted_input[index]);
            }

            idx = 0;
            items_line.clear();
            items_line.push_back("");
        }

        if (buff_item.size() == sep_item.size()) {
            buff_item.erase(0,1);
        }

        if (buff_line.size() == sizeof(SEPARATOR_LINE)/sizeof(char)) {
            buff_line.erase(0,1);            
        }

    }
    

    // verification.close();

    // (*this->_file).seekg(0);
    // string line;
    // bool found = false;
    // vector<string> results;
    // const string s((istreambuf_iterator<char>((*this->_file))), istreambuf_iterator<char>());
    
    // std::regex ws_re(SEPARATOR_LINE);
    // std::sregex_token_iterator end;
    // bool find_in_line = false;

    // for (std::sregex_token_iterator i(s.begin(), s.end(), ws_re, -1); i != end; ++i) {
        
    //     if (find_in_line) {
    //         if (i != end) {
    //             std::regex ws_re(SEPARATOR_ITEM);
    //             string line = *i;
    //             string result = "";
        
    //             std::sregex_token_iterator item_i(line.begin(), line.end(), ws_re, -1);
        
    //             int idx = 0;
    //             string addr = *item_i;
    //             size.push_back(bytes_to_int(addr) - 1);
    //         } else {
    //             size.push_back((int) (*this->_file).tellg() - 1);
    //         }
    //     }

    //     std::regex ws_re(SEPARATOR_ITEM);
    //     string line = *i;
    //     find_in_line = false;

    //     std::sregex_token_iterator item_i(line.begin(), line.end(), ws_re, -1);

    //     if (index <= distance(item_i, end)) {
    //         int idx = 0;
    //         string result;
    //         string addr = *item_i;
    //         int addr_value = bytes_to_int(addr);
            
    //         result = to_string(addr_value) + " | ";
    //         ++item_i;
    //         for (; item_i != end; ++item_i) {
    //             if ((!type[idx].compare("STR")) || (!type[idx].compare("BIN"))){
    //                 string input = *item_i;
    //                 if (!value.compare(input)) {
    //                     find_in_line = true;
    //                 }
    //                 result += input + " | ";
    //             } else if(!type[idx].compare("INT")){
    //                 int entry = 0;
    //                 int multiplier = 0;
    //                 string a = (*item_i);
    //                 for (auto c: a) {
    //                     entry = int(entry | (unsigned char)(c) << multiplier);
    //                     multiplier += 8;
    //                 }
    //                 if (!value.compare(to_string(entry))) {
    //                     find_in_line = true;
    //                 }
    //                 result += to_string(entry) + " | ";
    //             } else if(!type[idx].compare("FLT")){
    //                 // m=stof(splitted_input[index]);
    //             }
    //             idx++;
    //         }

    //         vector<int>::iterator it = find(deleted.begin(), deleted.end(), addr_value);
    //         if (find_in_line && it == deleted.end()) {
    //             address.push_back(addr_value);
    //             results.push_back(result);
    //         }
    //     }

    //     if (find_in_line) {
    //         size.push_back((int) (*this->_file).tellg() - 1);
    //     }

    // }
 
    return results;
}

bool metafile::index_entries(int index, chunk tree, vector<int> deleted){ 

    (*this->_file).seekg(0);
    string line;
    const string s((istreambuf_iterator<char>((*this->_file))), istreambuf_iterator<char>());
    
    std::regex ws_re(SEPARATOR_LINE);
    std::sregex_token_iterator end;

    for (std::sregex_token_iterator i(s.begin(), s.end(), ws_re, -1); i != end; ++i) {
        
        std::regex ws_re(SEPARATOR_ITEM);
        string line = *i;

        std::sregex_token_iterator item_i(line.begin(), line.end(), ws_re, -1);

        if (index <= distance(item_i, end)) {
            int idx = 0;
            string addr = *item_i;
            long int addr_value = bytes_to_int(addr);
            long int entry;
            ++item_i;
            for (; item_i != end; ++item_i) {
                if (index == idx) {
                    entry = 0;
                    long int multiplier = 0;
                    string a = (*item_i);
                    for (auto c: a) {
                        entry = long(entry | (unsigned char)(c) << multiplier);
                        multiplier += 8;
                    }
                    vector<int>::iterator it = find(deleted.begin(), deleted.end(), addr_value);
                    if (it == deleted.end()) {
                        long int root_addr = 0;
                        tree.insert(root_addr, entry, addr_value);
                        tree.update();
                    }
                }
                idx++;
            }

        }

    }

    return true;
}


bool metafile::index_entries_hash(int index, string fl, vector<int> deleted){ 

    (*this->_file).seekg(0);
    string line;
    
    string buff_item;
    string buff_line;
    vector<string> items_line;

    char b;
    int idx = 0;
    items_line.push_back("");
    hash<int> hash_fn;
    char * beginner = "beginshere";
    long int entry;
    long int add;

    int test_indx = 0;

    while ((*this->_file).get(b)) {

        items_line[idx].push_back(b);
        buff_item.push_back(b);
        buff_line.push_back(b);
        string sep_item(SEPARATOR_ITEM);
        string sep_line(SEPARATOR_LINE);

        if (!buff_item.compare(sep_item)) {

            items_line[idx].erase(items_line[idx].end()-sep_item.size(), items_line[idx].end());
            
            if (index + 1 == idx){
                entry = 0;
                int multiplier = 0;
                string a = items_line[idx];
                for (auto c: a) {
                    entry = int(entry | (unsigned char)(c) << multiplier);
                    multiplier += 8;
                }
            }

            if (0 == idx){
                add = 0;
                int multiplier = 0;
                string a = items_line[idx];
                for (auto c: a) {
                    add = int(add | (unsigned char)(c) << multiplier);
                    multiplier += 8;
                }
            }

            items_line.push_back("");
            idx++;
            buff_item.erase(buff_item.begin(), buff_item.end());
            buff_line.erase(buff_line.begin(), buff_line.end());
        } 
        
        if (!buff_line.compare(SEPARATOR_LINE)) {
            
            bool found_addr = false;
            int counter = 0;

            ifstream verification(fl, ios_base::binary);
            verification.seekg(hash_fn(entry)*(strlen(beginner)+sizeof(long int)*2));
            while (!found_addr) {
                
                char buffer[strlen(beginner) + 1] = "randominfo";
                verification.read (buffer,strlen(beginner));
                buffer[strlen(beginner)] = '\0';
                string bff(buffer);
                long int __entry;
                long int __add;
                
                char _entry[sizeof(long int)] = "rand";
                char _add[sizeof(long int)] = "rand";
                verification.read((char*)&_entry, sizeof(long int));
                verification.read((char*)&_add, sizeof(long int));
                __entry = char_to_int(_entry);
                __add = char_to_int(_add);
                
                if (!bff.compare(beginner)) {
                    counter++;
                    size_t pos = hash_fn(entry+counter)*(strlen(beginner)+sizeof(long int)*2);
                    verification.seekg(pos);
                } else {
                    found_addr = true;
                }
            }
            
            verification.close();

            // cout << strlen(beginner) << " " << sizeof(long int) << endl;
            // cout << hash_fn(entry+counter)*(strlen(beginner)+sizeof(long int)*2) << endl;
            ifstream in(fl, std::ifstream::ate | std::ifstream::binary);
            long int file_size = in.tellg();
            in.close(); 

            if (hash_fn(entry+counter)*(strlen(beginner)+sizeof(long int)*2) >= file_size || file_size == -1) {

                fstream insertion(fl, fstream::binary | fstream::out | fstream::app);
                string bff(beginner);
                char a = 0;
                for (long int i = 0; i < hash_fn(entry+counter)*(strlen(beginner)+sizeof(long int)*2) - file_size; i++) insertion.write((char*)&a, sizeof(char));
                // insertion.seekp(hash_fn(entry+counter)*(strlen(beginner)+sizeof(long int)*2));
                insertion.write( bff.c_str(), bff.size());
                insertion.write( (char*) &entry, sizeof(entry));
                insertion.write( (char*) &add, sizeof(add));
                test_indx++;
                insertion.close();
            } else {

                FILE *f = fopen(fl.c_str(), "r+b");
                fseek(f, hash_fn(entry+counter)*(strlen(beginner)+sizeof(long int)*2), SEEK_SET);
                
                string bff(beginner);
                
                fputs( bff.c_str(), f);
                fwrite( (char*) &entry, sizeof(char), sizeof(entry), f);
                fwrite( (char*) &add, sizeof(char), sizeof(add), f);
                
                test_indx++;

                fclose( f );

            }

            idx = 0;
            items_line.clear();
            items_line.push_back("");
        }

        if (buff_item.size() == sep_item.size()) {
            buff_item.erase(0,1);
        }

        if (buff_line.size() == sizeof(SEPARATOR_LINE)/sizeof(char)) {
            buff_line.erase(0,1);            
        }

    }

    return true;
}

long int metafile::find_hash(int key, string fl, vector<tuple<int, int>> deleted, long int &initial, long int &final){ 
    
    ifstream verification(fl, ios_base::binary);
    hash<int> hash_fn;
    char * beginner = "beginshere";
    int counter = 0;
    verification.seekg(0);
    char buffer[strlen(beginner) + 1];
    string sep(beginner);

    bool found_addr = false;
    while (!found_addr) {

        size_t pos = hash_fn(key+counter)*(strlen(beginner)+sizeof(long int)*2);
        verification.seekg(pos);
        
        char buffer[strlen(beginner) + 1] = "randominfo";
        verification.read (buffer,sep.size());
        buffer[strlen(beginner)] = '\0';
        string bff(buffer);

        char _entry[sizeof(long int)] = "rand";
        char _add[sizeof(long int)] = "rand";
        verification.read((char*)&_entry, sizeof(long int));
        verification.read((char*)&_add, sizeof(long int));
        long int entry = char_to_int(_entry);
        long int add = char_to_int(_add);
        if (!bff.compare(beginner)) {
            if (entry == key) return add;
            counter++;
            size_t pos = hash_fn(key+counter)*(strlen(beginner)+sizeof(long int)*2);
            verification.seekg(pos);
        } else {
            return -1;
        }
    }

    verification.close();

}


// bool metafile::find_hash(int key, string fl, vector<tuple<int, int>> deleted){ 
    
//     ifstream verification(fl, ios_base::binary);
//     hash<int> hash_fn;
//     char * beginner = "beginshere";
//     int counter = 0;
//     verification.seekg(0);
//     char buffer[strlen(beginner) + 1];
//     string sep(beginner);

//     vector <int> keys  {10,20,30,40,70};

//     for (auto key: keys) {
//         verification.seekg(hash_fn(key)*(strlen(beginner)+sizeof(long int)*2));
//         bool found_addr = false;
//         while (!found_addr) {

//             char buffer[strlen(beginner) + 1] = "randominfo";
//             verification.read (buffer,sep.size());
//             buffer[strlen(beginner)] = '\0';
//             string bff(buffer);

//             char _entry[sizeof(long int)] = "rand";
//             char _add[sizeof(long int)] = "rand";
//             verification.read((char*)&_entry, sizeof(long int));
//             verification.read((char*)&_add, sizeof(long int));
//             long int entry = char_to_int(_entry);
//             long int add = char_to_int(_add);
            
//             cout << buffer << " " << entry << " " << add << endl; 
            
//             if (!bff.compare(beginner)) {
//                 counter++;
//                 size_t pos = hash_fn(entry+counter)*(strlen(beginner)+sizeof(long int)*2);
//                 verification.seekg(pos);
//             } else {
//                 cout << buffer << _entry << add << endl;
//                 found_addr = true;
//             }
//         }
//     }

//     verification.close();

// }


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
    ifstream file;
    metafile * _file = new metafile("meta/tables.meta");
    _file->show();
    delete _file;

    file.open("meta/tables.meta", ios::in);
    
    string line;
    bool found =false;
    getline((file), line);

    boost::split(table, file_name, boost::is_any_of("/."));  //pega o nome da tabela a ser acessada
    while ((!(file.eof())) && (!found) && line.compare("")) {
        if (!line.find(table[1])) {
            found=true;
            boost::split(splitted_input, line, boost::is_any_of(":; "));
        }
        getline((file), line);
    }

    for(int i=0;i<splitted_input.size();i++){ //coloca os tipos no vector types
        if((!splitted_input[i].compare("STR")) || (!splitted_input[i].compare("FLT")) || (!splitted_input[i].compare("BIN")) || (!splitted_input[i].compare("INT"))  || (!splitted_input[i].compare("INT-A"))  || (!splitted_input[i].compare("INT-H"))){
            types.push_back(splitted_input[i]);
        }
    }

    file.close();

    return types;
}

vector<string> metafile::get_keys(){
    vector<string>keys; // vetor que recebe as chaves existentes na tabela
    vector<string>table;
    vector<string>splitted_input;
    fstream file;
    file.open("meta/tables.meta", ios::in);

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

    for(int i=0;i<splitted_input.size();i++){ //coloca as chaves no vector keys
        if((splitted_input[i].compare(table[1])) && (splitted_input[i].compare("STR")) && (splitted_input[i].compare("FLT")) && (splitted_input[i].compare("BIN")) && (splitted_input[i].compare("INT")) && (splitted_input[i].compare("INT-A"))  && (splitted_input[i].compare("INT-H"))){
            keys.push_back(splitted_input[i]);
        }
    }

    file.close();

    return keys;
}