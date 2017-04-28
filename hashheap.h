//hashheap.h

#include <iostream>
#include <string>
using namespace std;

class HashHeap {

public:

    struct Node {    //node definition
        int value;
        int heapIndex;
        Node *child;
        Node(int v, int h) : value(v), heapIndex(h), child(0) { }
    };

	HashHeap(int size);
	~HashHeap();
	void insert(int i);
	Node* lookup(int i);
	int deleteMax();
	void deleteVal(int i);
	void print();

//private:

    //helper functions
    void clear(Node *n);    //clear n + all n's descendents
    int hash(int i);
    void percolateUp(int newNodeIndex);
    void percolateDown(int index);
    int getParentIndex(int i);
    int getLChildIndex(int i);
    int getRChildIndex(int i);
    void swapNodes(int i, int j);

    //private fields
    int tableSize;  //capacity of hash table
    int heapSize;   //#elements in heap
    Node** hashTable;
    int* heap;
};
