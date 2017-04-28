#include "hashheap.h"

HashHeap::HashHeap(int size){
    tableSize = size;
    heapSize = 0;
    hashTable = new Node* [size];
    heap = new int[size];
    for(int i = 0; i < tableSize; i++){
        hashTable[i] = NULL;
        heap[i] = 0;
    }
}

HashHeap::~HashHeap(){
    for(int i = 0; i < tableSize; i++){
        clear(hashTable[i]);
    }
    delete[] hashTable;
    delete[] heap;
}

void HashHeap::clear(Node *n){
    if (n) {
        clear(n->child);
        delete n;
    }
}

int HashHeap::hash(int i){
    return i%tableSize;
}

int HashHeap::getParentIndex(int i){
    return (i-1)/2;
}

int HashHeap::getLChildIndex(int i){
    return 2*i+1;
}

int HashHeap::getRChildIndex(int i){
    return 2*i+2;
}

void HashHeap::swapNodes(int i, int j){
    Node* iNode = HashHeap::lookup(heap[i]);
    Node* jNode = HashHeap::lookup(heap[j]);

    int temp = heap[j];
    heap[j] = heap[i];
    heap[i] = temp;

    iNode->heapIndex = j;
    jNode->heapIndex = i;
}

void HashHeap::percolateUp(int newNodeIndex){
    if(newNodeIndex != 0){
        int parentIndex = getParentIndex(newNodeIndex);
        if(heap[newNodeIndex] > heap[parentIndex]){
            swapNodes(newNodeIndex, parentIndex);   //parentIndex should now contain the node that was at newNodeIndex
            percolateUp(parentIndex);
        }
    }
}

void HashHeap::percolateDown(int index){
    int lChildIndex = getLChildIndex(index);
    int rChildIndex = getRChildIndex(index);

    if(lChildIndex >= heapSize && rChildIndex >= heapSize){ //no children
        return;
    }
    else if(lChildIndex >= heapSize && rChildIndex < heapSize){ //only R child
        if(heap[index] < heap[rChildIndex]){
            swapNodes(index, rChildIndex);
            percolateDown(rChildIndex);
        }
    }
    else if(rChildIndex >= heapSize && lChildIndex < heapSize){ //only L child
        if(heap[index] < heap[lChildIndex]){
            swapNodes(index, lChildIndex);
            percolateDown(lChildIndex);
        }
    }
    else{ //two children
        int largerChildIndex;
        if(heap[lChildIndex] > heap[rChildIndex]){
            largerChildIndex = lChildIndex;
        }
        else largerChildIndex = rChildIndex;

        if(heap[index] < heap[largerChildIndex]){
            swapNodes(index, largerChildIndex);
            percolateDown(largerChildIndex);
        }
    }
}

void HashHeap::insert(int i){
    if(lookup(i) != NULL){
        cout << "error: item already exists" << endl;
        return;
    }

    Node* tempNode = hashTable[hash(i)];

    if(tempNode == NULL){
        tempNode = new Node(i, heapSize);
        hashTable[hash(i)] = tempNode;
    }
    else{
        while(tempNode->child != NULL){
            tempNode = tempNode->child;
        }
        tempNode->child = new Node(i, heapSize);
    }

    heap[heapSize] = i;
    percolateUp(heapSize);
    heapSize++;
}

HashHeap::Node* HashHeap::lookup(int i){
    Node* tempNode = hashTable[hash(i)];
    while(tempNode != NULL){
        if(tempNode->value == i){
            return tempNode;
        }
        else tempNode = tempNode->child;
    }

    return NULL;
}

int HashHeap::deleteMax(){
    if(heapSize == 0){
        cout << "error" << endl;
        return -6875843;
    }
    int maxVal = heap[0];

    Node* valNode = hashTable[hash(maxVal)];
    Node* parentNode = NULL;
    while(valNode->value != maxVal){
        parentNode = valNode;
        valNode = valNode->child;
    }
    if(parentNode != NULL){
        parentNode->child = valNode->child;
    }
    else hashTable[hash(maxVal)] = valNode->child;
    delete valNode;

    heap[0] = heap[heapSize - 1];
    heap[heapSize - 1] = 0;
    heapSize--;
    percolateDown(0);

    return maxVal;
}

void HashHeap::deleteVal(int i){

    if(lookup(i) == NULL){
        cout << "error: item not present" << endl;
        return;
    }

    Node* valNode = hashTable[hash(i)];
    Node* parentNode = NULL;
    while(valNode->value != i){
        parentNode = valNode;
        valNode = valNode->child;
    }
    if(parentNode != NULL){
        parentNode->child = valNode->child;
    }
    else hashTable[hash(i)] = valNode->child;
    int index = valNode->heapIndex;
    delete valNode;

    heap[index] = heap[heapSize - 1];
    heap[heapSize - 1] = 0;
    heapSize--;

    if(heap[index] > heap[getParentIndex(index)]){
        percolateUp(index);
    }
    else percolateDown(index);
}

void HashHeap::print(){
    int numValues = heapSize;
    for(int i = 0; i < numValues; i++){
        cout << deleteMax();
        if(i != numValues - 1) cout << " ";
    }
}

int main(){

    int size;
    int numFunctions;
    cin >> size;
    cin >> numFunctions;

    HashHeap* h = new HashHeap(size);

    string s;
    int param;

    for(int i = 0; i < numFunctions; i++){
        cin >> s;

        if(s == "insert"){
            cin >> param;
            h->insert(param);
        }
        else if(s == "delete"){
            cin >> param;
            h->deleteVal(param);
        }
        else if(s == "lookup"){
            cin >> param;
            if(h->lookup(param) != NULL){
                cout << "found ";
                cout << param << endl;
            }
            else{
                cout << param;
                cout << " not found" << endl;
            }
        }
        else if(s == "deleteMax"){
            int i = h->deleteMax();
            if(i != -6875843) cout << i << endl;
        }
        else if(s == "print"){
            h->print();
        }
    }

    return 0;
}

