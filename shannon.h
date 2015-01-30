//
//  shannon.h
//  Lossless
//
//  Created by Yuan Liu on 1/27/15.
//  Copyright (c) 2015 Yuan Liu. All rights reserved.
//

#ifndef __Lossless__shannon__
#define __Lossless__shannon__

#include <stdio.h>
#include "Huffman.h"

typedef struct shannontable{
    int code;
    int codeLength;
}ShannonTable;

void shannon_routine(char *filename);
void makeShannonArray(int *frequency, int nodeNum, Node *array);
Node* shannonTree(Node *array, int pos1, int pos2);
void shannonCode(ShannonTable *table, Node *root, int code, int bit, int length);
void doCompression(char* filename, ShannonTable *table);
#endif /* defined(__Lossless__shannon__) */
