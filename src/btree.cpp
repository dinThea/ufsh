// CPP program to implement B* tree
#include <bits/stdc++.h>
#include <vector>
#include <string>
#include <iostream>
#include "file.h"
#include "btree.h"
#include <sys/statvfs.h>
#include <cstdio>
#include <fstream>
using namespace std;

// This can be changed to any value -
// it is the order of the B* Tree

bool chunk::change_child_and_update(int index, int val){
    this->chunk_values[index] = val;
    this->update();
}


bool chunk::load_addr_chunk(long int addr, bool no_update) {
    
    this->isleaf = 1;
    ifstream file;
    if (!no_update) this->update();
    string filename = "meta/" + this->tree_name + ".meta"; 
    file.open(filename, fstream::binary | fstream::in);
    file.seekg(addr);
    file.read((char *) &(this->address), sizeof(long int));
    file.read((char *) &(this->parent_address), sizeof(long int));
    
    child_addresses.clear();
    chunk_values.clear();
    refs.clear();
    
    int i;
    long int buf;
    this->n = 0;
    for (i = 0; i < this->max_n; i++) {
        file.read((char *) &(buf), sizeof(long int));
        child_addresses.push_back(buf);
        file.read((char *) &(buf), sizeof(long int));
        chunk_values.push_back(buf);
        file.read((char *) &(buf), sizeof(long int));
        refs.push_back(buf);
        // cout << "refs " << this->refs[i] << endl;
        // cout << "child_addresses " << this->child_addresses[i] << endl;
        // cout << "chunk_values " << this->chunk_values[i] << endl;

        if (this->refs[i] != -1) {
            this->n++;
        }

        if (this->child_addresses[i] != -1) {
            this->isleaf = 0;
        }
    } 
    file.read((char *) &buf, sizeof(buf));
    child_addresses.push_back(buf);
    
    file.close();

    return true;
}

bool chunk::create_addr_chunk(long int parent_addr) {
    
    ofstream file;
        
    file.open("meta/" + tree_name + ".meta", fstream::binary | fstream::out | fstream::in);
    file.seekp(0,file.end);
    long len = file.tellp();
    file.seekp(len);
    
    long int addr = file.tellp();
    file.write((char *) &(addr), sizeof(addr));
    this->parent_address = parent_addr;

    this->address = addr;
    file.write((char *) &(parent_addr), sizeof(parent_addr));

    child_addresses.clear();
    chunk_values.clear();
    refs.clear();
    long int a = -1;
    this->n = 0;
    for (int i = 0; i < this->max_n; i++) {
        file.write((char *) &a, sizeof(a));
        child_addresses.push_back(-1);
        file.write((char *) &a, sizeof(a));
        chunk_values.push_back(-1);
        file.write((char *) &a, sizeof(a));
        refs.push_back(-1);
    } 
    file.write((char *) &a, sizeof(a));
    child_addresses.push_back(-1);
    
    file.close();
    this->isleaf = 1;
    return true;
}

void show_tree_r(chunk tree) {

    vector <long int> trees_to_visit;
    int i;
    for (i = 0; i < tree.n; i++) {
        cout << tree.chunk_values[i] << " | ";
        if (tree.child_addresses[i] != -1) trees_to_visit.push_back(tree.child_addresses[i]);
    }

    if (tree.child_addresses[i] != -1) trees_to_visit.push_back(tree.child_addresses[i]);

    string a;
    cin >> a;
    cout << endl;

    for (auto tr: trees_to_visit) {
        tree.move_to_address(tr);
        show_tree_r(tree);
    }
}

void show_tree(chunk tree) {
    long int last = tree.address;
    tree.move_to_root();
    show_tree_r(tree);
    tree.move_to_address(last); 
}


