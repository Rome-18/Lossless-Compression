//
//  adaptiveHuff.cpp
//  Lossless
//
//  Created by Yuan Liu on 1/31/15.
//  Copyright (c) 2015 Yuan Liu. All rights reserved.
//

#include "adaptiveHuff.h"
#include "bitio.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <fstream>


using namespace std;

void adaptiveCompr(char *filename){
    int IDindex=511;
    int code;
    int codeLength;
    int bit_count=0;
    
    AHNode *nodeList[256]={};
    AHNode *nodeTree[512]={};
    AHNode *root;
    AHNode *NYA=NULL;
    AHNode *temp;
    
    //initial root;
    
    root=new AHNode;
    root->ID=IDindex;
    // add root to tree;
    nodeTree[root->ID]=root;
    

    
    BIT_FILE *output_file;
    char *output=new char[64];
    strcat(output, filename);
    strcat(output, "-adaptive-huffman.out");
    
    output_file=OpenOutputBitFile(output);
    
    ifstream infile(filename, ios_base::in | ios_base::binary);
    unsigned char sym;
    
    
    ///////////////////////////
    //  Process the first symbol as a special case,
    //  Since no tree update is involved in this stage, just initialize the tree and add the root node, with NYA
    ///////////////////////////
    // Read in first symbol of the input file
    sym=infile.get();
    temp=new AHNode;
    
    
    // Initialize the NYA node, add NYA and new node to the tree;
    NYA=new AHNode;
    
    // Link three nodes: root, NYA and new node
    root->left=NYA;
    root->right=temp;
    
    temp->parent=root;
    temp->ID=root->ID-1;
    temp->val=sym;
    temp->weight=1;
    nodeList[sym]=temp;
    
    NYA->parent=root;
    NYA->ID=root->ID-2;
    
    // add to the tree structure
    nodeTree[NYA->ID]=NYA;
    nodeTree[temp->ID]=temp;

    root->weight=1;
    OutputBit(output_file, 0);
    OutputBits(output_file, sym, 8);
    bit_count+=9;
    
    
    ///////////////////////////
    //  From the second symbol to the EOF
    //  Need to update the tree every time a symbols comes, no matter new or old, do inset node for new symbol
    ///////////////////////////
    while(infile.good()){
        sym=infile.get();
        
        // Initial new node;
        AHNode *symbol=new AHNode;
        symbol->val=sym;
        symbol->weight=1;
        symbol->ID=-1;
        symbol->left=NULL;
        symbol->right=NULL;
        symbol->parent=NULL;
        
        
        // new symbol,
        if(nodeList[sym]==NULL){
                codeLength=0;
                code=getCode2(NYA,0,0,&codeLength);
                OutputBits(output_file, code, codeLength);
                bit_count+=codeLength;
                codeLength=0;
                // Use the symbol's representation as its unique codeword, so, output code is sym itself here
                OutputBits(output_file, sym, 8);
                bit_count+=codeLength;
            
                // Create new NYA node and node for new symbol, add them to the tree.
                //NYA=addnewNode(NYA, nodeList, nodeTree, sym);
                NYA=addnewNode(NYA, nodeList, nodeTree, symbol);
                // Put the new added branch to proper place on the tree
                // This update is a special case, new node has weight of 1, to be added to the old NYA's right child
                // The nearest existing node with weight change is new node's parent
                // It's weight is increase by 1
                // using Node->parent instead of Node as input will save one resursion on the update
                updateTree(nodeList[sym]->parent, nodeTree);
            
        }else{// old symbol comes
            codeLength=0;
            code=getCode2(nodeList[sym],0,0,&codeLength);
            OutputBits(output_file, code, codeLength);
            bit_count+=codeLength;
            codeLength=0;
            updateTree(nodeList[sym], nodeTree);
            
        }
    }
    
    cout<<"Adaptive Huffman compressed size: "<<bit_count<<" bits"<<endl;
    infile.close();
    CloseOutputBitFile(output_file);
    
    
}




