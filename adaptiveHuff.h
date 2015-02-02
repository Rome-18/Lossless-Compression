//
//  adaptiveHuff.h
//  Lossless
//
//  Created by Yuan Liu on 1/31/15.
//  Copyright (c) 2015 Yuan Liu. All rights reserved.
//

#ifndef __Lossless__adaptiveHuff__
#define __Lossless__adaptiveHuff__

#include <stdio.h>


typedef struct ahnode{
    int val=-1;
    int weight=0;
    int ID=-1;
    ahnode *left=NULL;
    ahnode *right=NULL;
    ahnode *parent=NULL;
    
}AHNode;


void adaptiveCompr(char *filename);
int getCode(AHNode *temp, int code, int *codelength);
int getCode2(AHNode *temp, int code, int bit, int *length);
AHNode *addnewNode(AHNode *oldNYA, AHNode *nodeList[], AHNode *nodeTree[], unsigned char sym);
AHNode *findTarget( AHNode *temp, AHNode *nodeTree[]);
void updateTree(AHNode *temp, AHNode *nodeTree[]);



#endif /* defined(__Lossless__adaptiveHuff__) */
