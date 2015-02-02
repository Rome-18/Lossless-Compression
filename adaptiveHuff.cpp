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
#include <queue>
#include <string.h>
#include <vector>

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
    strcat(output, "-adaptive-huffman");
    output_file=OpenOutputBitFile(output);
    
    ifstream infile(filename, ios_base::in | ios_base::binary);
    unsigned char sym;
    
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
    
    
    
    
    
    while(infile.good()){
        sym=infile.get();
        
        // new symbol,
        if(nodeList[sym]==NULL){
//            temp=new AHNode;
//            temp->val=-1;
//            temp->weight=0;
//            temp->ID=-1;
//            temp->left=NULL;
//            temp->right=NULL;
//            temp->parent=NULL;
            
            // first ever symbol, initial tree; add NYA;
//            if(NYA==NULL){
//                NYA=new AHNode;
//                
//                root->left=NYA;
//                root->right=temp;
//                
//                
//                temp->parent=root;
//                temp->ID=root->ID-1;
//                temp->val=sym;
//                temp->weight=1;
//                nodeList[sym]=temp;
//                
//                NYA->parent=root;
//                NYA->ID=root->ID-2;
//                
//                nodeTree[NYA->ID]=NYA;
//                nodeTree[temp->ID]=temp;
//                
//                root->weight=1;
//                OutputBit(output_file, 0);
//                OutputBits(output_file, sym, 8);
//                bit_count+=9;
//            }else{
                codeLength=0;
               // code=getCode(NYA,0,&codeLength);
                code=getCode2(NYA,0,0,&codeLength);
                OutputBits(output_file, code, codeLength);
                bit_count+=codeLength;
                codeLength=0;
                OutputBits(output_file, sym, 8);
                bit_count+=codeLength;
                NYA=addnewNode(NYA, nodeList, nodeTree, sym);
                updateTree(NYA->parent->parent, nodeTree);
//            }
        }else{// old symbol comes
            codeLength=0;
            //code=getCode(nodeList[sym], 0, &codeLength);
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



// A bottom-up mechanism to get codeword
int getCode(AHNode *temp, int code, int *codelength){
    if (temp->parent==NULL){ // node is root node
        return code;
    }else{
        
        if (temp==temp->parent->left){
            code=code+(0<<(*codelength));
        }else{
            code=code+(1<<(*codelength));
        }
        ++(*codelength);
        return getCode(temp->parent, code, codelength);
    }
    
}


int getCode2(AHNode *temp, int code, int bit, int *length){
    //cout<<"  "<<*length<<"-"<<length;
    if(temp->parent==NULL){// this node is root node,
        
        return code;
        
    }else{
         *length+=1;
        
        if(temp==temp->parent->left){// this node is left node
            //code = code + (0<<(*length));
                        code = code << ((*length)-1);
                        code += bit;
            //
                        return getCode2(temp->parent, code, 0, length);
        }else{
            //code = code + (1<<(*length));
                        code = code << ((*length)-1);
                        code += bit;
            //
                        return getCode2(temp->parent, code, 1, length);
        }
        //++(*length);
        //return getCode(temp->parent, code, 0, length);
    }
}



AHNode *addnewNode(AHNode *oldNYA, AHNode *nodeList[], AHNode *nodeTree[], unsigned char sym){
    AHNode *temp=new AHNode;
    temp->val=-1;
    temp->weight=0;
    temp->ID=-1;
    temp->left=NULL;
    temp->right=NULL;
    temp->parent=NULL;
    
    AHNode *newNYA=new AHNode;
    newNYA->val=-1;
    newNYA->weight=0;
    newNYA->ID=-1;
    newNYA->left=NULL;
    newNYA->right=NULL;
    newNYA->parent=NULL;
    
    temp->ID=oldNYA->ID-1;
    newNYA->ID=oldNYA->ID-2;
    
    temp->weight=1;
    temp->parent=oldNYA;
    temp->val=sym;
    
    newNYA->parent=oldNYA;
    
    oldNYA->left=newNYA;
    oldNYA->right=temp;
    oldNYA->weight=1;
    
    nodeList[sym]=temp;
    nodeTree[temp->ID]=temp;
    nodeTree[newNYA->ID]=newNYA;
    
    return newNYA;
}


AHNode *findTarget( AHNode *temp, AHNode *nodeTree[]){
    AHNode *target=NULL;
    for (int i=temp->ID+1;i<511;i++){
        if(nodeTree[i]->weight==temp->weight){
            target=nodeTree[i];
        }
    }
    return target;
}


void updateTree(AHNode *temp, AHNode *nodeTree[]){
    AHNode *target;
    AHNode *tempParent;
    AHNode *targetParent;
    
    if(temp->parent==NULL){
        temp->weight++;
    }else{
        target=findTarget(temp, nodeTree);
        if(target!=NULL&& target!=temp->parent&& target->parent!=NULL){
            tempParent=temp->parent;
            targetParent=target->parent;
            
            
            // Link temp to target position
            temp->parent=targetParent;
            if(target==targetParent->left){
                targetParent->left=temp;
            }else{
                targetParent->right=temp;
            }
  
            // Link target to the temp position
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
        temp->weight++;
        updateTree(temp->parent, nodeTree);
    }
}

