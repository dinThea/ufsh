#include <string>
#include <fstream>

using namespace std;

#ifndef FILE_T
#define FILET

class metafile {
public:
    metafile(string file_path);
    ~metafile();
    bool insert_line(string line);
    bool remove_line(string line);
private:
    fstream *_file;
    static bool verify_file_existence(string file_path);
    bool verify_line_existence(string line);
};

#endif