bool chunk::init_chunk(long int parent_addr) {
    
    ofstream file;
    
    string file_name = "meta/" + tree_name + ".meta";
    file.open(file_name, fstream::binary | fstream::out);
    
    long int addr = file.tellp();
    file.write((char *) &(addr), sizeof(addr));
    this->parent_address = parent_addr;

    this->address = addr;
    file.write((char *) &(parent_addr), sizeof(parent_addr));

    child_addresses.clear();
    chunk_values.clear();
    refs.clear();
    long int a = -1;
    this->n = 0;
    for (int i = 0; i < this->max_n; i++) {

        file.write((char *) &a, sizeof(a));
        child_addresses.push_back(-1);
        file.write((char *) &a, sizeof(a));
        chunk_values.push_back(-1);
        file.write((char *) &a, sizeof(a));
        refs.push_back(-1);
    } 
    file.write((char *) &a, sizeof(a));
    child_addresses.push_back(-1);
    
    file.close();
    this->isleaf = 1;
    return true;
}

void flush(vector <long int> & vec) {
    for (vector<long int>::iterator it = vec.begin(); it != vec.end(); it++) *it = -1;
} 

bool chunk::update() {
    
    ofstream file;    
    
    string filename = "meta/" + tree_name + ".meta";

    file.open(filename, fstream::binary | fstream::out | fstream::in);
    file.seekp(this->address);
    file.write((char *) &(this->address), sizeof(this->address));
    file.write((char *) &(this->parent_address), sizeof(this->parent_address));    
    int i;
    this->n = 0;
    for (i = 0; i < this->max_n; i++) {
        // cout << "on update\t" << chunk_values[i] << " " << child_addresses[i] << " " << (this->parent_address) << endl;
        file.write((char *) &child_addresses[i], sizeof(child_addresses[i]));
        file.write((char *) &chunk_values[i], sizeof(chunk_values[i]));
        file.write((char *) &refs[i], sizeof(refs[i]));
        if (this->refs[i] != -1) {
            this->n++;
        }
        if (this->child_addresses[i] != -1) {
            this->isleaf = 0;
        }
    }
    file.write((char *) &child_addresses[i], sizeof(child_addresses[i]));

    if (this->child_addresses[i] != -1) {
        this->isleaf = 0;
    }

    file.close();

    return true;
}

bool chunk::load_child_chunk(int idx) {
    this->isleaf = 1;
    this->isroot = 0;
    this->n = 0;

    if (child_addresses[idx] == -1) {
        
        ofstream file;
        
        file.open("meta/" + tree_name + ".meta", fstream::binary | fstream::out | fstream::in);
        file.seekp(0,file.end);
        long len = file.tellp();

        child_addresses[idx] = len;
        file.close();
        this->update();
        
        file.open("meta/" + tree_name + ".meta", fstream::binary | fstream::out | fstream::in);
        file.seekp(len);

        file.write((char *) &(this->child_addresses[idx]), sizeof(this->child_addresses[idx]));
        this->parent_address = this->address;

        this->address = child_addresses[idx];
        file.write((char *) &(this->parent_address), sizeof(this->parent_address));

        child_addresses.clear();
        chunk_values.clear();
        refs.clear();
        long int a = -1;
        int i;
        for (i = 0; i < this->max_n; i++) {
            file.write((char *) &a, sizeof(a));
            child_addresses.push_back(-1);
            file.write((char *) &a, sizeof(a));
            chunk_values.push_back(-1);
            file.write((char *) &a, sizeof(a));
            refs.push_back(-1);
            if (this->refs[i] != -1) {
                this->n++;
            }
            if (this->child_addresses[i] != -1) {
                this->isleaf = 0;
            }
        } 
        file.write((char *) &a, sizeof(a));
        child_addresses.push_back(-1);
        if (this->child_addresses[i] != -1) {
            this->isleaf = 0;
        }
   
        file.close();

    } else {
        this->isleaf = 1;
        ifstream file;
        this->update();
        file.open("meta/" + tree_name + ".meta", fstream::binary | fstream::in);

        file.seekg(child_addresses[idx]);

        file.read((char *) &(this->address), sizeof(long int));
        file.read((char *) &(this->parent_address), sizeof(long int));
        
        child_addresses.clear();
        chunk_values.clear();
        refs.clear();
        
        int i;
        long int buf;

        for (i = 0; i < this->max_n; i++) {
            file.read((char *) &(buf), sizeof(long int));
            child_addresses.push_back(buf);
            file.read((char *) &(buf), sizeof(long int));
            chunk_values.push_back(buf);
            file.read((char *) &(buf), sizeof(long int));
            refs.push_back(buf);
            if (this->refs[i] != -1) {
                this->n++;
            }

            if (this->child_addresses[i] != -1) {
                this->isleaf = 0;
            }
        } 
        file.read((char *) &buf, sizeof(buf));
        child_addresses.push_back(buf);
        
        if (this->child_addresses[i] != -1) {
            this->isleaf = 0;
        }
        file.close();
    }

    return true;

}

