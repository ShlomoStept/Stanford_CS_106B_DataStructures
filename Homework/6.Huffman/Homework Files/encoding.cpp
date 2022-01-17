/*
 * File: encoding.cpp
 * --------------------------
 * Name: Shlomo Stept
 * Section leader: N/A --> Self administered course
 * This file contains my implimentation of Huffman compression
 */

#include "encoding.h"
#include "pqueue.h"
#include "filelib.h"


    //  1.0 -> buildFrequencyTable
// :: Build the Frequence table map by removing each char one at a time and incrementing its value in the map.
//------------------------------------------------------------------------------------------------------------
Map<int, int> buildFrequencyTable(istream& input) {
    Map<int, int> freqTable;

    int next = input.get();  // Key == use int rather than char for next, in this func and in 2.0 and 4.0
    while( next != -1 ){
        freqTable[next]++;
        next = input.get();
    }
    freqTable[PSEUDO_EOF]++;
    return freqTable;
}
//------------------------------------------------------------------------------------------------



    // 2.0 -->buildEncodingTree
// :: Build the encoding tree using the frequncy table/map and special huffman nodes.
//------------------------------------------------------------------------------------------------------------
HuffmanNode* buildEncodingTree(const Map<int, int>& freqTable) {
    PriorityQueue<HuffmanNode *> pq;
    Vector<int> map = freqTable.keys();

    // Step 1: Build the priority queue
    for(int key: map){
            int val = freqTable[key];
            HuffmanNode *next = new HuffmanNode(key,val);
            pq.enqueue(next, val);
    }

    // Step 2: create the Encoding tree (a) remove first two nodes, combine, and add new node into the queue, untill priority queue has one node in it
    while(pq.size() > 1){
        HuffmanNode *first, *second;
        first = pq.dequeue();
        second = pq.dequeue();
        int comb_val = (first->count + second->count);
        HuffmanNode *next = new HuffmanNode(NOT_A_CHAR, comb_val, first, second);
        pq.enqueue(next, comb_val);
    }

    //if(pq.size()<1){
    //    HuffmanNode *garb = new HuffmanNode() ;
    //    return garb;
    //}
    return pq.dequeue();
}
//------------------------------------------------------------------------------------------------



//------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------
    // 3.5 --> Helper function for :: buildEncodingMap ::
 // :: Helper function:  that takes in the string representing tha path taken so far and either:
 //   ----> (i) Adds the current path to the map if its the end of the path
 //   ----> (ii) Or Calls this functions and passes the right and left path (and the appropriate char) to each seperate call
//------------------------------------------------------------------------------------------------------------
Map<int, string> buildEncodingMap_helper(HuffmanNode* encodingTree, string sofar){
    Map<int, string> encodingMap;
    if(encodingTree != nullptr){
        // Case 1: this node is not a charachter --> recursivly call this helper funct on the left and right nodes
        if(encodingTree->character == NOT_A_CHAR){
            return buildEncodingMap_helper(encodingTree->zero, sofar+"0")
                    + buildEncodingMap_helper(encodingTree->one, sofar+"1");

        // Case 2: this node is a char (the end of a path) --> add this char and its encoding to the Map
        }else{
            encodingMap[encodingTree->character] = sofar;
        }
    }
    return encodingMap;
}


    // 3.0 -> buildEncodingMap
// :: Build the encoding map, i.e binary code that represents each charachter in the encoding tree
//------------------------------------------------------------------------------------------------------------
Map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    Map<int, string> encodingMap;
    // NOTE: If the entered tree is not null this passes the encoding tree node to the helper function
    if(encodingTree != nullptr){
        if(encodingTree->character !=NOT_A_CHAR){
            encodingMap[encodingTree->character] = "";
        }
        encodingMap += buildEncodingMap_helper(encodingTree->zero, "0")
                + buildEncodingMap_helper(encodingTree->one, "1");
    }
    //cout << encodingMap.toString() <<endl;
    return encodingMap;
}
//---------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------



    //  4.0 -> encodeData