int getCode2(AHNode *temp, int code, int bit, int *length){
    //cout<<"  "<<*length<<"-"<<length;
    if(temp->parent==NULL){// this node is root node,
        
        return code;
        
    }else{
         *length+=1;
        
        if(temp==temp->parent->left){// this node is left node
            
                code = code << ((*length)-1); //shift bits
                code += bit;// add new bit to current number
                return getCode2(temp->parent, code, 0, length);
        }else{
                code = code << ((*length)-1);
                code += bit;
                return getCode2(temp->parent, code, 1, length);
        }
    }
}



//AHNode *addnewNode(AHNode *oldNYA, AHNode *nodeList[], AHNode *nodeTree[], unsigned char sym){
AHNode *addnewNode(AHNode *oldNYA, AHNode *nodeList[], AHNode *nodeTree[], AHNode *temp){
    
    // initialize a new NYA node
    AHNode *newNYA=new AHNode;
    newNYA->val=-1;
    newNYA->weight=0;
    newNYA->ID=-1;
    newNYA->left=NULL;
    newNYA->right=NULL;
    newNYA->parent=NULL;
    
    
    // Assign ID number to new nodes, and link it to the oldNYA, as it is the parent of new node now
    // The new node should be always the right child of old NYA, so link to the right
    temp->ID=oldNYA->ID-1;
    temp->parent=oldNYA;
    oldNYA->right=temp;
    
    // Link new NYA to old NYA, new NYA should always be the left child of old NYA
    newNYA->ID=oldNYA->ID-2;
    newNYA->parent=oldNYA;
    oldNYA->left=newNYA;

    
    // Old NYA now is a pseudo node with weight = 1
    oldNYA->weight=1;
    
    
    // Add new node to the nodelist, so that the next time it appears, its old symbol
    nodeList[temp->val]=temp;
    
    nodeTree[temp->ID]=temp;
    nodeTree[newNYA->ID]=newNYA;
    
    return newNYA;
}


// Traverse the tree nodes with ID larger than the new node,
// Find the one with same weight and largest ID to swap
AHNode *findTarget( AHNode *temp, AHNode *nodeTree[]){
    AHNode *target=NULL;
    for (int i=temp->ID+1;i<511;i++){
        if(nodeTree[i]->weight==temp->weight){
            target=nodeTree[i];
        }
    }
    return target;
}

// Recursive function to update the tree, it update weights, swap nodes
void updateTree(AHNode *temp, AHNode *nodeTree[]){
    AHNode *target;
    
    if(temp->parent==NULL){
        temp->weight++;
    }else{
        target=findTarget(temp, nodeTree);
        //Condition#1: if no target found, no need to swap node
        //Condition#2: if target is the node's parent, no need to swap the node
        //             since the parent is a pseudo node made for linking the tree structure
        if(target!=NULL&& target!=temp->parent){
            swapNode(temp, target, nodeTree);
        }
        temp->weight++;
        updateTree(temp->parent, nodeTree);
    }
}

void swapNode(AHNode *temp, AHNode *target, AHNode *nodeTree[]){
    // point to the node's parent, so that we can swap nodes
    AHNode *tempParent;
    AHNode *targetParent;
    
    tempParent=temp->parent;
    targetParent=target->parent;
    
    
    // Link temp to target position
    // Need to determine the node is left or right child of its parent
    temp->parent=targetParent;
    if(target==targetParent->left){
        targetParent->left=temp;
    }else{
        targetParent->right=temp;
    }
    
    // Link target to the temp position
    // Need to determine the left or right as well
    target->parent=tempParent;
    if(temp==tempParent->left) {
        tempParent->left=target;
    }else{
        tempParent->right=target;
    }
    
    // update in tree
    nodeTree[temp->ID]=target;
    nodeTree[target->ID]=temp;
    
    
    // update ID information
    int swap=target->ID;
    target->ID=temp->ID;
    temp->ID=swap;

    
}