chunk::chunk(string tree_name)
{

    string file_name = "meta/" + tree_name + ".meta";
    this->_file = new metafile(file_name);
    this->tree_name = tree_name;
    this->address = 0;
    this->parent_address = 0;
    this->max_n = 18;
    if (_file->isnew()) {
        delete _file;
        this->parent_address = -1;
        this->isleaf = 1;
        this->root_address = 0;
        init_chunk(-1);
    } else {
        delete _file;
        while (this->parent_address != -1) {
            load_addr_chunk(this->parent_address, true);
        }
    } 

    this->chunksize = sizeof(long int) * (3*this->max_n + 3); //val + refs + addrs + parent 

}

bool chunk::move_to_root() {
    this->move_to_address(this->root_address);
}

bool chunk::is_root() {
    return address == root_address;
}

long int chunk::insert(long int & root_addr, long int k, long int ref)
{   
    if (root_addr != -1) {
        long int p = this->searchforleaf(k, root_addr, -1, 0);
        long int q = -1;
        int e = k;

        // If the leaf node is empty, simply
        // add the element and return
        load_addr_chunk(p);

        for (int e = k; p!=-1; p = this->parent_address) {
            if (this->n == 0) {
                this->chunk_values[0] = e;
                this->refs[0] = ref;
                this->update();
                return root_addr;
            }
            // If number of filled keys is less than maximum
            if (this->n < this->max_n - 1) {
                int i;
                for (i = 0; i < this->n; i++) {
                    if (this->chunk_values[i] > e) {
                        for (int j = this->n - 1; j >= i; j--) {
                            this->chunk_values[j + 1] = this->chunk_values[j];
                            this->refs[j + 1] = this->refs[j];
                        }
                        break;
                    }
                }
                this->chunk_values[i] = e;
                this->refs[i] = ref;
                this->n = this->n + 1;
                return root_addr;
            }

            // If number of filled keys is equal to maximum
            // and it's not root and there is space in the parent
            // cout << "and it's not root and there is space in the parent" << endl;
            chunk *parent = NULL;
            if (this->parent_address != -1) {
                parent = new chunk(this->tree_name);
                cout << this->parent_address << endl;
            }
            // cout << this->max_n << " " << this->n << " " << parent->n << endl; 
            // show_tree(*this);
            // cout << ((this->n == this->max_n - 1) ? 1 : 0) << ((parent!=NULL) ? 1 : 0) << ((parent->n < this->max_n) ? 1 : 0) << endl;  
            if (this->n == this->max_n - 1 && parent!=NULL && parent->n < this->max_n) { /////////// essa linha pode ser problemativa
                int m;

                for (int i = 0; i < parent->n; i++) {
                    cout << parent->child_addresses[i] << " " << this->address << endl;
                    if (parent->child_addresses[i] == this->address) {
                        m = i;
                        break;
                    }
                }
                cout << "tree" << endl;
                show_tree(*this);
                // If right sibling is possible
                if (m + 1 <= this->max_n - 1)
                {
                    // q is the right sibling
                    cout << "q is the right sibling" << endl;
                    q = parent->child_addresses[m + 1];
                    if (q != -1) {
                        chunk *q_chunk;
                        q_chunk = new chunk(this->tree_name);
                        q_chunk->load_addr_chunk(q);
                        // If right sibling is full
                        cout << "If right sibling is full" << endl;
                        if (q_chunk->n == this->max_n - 1) {

                            vector<int> z;
                            // int* z = new int[((2 * this->max_n) / 3)];
                            long int parent1, parent2;
                            vector<int> marray;
                            vector<int> marray_refs;
                            vector<int> marray_addrs;
                            chunk last(this->tree_name);
                            last.create_addr_chunk(parent->address);
                            // int* marray = new int[2 * this->max_n];
                            int i;
                            for (i = 0; i < this->n; i++) {
                                marray.push_back(this->chunk_values[i]);
                                marray_refs.push_back(this->refs[i]);
                                marray_addrs.push_back(this->child_addresses[i]);
                            }

                            flush(this->chunk_values);
                            flush(this->refs);
                            flush(this->child_addresses);
                            
                            marray.push_back(e);
                            marray_refs.push_back(ref);
                            marray_addrs.push_back(-1);
                            marray.push_back(parent->chunk_values[m]);
                            marray_refs.push_back(parent->refs[m]);
                            marray_addrs.push_back(parent->child_addresses[m]);
                            
                            for (int j = i + 2; j < ((i + 2) + (q_chunk->n)); j++) {
                                marray.push_back(q_chunk->chunk_values[j - (i + 2)]);
                                marray_refs.push_back(q_chunk->refs[j - (i + 2)]);
                                marray_addrs.push_back(q_chunk->child_addresses[j - (i + 2)]);
                            }

                            flush(q_chunk->chunk_values);
                            flush(q_chunk->refs);
                            flush(q_chunk->child_addresses);
                            
                            // marray=bubblesort(marray, 2*this->max_n)
                            // a more rigorous implementation will
                            // sort these elements

                            // Put first (2*this->max_n-2)/3 elements into keys of p
                            for (int i = 0; i < (2 * this->max_n - 2) / 3; i++) {
                                this->chunk_values[i] = marray[i];
                                this->refs[i] = marray[i];
                            }
                            parent1 = marray[(2 * this->max_n - 2) / 3];
                            long int parent1_ref = marray_refs[(2 * this->max_n - 2) / 3];

                            // Put next (2*this->max_n-1)/3 elements into keys of q
                            for (int j = ((2 * this->max_n - 2) / 3) + 1; j < (4 * this->max_n) / 3; j++) {
                                q_chunk->chunk_values[j - ((2 * this->max_n - 2) / 3 + 1)] = marray[j];
                                q_chunk->refs[j - ((2 * this->max_n - 2) / 3 + 1)] = marray_refs[j];
                            }
                            parent2 = marray[(4 * this->max_n) / 3];
                            long int parent2_ref = marray_refs[(4 * this->max_n) / 3];

                            // Put last (2*this->max_n)/3 elements into keys of r
                            for (int f = ((4 * this->max_n) / 3 + 1); f < 2 * this->max_n; f++) {
                                last.chunk_values[f - ((4 * N) / 3 + 1)] = marray[f];
                                last.refs[f - ((4 * N) / 3 + 1)] = marray_refs[f];
                            }

                            // Because m=0 and m=1 are children of the same key,
                            // a special case is made for them
                            if (m == 0 || m == 1) {
                                parent->chunk_values[0] = parent1;
                                parent->chunk_values[1] = parent2;
                                parent->child_addresses[0] = p;
                                parent->child_addresses[1] = q_chunk->address;
                                parent->child_addresses[2] = last.address;

                                parent->refs[0] = parent1_ref;
                                parent->refs[1] = parent2_ref;
                            }
                            else {
                                parent->chunk_values[m - 1] = parent1;
                                parent->chunk_values[m] = parent2;
                                parent->child_addresses[m - 1] = p;
                                parent->child_addresses[m] = q_chunk->address;
                                parent->child_addresses[m + 1] = last.address;
                            
                                parent->refs[m - 1] = parent1_ref;
                                parent->refs[m] = parent2_ref;
                            
                            }

                            parent->update();
                            q_chunk->update();
                            last.update();
                            this->update();
                            return root_addr;
                        }
                    }
                    else // If right sibling is not full
                    {
                        int put, put_ref;
                        chunk *q_chunk;
                        q_chunk = new chunk(this->tree_name);
                        q_chunk->load_addr_chunk(q);

                        if (m == 0 || m == 1) {
                            put = parent->chunk_values[0];
                            put_ref = parent->refs[0];
                        }
                        else {
                            put = parent->chunk_values[m - 1];
                            put_ref = parent->refs[m - 1];
                        }
                        for (int j = (q_chunk->n) - 1; j >= 1; j--)
                            q_chunk->chunk_values[j + 1] = q_chunk->chunk_values[j];
                        q_chunk->chunk_values[0] = put;
                        parent->chunk_values[m == 0 ? m : m - 1] = this->chunk_values[this->n - 1];

                        q_chunk->refs[0] = put_ref;
                        parent->refs[m == 0 ? m : m - 1] = this->refs[this->n - 1];

                        parent->update();
                        q_chunk->update();
                        this->update();
                    }
                }
            }

            load_addr_chunk(p);
        }

        /*Cases of root splitting, etc. are omitted
         as this implementation is just to demonstrate
         the two-three split operation*/
    }
    else
    {
        this->create_addr_chunk(this->parent_address);

    }

}

