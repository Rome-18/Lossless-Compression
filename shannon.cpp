//
//  shannon.cpp
//  Lossless
//
//  Created by Yuan Liu on 1/27/15.
//  Copyright (c) 2015 Yuan Liu. All rights reserved.
//

#include "shannon.h"
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
#include <algorithm>

using namespace std;



// Node struct defined in huffman.h

void shannon_routine(char *filename){
    
    int frequency[256]={};
    int node_count=0;
    
    
    char_count(filename, frequency);
    
    int i=0;
    // Count how many nodes are there
    while(i<256){
        if(frequency[i]!=0){
            node_count++;
        }
        i++;
    }
    
    
  //  cout<<"number of node is: "<<node_count<<endl;
    
    
    Node array[node_count];
   
    makeShannonArray(frequency,node_count, array);
    
//    cout<<"check sorted array by weight begin"<<endl;
//    for (int x=0; x<node_count;x++){
//        
//        cout<<array[x].val<<" "<<array[x].weight<<endl;
//    }
//    cout<<"check sorted array by weight end"<<endl;
    
    Node *root=shannonTree(array, 0, node_count-1);
    
    ShannonTable table[256]={};
    
    shannonCode(table, root, 0,0,0);
    
//    for (int x=0;x<256;x++){
//        cout<<x<<" "<<table[x].codeLength<<endl;
//    }

    doCompression(filename, table);
    
    
}

void makeShannonArray(int *frequency, int nodeNum, Node array[]){
    
    
   // Node *temp;
   // Node *array[nodeNum]; // store non-zero nodes
    int idx, nodeidx=0;
    
    for (idx=0;idx<256;idx++){
        if(frequency[idx]!=0){
            array[nodeidx] = *new Node;
            array[nodeidx].val=idx;
            array[nodeidx].weight=frequency[idx];
            array[nodeidx].left=NULL;
            array[nodeidx].right=NULL;
            nodeidx++;
        }
    }
    
    
    // Since need to access the original sorted array multiple times, use selection sort to sort the array for further use.
    for (int idx=0; idx<nodeNum; idx++){
        
        int max=idx;
        for (int current=idx+1; current<nodeNum;current++){
            if(array[current].weight>array[max].weight){
                max=current;
            }
        }
        swap(array[idx],array[max]);
    }

}


Node* shannonTree(Node *array, int pos1, int pos2){
    int idxleft=pos1, idxright=pos2, sumleft=array[pos1].weight, sumright=array[pos2].weight;

    
    if(pos1==pos2){
        return &array[pos1];
    }else{
        while(idxleft!=idxright-1){  // start from two end to center, add one node to the side with less weight
            
            if(sumleft<sumright){   // if left side's weight is less than right side, add one node to left
                idxleft=idxleft+1;
                sumleft=sumleft+array[idxleft].weight;

            }else{
                idxright=idxright-1;
                sumright=sumright+array[idxright].weight;
            }
        }
        
            Node *temp=new Node;
            int sum=0;

            // Calculate weight for parent node
            for (int idx=pos1;idx<=pos2;idx++){
                sum=sum+array[idx].weight;
            }
        
            temp->weight=sum;
            temp->val=0;
            temp->left=shannonTree(array, pos1, idxleft);
            temp->right=shannonTree(array, idxright, pos2);
        
        return temp;
    }
}


void shannonCode(ShannonTable *table, Node *root, int code, int bit, int length){
    code = code << 1;
    code += bit;
    // root1->code=code;
    
    if (root->left!=NULL && root->right!=NULL) {
        length++;
        shannonCode(table, root->left, code, 0, length);
        shannonCode(table, root->right, code, 1, length);
    }
    else {
        table[root->val].code = code;
        table[root->val].codeLength = length;
       // cout<<root->val<<" "<<table[root->val].code<<" "<<table[root->val].codeLength<<endl;
    }
    
}


void doCompression(char* filename, ShannonTable *table){
    
    int bit_count=0;
    char *output=new char[64];
    strcat(output, filename);
    strcat(output, "-shannon-fano.out");
    
    BIT_FILE *output_file;
    output_file = OpenOutputBitFile(output);
    
    
    ifstream infile(filename, ios_base::in | ios_base::binary);
    unsigned char sym;
    // first character;
    sym= infile.get();
    
    while(infile.good())
    {
        OutputBits(output_file, table[sym].code, table[sym].codeLength);
        bit_count+=table[sym].codeLength;
        sym=infile.get();
    }
    
    infile.close();
    CloseOutputBitFile(output_file);
    cout<<"Shannon compressed size: "<<bit_count<<" bits"<<endl;
    
}
