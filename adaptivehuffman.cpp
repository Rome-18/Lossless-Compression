//
//  adaptivehuffman.cpp
//  Lossless
//
//  Created by Yuan Liu on 1/28/15.
//  Copyright (c) 2015 Yuan Liu. All rights reserved.
//

#include "adaptivehuffman.h"
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


void adapHuffman_routine(char *filename){
    
    
    doCompression(filename);
    
}


// When newly appeared symbol comes, and tree already built, use this function to get new NYA node.
afNode *getNewNYA(afNode *old, afNode *nodeList, afNode *nodeTree, unsigned char sym){
    afNode *temp=new afNode;
    afNode *newNYA=new afNode;
   
    /* Implement the algorithm to get new NYA 
     and add new symbol to the tree.
     */
    
    // Add new symbol to the right child of old NYA
    temp->weight=1;
    temp->val=sym;
    temp->identifier=old->identifier-1;
    temp->parent=old;
    
    // add new NYA to the left child of old NYA
    newNYA->identifier=old->identifier-2;
    newNYA->parent=old;
    
    // Update old NYA information
    old->weight=1;   // right weight 1 + left weight 0
    old->left=newNYA;
    old->right=temp;
    
    nodeList[sym]=*temp;
    nodeTree[temp->identifier]=*temp;
    nodeTree[newNYA->identifier]=*newNYA;
    
    return newNYA;// make it compile
}


int getCode(afNode *temp, int code, int bit, int *length){
    if(temp->parent==NULL){// this node is root node,
        return code;
    }else{
        *length++;
        if(temp==temp->parent->left){// this node is left node
            code = code << ((*length)-1);
            code += bit;

            return getCode(temp->parent, code, 0, length);
        }else{
            code = code << ((*length)-1);
            code += bit;
            
            return getCode(temp->parent, code, 1, length);
        }
    }
}


void updateTree(afNode *temp, afNode *nodeTree){
    afNode *max, *candidate1, *candidate2;
    max=NULL;
    
    //This is the root;
    if(temp->parent==NULL){
        temp->weight++;  // add one weight to the root, no swap of nodes involved
    }else{
        max=maxNode(temp, nodeTree);
        if(max!=NULL && max!=temp->parent && max->parent!=NULL){  // in such case, need to swap the nodes
           
            candidate1=temp->parent;
            candidate2=max->parent;       
            temp->parent=candidate2;
            
            if(max==candidate2->left){
                candidate2->left=temp;
            }else{
                candidate2->right=temp;
            }
            
            max->parent=candidate1;
            if(temp==candidate1->left){
                candidate1->left=max;
            }else{
                candidate1->right=max;
            }
            
            
            
            nodeTree[temp->identifier]=*max;
            nodeTree[max->identifier]=*temp;
            int id=max->identifier;
            max->identifier=temp->identifier;
            temp->identifier=id;
            
        }else{
            // no need to swap nodes
        }
        temp->weight++;
        updateTree(temp->parent, nodeTree);
        
    }
    
}


// find the max node with same weight, to be swap later in the algorithm
afNode *maxNode(afNode *temp, afNode *nodeTree){
    afNode *max=NULL;
    
    int i;
    for(i=(temp->identifier)+1;i<512;i++){
        if(nodeTree[i].weight==temp->weight){
            max=&nodeTree[i];
        }
    }
    return max;
}

void doCompression(char * filename){
    // table to store appeared symbol and its information, nodeList
    afNode nodeList[256]={};
    // for maxium 256 symbols, total number of nodes is 2*256+NYA nodes in nodeTree
    afNode nodeTree[512]={};
    
    
    // Initialize root;
    afNode *root=new afNode;
    root->identifier=511;
    nodeTree[root->identifier]=*root;
    
    afNode *NYA=NULL;   // control character
    
    
    // input file
    ifstream infile(filename, ios_base::in | ios_base::binary);
    unsigned char sym;
    // output file
    char *output=new char;
    strcat(output, filename);
    strcat(output, "-adaptivehuffman");
    BIT_FILE *output_file;
    output_file = OpenOutputBitFile(output);
    
    while(infile.good()){
        
        sym= infile.get();
        
        // Fisrt appearance of char, create node, add to tree
        if(nodeList[sym].val==NULL){  // NO such symbol stored in exist nodeList
            afNode *temp=new afNode;
            
            if(NYA==NULL){// No tree existing
                NYA=new afNode;
                root=new afNode;
                //Build tree, put NYA to left leaf and new symbol to right
                root->identifier=512;
                root->weight=1; // weight=1 because, it's the sum of first node and NYA node; 1+0=1
                root->left=NYA;
                root->right=temp;

                
                NYA->parent=root;
                NYA->identifier=root->identifier-2; //left leaf of root, identifier number is root's - 2;
                
                
                
                temp->val=sym;
                temp->weight=1;  //Initial weight is one.
                temp->identifier=root->identifier-1; //right leaf of root, identifier is root's -1;
                temp->parent=root;
                nodeList[sym]=*temp;
                
                nodeTree[NYA->identifier]=*NYA;
                nodeTree[temp->identifier]=*temp;

                
               // OutputBits(output_file, sym, 8);
                //create tree, add node
                
            }else{
                int codeLength=0;
                
                int code=getCode(NYA, 0, 0, &codeLength);
                // output
                OutputBits(output_file, code, codeLength);
                
                codeLength=0;
                OutputBits(output_file, sym, 8);
                
                NYA=getNewNYA(NYA, nodeList, nodeTree, sym);
                
                // NYA already updated, so use NYA->parent->parent
                updateTree(NYA->parent->parent, nodeTree);
                
                
                //create node of char, add to tree
            }
            
            
        }else{ //  not first time appearance, update tree
            
            int codeLength=0;
            int code=getCode(&nodeList[sym], 0, 0, &codeLength);
            OutputBits(output_file,code, codeLength);
            codeLength=0;
            updateTree(&nodeList[sym], nodeTree);
            
        }
        
    }
    
    
}