// // bool mode_to_addres

long int chunk::searchforleaf(int k, long int & root, long int parent, int chindex) {

    // If the passed root is a leaf node, then
    if (root != -1) {
        // k can be inserted in this node itself
        this->load_addr_chunk(root);
        if (this->isleaf == 1)
            return root;

        // If the passed root is not a leaf node,
        // implying there are one or more children
        else {
            int i;

            /*If passed root's initial key is itself g
            reater than the element to be inserted,
            we need to insert to a new leaf left of the root*/
            if (k < chunk_values[0])
                root = searchforleaf(k, child_addresses[0], root, 0);

            else {
                // Find the first key whose value is greater
                // than the insertion value
                // and insert into child of that key
                for (i = 0; i < this->max_n; i++)
                    if (chunk_values[i] > k)
                        root = searchforleaf(k, child_addresses[i], root, i);

                // If all the keys are less than the insertion
                // key value, insert to the right of last key
                if (chunk_values[i - 1] < k)
                    root = searchforleaf(k, child_addresses[i], root, i);
            }
        }
    } else {
        load_child_chunk(chindex);
        return this->address;
    }
}

// // Function to traverse all nodes in a subtree rooted with this node 
// void chunk::traverse() 
// { 
//     // There are n keys and n+1 children, travers through n keys 
//     // and first n children 
//     int i; 
//     long int addr = this->address;
//     for (i = 0; i < this->n; i++) 
//     { 
//         // If this is not leaf, then before printing key[i], 
//         // traverse the subtree rooted with child C[i]. 
//         if (this->isleaf == 0) {
//             move_to_child(i);
//             // C[i]->traverse();
//             move_to_address(addr); 
//         }
//         cout << " " << this->chunk_values[i]; 
//     } 
  
