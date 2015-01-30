//
//  adaptivehuffman.h
//  Lossless
//
//  Created by Yuan Liu on 1/28/15.
//  Copyright (c) 2015 Yuan Liu. All rights reserved.
//

#ifndef __Lossless__adaptivehuffman__
#define __Lossless__adaptivehuffman__

#include <stdio.h>


typedef struct afnode{
    int val=0;
    int weight=0;
    //Unique identifier to help track the sibling property
    int identifier=0;
    struct afnode *left=NULL, *right=NULL, *parent=NULL;
}afNode;

void adapHuffman_routine(char *filename);
afNode *getNewNYA(afNode *old, afNode *table, afNode *total, unsigned char sym);
int getCode(afNode *temp, int code, int bit, int *length);
afNode *maxNode(afNode *temp, afNode *total);
void updateTree(afNode *temp, afNode *total);
void doCompression(char * filename);

#endif /* defined(__Lossless__adaptivehuffman__) */
