//
//  Huffman.h
//  Lossless
//
//  Created by Yuan Liu on 1/26/15.
//  Copyright (c) 2015 Yuan Liu. All rights reserved.
//

#ifndef __Lossless__Huffman__
#define __Lossless__Huffman__

#include <stdio.h>

typedef struct node{
    int val;
    int weight;
    struct node *left, *right;
}Node;

typedef struct huffmantable{
    int code;
    int codeLength;
}HuffmanTable;

void huffman_routine(char *filename);
void char_count(char *input,int *frequency);
Node* build_tree(int nodeNum, int *frequency);
void HuffCode(HuffmanTable table[], node *root, int code, int bit, int length);
void doCompression(char* filename, HuffmanTable *table);



#endif /* defined(__Lossless__Huffman__) */