//     // Print the subtree rooted with last child 
//     if (this->isleaf == 0) {
//         move_to_child(i);
//         // C[i]->traverse();
//         move_to_address(addr);
//     } 
// } 
  
// // Function to search key k in subtree rooted with this node 
// long int chunk::search(int k) 
// { 
//     // Find the first key greater than or equal to k 
//     int i = 0; 
//     while (i < this->n && k > this->chunk_values[i]) i++; 
  
//     // If the found key is equal to k, return this node 
//     if (this->chunk_values[i] == k) 
//         return this->address; 
  
//     // If key is not found here and this is a leaf node 
//     if (isleaf == true) 
//         return -1; 
  
//     // Go to the appropriate child
//     move_to_child(i);
//     return this->search(k); 
// } 
  
// // The main function that inserts a new key in this B-Tree 
// long chunk::insert(long int & root_addr, long int k, long int ref) 
// { 
//     move_to_root();
//     // If tree is empty
//     if (this->n == 0) 
//     { 
//         // Allocate memory for root 
//         this->chunk_values[0] = k;  // Insert key
//         this->refs[0] = ref;  // Insert key 
//         this->n = 1;  // Update number of keys in root 
//     } 
//     else // If tree is not empty 
//     { 
//         // If root is full, then tree grows in height 
//         if (this->n == this->max_n) 
//         { 
//             // Allocate memory for new root
//             create_addr_chunk(this->address); 
//             this->
//             // Make old root as child of new root 
//             s->C[0] = root; 
  
