//
//  Huffman.cpp
//  Lossless
//
//  Created by Yuan Liu on 1/26/15.
//  Copyright (c) 2015 Yuan Liu. All rights reserved.
//

#include "Huffman.h"
#include "bitio.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <fstream>
#include <queue>
#include <string.h>
#include <vector>

using namespace std;

// Using by priority queue to sort the node list
class nodeComparator{
public:
    bool operator()( Node*a,  Node *b) {
        return a->weight>b->weight;
    }
};
priority_queue<Node*, vector<Node*>, nodeComparator> nodeQueue;


void huffman_routine(char *filename){
    int frequency[256]={};
    int node_count=0;

    
    char_count(filename,frequency);
    
    int i=0;
    // Count how many nodes are there
    while(i<256){
        if(frequency[i]!=0){
            node_count++;
           // cout<<i<<" "<<frequency[i]<<endl;
        }
        i++;
    }
    
    
    
    
   // cout<<"number of node is: "<<node_count<<endl;
    
    Node* root=build_tree(node_count, frequency);
    
    HuffmanTable table[256]={};
    
    HuffCode(table, root, 0, 0, 0);

//    for (int x=0;x<256;x++){
//        if(frequency[x]!=0){
//        cout<<"symbol: "<<x<<" code: "<<table[x].code<<endl;
//        }
//    }
    
    doCompression(filename, table);
    
}



void char_count(char *input,int *frequency){
    
    ifstream infile(input, ios_base::in | ios_base::binary);
    //unsigned char sym;
    unsigned char sym;
    
    sym= infile.get();
    while(infile.good())
    {
        frequency[sym]++;
        sym=infile.get();

    }
    
    infile.close();
}



Node* build_tree(int nodeNum, int *frequency){
    Node *temp;
    Node *array[nodeNum];
    int idx, nodeidx=0;
    
    for (idx=0;idx<256;idx++){
        if(frequency[idx]!=0){
            array[nodeidx] = new Node;
            array[nodeidx]->val=idx;
            array[nodeidx]->weight=frequency[idx];
            array[nodeidx]->left=NULL;
            array[nodeidx]->right=NULL;
            //  array[nodeidx]->parent=NULL;
            nodeQueue.push(array[nodeidx]);
            nodeidx++;
        }
    }
    
    while(nodeQueue.size()>1)
    {
        Node *first=nodeQueue.top();
        
        nodeQueue.pop();
        node *second=nodeQueue.top();
        nodeQueue.pop();
        
        
        
        temp=new Node();
        temp->weight=first->weight+second->weight;
        
        temp->left=first;
        temp->right=second;
        nodeQueue.push(temp);
        
        
    }
    
    return nodeQueue.top();
}


void HuffCode(HuffmanTable table[], node *root, int code, int bit, int length)
{
    
    code = code << 1;
    code += bit;
    // root1->code=code;
    
    if (root->left!=NULL && root->right!=NULL) {
        length++;
        HuffCode(table, root->left, code, 0, length);
        HuffCode(table, root->right, code, 1, length);
    }
    else {
        table[root->val].code = code;
        table[root->val].codeLength = length;
        //   cout<<table[root->val].code<<" "<<table[root->val].codeLength<<endl;
    }
    
}


void doCompression(char* filename, HuffmanTable *table){
    
    int bit_count=0;
    //int size=0;
    char *output=new char[64];
    
    strcat(output, filename);
    strcat(output, "-huffman.out");
    
    BIT_FILE *output_file;
    output_file = OpenOutputBitFile(output);
    
    
    ifstream infile(filename, ios_base::in | ios_base::binary);
    unsigned char sym;
    sym= infile.get();
    
    while(infile.good())
    {
        //size+=8;
        bit_count+=table[sym].codeLength;
        OutputBits(output_file, table[sym].code, table[sym].codeLength);
        sym=infile.get();
    }
    
    infile.close();
    CloseOutputBitFile(output_file);
    //cout<<"Original size is: "<<size/8<<" bytes"<<endl;
    cout<<"Huffman compressed size: "<<bit_count<<" bits"<<endl;
    
}

