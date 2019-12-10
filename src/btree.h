#ifndef BTREE
#define BTREE

#include "file.h"
#include <variant>
#include "table.h"
#include <vector>
#include <tuple>
#include <string>

#define N 4

void show_tree(chunk tree);

class chunk
{
private:
    string tree_name;
    // key of N-1 chunks
    // Child array of 'N' length
    vector<long int> refs;
    // To state whether a leaf or not; if chunk
    // is a leaf, isleaf=1 else isleaf=0
    int isleaf;
    metafile * _file;
    int isroot;
    long int root_address;
    // Keeps track of the parent chunk
    bool read_chunk();
    bool create_addr_chunk(long int parent_addr);
    bool init_chunk(long int parent_addr);
    long int chunksize;
    bool load_child_chunk(int idx);
public:

    // Counts the number of filled keys in a chunk
    int max_n;
    void show() { show_tree(*this); }
    // A utility function to insert a new key in the subtree rooted with 
    // this node. The assumption is, the node must be non-full when this 
    // function is called 
    void insertNonFull(int k); 
  
    // A utility function to split the child y of this node. i is index of y in 
    // child array C[].  The Child y must be full when this function is called 
    void splitChild(int i, long int address); 
  
    // A function to traverse all nodes in a subtree rooted with this node 
    void traverse(); 
  
    // A function to search a key in the subtree rooted with this node. 
    long int search(int k); 

    bool update();
    int n;
    vector<long int> child_addresses;
    vector<long int> chunk_values;
    long int address;
    long int parent_address;
    long int insert(long int & root_addr, long int value, long int ref);
    chunk(string tree_name);
    bool load_addr_chunk(long int addr, bool no_update=false);
    long int query_address(int k, long int, int, vector<tuple<int, int>>, long int & initial, long int & final);
    bool is_root();
    bool move_to_address(long int idx) { return this->load_addr_chunk(idx); };
    bool move_to_root();
    bool move_to_child(int index) { return this->load_child_chunk(index); };
    bool change_child_and_update(int index, int val);
    long int get_parent();
    vector<long int> get_child();
    int is_leaf();
    long int searchforleaf(int k, long int &, long int, int, bool biggest = false);
    long int searchforvalue(int k, long int &, long int, int, int&);

};

#endif