//             // Split the old root and move 1 key to the new root 
//             s->splitChild(0, root); 
  
//             // New root has two children now.  Decide which of the 
//             // two children is going to have new key 
//             int i = 0; 
//             if (s->keys[0] < k) 
//                 i++; 
//             s->C[i]->insertNonFull(k); 
  
//             // Change root 
//             root = s; 
//         } 
//         else  // If root is not full, call insertNonFull for root 
//             root->insertNonFull(k); 
//     } 
// } 
  
// // A utility function to insert a new key in this node 
// // The assumption is, the node must be non-full when this 
// // function is called 
// void BTreeNode::insertNonFull(int k) 
// { 
//     // Initialize index as index of rightmost element 
//     int i = n-1; 
  
//     // If this is a leaf node 
//     if (leaf == true) 
//     { 
//         // The following loop does two things 
//         // a) Finds the location of new key to be inserted 
//         // b) Moves all greater keys to one place ahead 
//         while (i >= 0 && keys[i] > k) 
//         { 
//             keys[i+1] = keys[i]; 
//             i--; 
//         } 
  
//         // Insert the new key at found location 
//         keys[i+1] = k; 
//         n = n+1; 
//     } 
//     else // If this node is not leaf 
//     { 
//         // Find the child which is going to have the new key 
//         while (i >= 0 && keys[i] > k) 
//             i--; 
  
//         // See if the found child is full 
//         if (C[i+1]->n == 2*t-1) 
//         { 
//             // If the child is full, then split it 
//             splitChild(i+1, C[i+1]); 
  
//             // After split, the middle key of C[i] goes up and 
//             // C[i] is splitted into two.  See which of the two 
//             // is going to have the new key 
//             if (keys[i+1] < k) 
//                 i++; 
//         } 
//         C[i+1]->insertNonFull(k); 
//     } 
// } 
  
// // A utility function to split the child y of this node 
// // Note that y must be full when this function is called 
// void chunk::splitChild(int i, long int address) 
// { 
//     // Create a new node which is going to store (t-1) keys 
//     // of y 
//     create_addr_chunk(-1); 
//     // BTreeNode *z = new BTreeNode(y->t, y->leaf); 
//     chunk *y = new chunk(this->name);
//     y->move_to_address(address);
//     // Copy the last (t-1) keys of y to z 
//     for (int j = 0; j < this->max_n/2 - 1; j++) 

//         z->keys[j] = y->keys[j+t]; 
  
//     // Copy the last t children of y to z 
//     if (y->leaf == false) 
//     { 
//         for (int j = 0; j < t; j++) 
//             z->C[j] = y->C[j+t]; 
//     } 
  
//     // Reduce the number of keys in y 
//     y->n = t - 1; 
  
//     // Since this node is going to have a new child, 
//     // create space of new child 
//     for (int j = n; j >= i+1; j--) 
//         C[j+1] = C[j]; 
  
//     // Link the new child to this node 
//     C[i+1] = z; 
  
