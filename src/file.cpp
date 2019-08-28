#include <string>
#include <fstream>
#include "file.h"

metafile::metafile(string file_path) {
    
    this->_file = new fstream;
    if (metafile::verify_file_existence(file_path)) {

    }

}

metafile::~metafile() {
}

bool metafile::insert_line(string line) {

}

bool metafile::remove_line(string line) {

}

bool metafile::verify_file_existence(string file_path) {
    
    ifstream test;
    test.open(file_path);
    return test.good();

}