// :: Using the encoding map and the original file to create the output of the compressed version of the file bit by bit
//------------------------------------------------------------------------------------------------------------
void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output) {

    int next = input.get();
    string to_encode;
    while( next != -1 ){
        to_encode += encodingMap[int(next)];
        next = input.get();
    }
    to_encode += encodingMap[PSEUDO_EOF];

    for(char C: to_encode){
        output.writeBit(int(C-'0'));
    }
}
//---------------------------------------------------------------------------------------------------



    //  5.0 -> DecodeData
// :: Using the Encoding tree to decode a string of bits into a word
    //  --> Note:: TREE TRAVERSAL not implimented recursivly. It appears Highly impractical in this case. (maybe i will come back later to figure how to do that)
    //  --> (1) While the end of bits or a charachter in the tree has not been reached, read next bit and go right or left depending on its value
    //  --> (2) if a char was reached, output it, and go back to top of tree, (3) is end of file reached, end funct
//------------------------------------------------------------------------------------------------------------
void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    HuffmanNode* curr = encodingTree;
    int next;
    bool end = false;
    while(!end){
        bool found = false;
        while(!found){
            if(curr->character != NOT_A_CHAR){
                if(curr->character == PSEUDO_EOF){
                    end = true;
                    break;
                }
                output.put(curr->character);
                curr = encodingTree;
                break;
            } else{
                next = input.readBit();
                /* --> This is not nessecary if we assume the input file has no errors <--*/
                if(next == -1 ){
                    end = true;
                    break;
                }else
                if( next == 1 ){
                    curr = curr->one;
                }else if( next == 0 ){
                    curr = curr->zero;
                }
            }
        }
    }
}
//----------------------------------------------------------------------------------------




    //  6.0 -> compress
// :: Use the previos functions to compress a file ::  (incluse the freq map as the file header)
//------------------------------------------------------------------------------------------------------------
void compress(istream& input, obitstream& output) {
    // Step 1: build the frequency table
    Map<int, int> FreqMap = buildFrequencyTable(input);

    // Step 1.5: add the frequency table bit by bit as the file header
    output << FreqMap;

    // Step 2: Build the frequency tree
    HuffmanNode * FreqTree_root = buildEncodingTree(FreqMap);

    // Step 3: Build the Enoding map (to use to encode the data)
    Map<int, string> EncodeMap = buildEncodingMap(FreqTree_root);
    //cout << EncodeMap.toString() <<endl;

    // Step 4: Encode the Data; (i.e use the previosuly written function to bitwise conpress the files contents)
    rewindStream(input); // * KEY PART I WAS MISSING!!, since i fully read the file in the "buildFrequencyTable" Function i need to reqind it before reading it again.
    encodeData(input, EncodeMap, output);

    // Step 5: Free the Tree
    freeTree(FreqTree_root);

}
//----------------------------------------------------------------------------------------



    //  7.0 -> decompress
// :: Decompress the file
//------------------------------------------------------------------------------------------------------------
void decompress(ibitstream& input, ostream& output) {
    //Step 1: Get the freq map (i.e the header from the file
    Map<int, int> FreqMap;
    input >> FreqMap;

    //cout << FreqMap.toString() <<endl;

    // Step 2: Build the frequency tree
    HuffmanNode * FreqTree_root = buildEncodingTree(FreqMap);

    // Step 3: Use the Freqency map to decode the data
    decodeData(input, FreqTree_root, output);

    // Step 4: Free the Tree
    freeTree(FreqTree_root);

}
//----------------------------------------------------------------------------------------



    //  8.0 -> freeTree
// :: Free the memory allocated in the heap for all the nodes in the tree.
//------------------------------------------------------------------------------------------------------------
void freeTree(HuffmanNode* node) {
    if(node != nullptr){
        freeTree(node->one);
        freeTree(node->zero);
        delete node;
    }
}
//----------------------------------------------------------------------------------------