//     // A key of y will move to this node. Find the location of 
//     // new key and move all greater keys one space ahead 
//     for (int j = n-1; j >= i; j--) 
//         keys[j+1] = keys[j]; 
  
//     // Copy the middle key of y to this node 
//     keys[i] = y->keys[t-1]; 
  
//     // Increment count of keys in this node 
//     n = n + 1; 
// } 


// int main () {
//     chunk tree("CLIENTES_CODIGO");
//     show_tree(tree);
// }
// Driver code
// int main()
// {
//     /* Consider the following tree that has been obtained  
//        from some root split: 
//                 6              
//                 / \              
//             1 2 4 7 8 9 
                  
//             We wish to add 5. This makes the B*-tree: 
//                 4 7              
//                 / \ \          
//             1 2 5 6 8 9  
              
//         Contrast this with the equivalent B-tree, in which 
//         some nodes are less than half full 
              
//             4 6  
//             / \ \ 
//             1 2 5 7 8 9 
              
//     */

//     int fd;
//     struct statvfs buf;
//     if (statvfs(".", &buf) == -1)
//         perror("statvfs() error");
//     else {
//         printf("each block is %d bytes big\n",
//                buf.f_frsize);
//         printf("there are %d blocks available out of a total of %d\n",
//                buf.f_bavail, buf.f_blocks);
//         printf("in bytes, that's %.0f bytes free out of a total of %.0f\n",
//                ((double)buf.f_bavail * buf.f_frsize),
//                ((double)buf.f_blocks * buf.f_frsize));
//         cout << "res size " << (4*sizeof(long int)) << endl;
//         cout << "us: " << sizeof(long int) << endl;

//     }

//     chunk tree("teste");

//     cout << "created" << endl;
//     // Start with an empty root
//     // struct node* root = NULL;
//     // Insert 6
//     cout << "one was inserted /\\ /\\" << endl;
//     cout << "root " << 0 << endl;

//     tree.move_to_root();
//     // tree.move_to_child(0);
//     // Insert 1, 2, 4 to the left of 6
//     tree.insert(tree.address, 1, 1);
//     tree.insert(tree.address, 2, 1);
//     tree.insert(tree.address, 4, 1);
//     tree.insert(tree.address, 6, 1);
    
//     // tree.update();

//     // // Insert 7, 8, 9 to the right of 6
//     // tree.move_to_root();
//     // tree.move_to_child(1);
//     // // Insert 1, 2, 4 to the left of 6
//     tree.insert(tree.address, 7, 1);
//     tree.insert(tree.address, 8, 1);
//     tree.insert(tree.address, 9, 1);
    
//     tree.update();
//     // cout << "Original tree: " << endl;
//     tree.move_to_root();

//     // for (int i = 0; i < tree.n; i++)
//     //     cout << tree.chunk_values[i] << " ";
//     // cout << endl;
//     // for (int i = 0; i < 2; i++) {
//     //     tree.move_to_root();
//     //     tree.move_to_child(i);
//     //     cout << tree.chunk_values[0] << " ";
//     //     cout << tree.chunk_values[1] << " ";
//     //     cout << tree.chunk_values[2] << " ";
//     // }
//     // cout << endl;

//     // cout << "After adding 5: " << endl;

//     // tree.move_to_root();
//     // // Inserting element '5':
//     // tree.move_to_child(0);

//     // cout << "Inserting at " << tree.address <<  endl;    
//     // tree.insert(tree.address, 5, 1);

//     // // Printing nodes
//     // tree.move_to_root();

//     // for (int i = 0; i <= tree.n; i++)
//     //     cout << tree.chunk_values[i] << " ";
//     // cout << endl;
//     // for (int i = 0; i < N - 1; i++) {
//     //     tree.move_to_child(i);
//     //     cout << tree.chunk_values[0] << " ";
//     //     cout << tree.chunk_values[1] << " ";
//     //     tree.move_to_root();
//     // }

//     show_tree(tree);

//     return 0;
// }