//
//  MTF.cpp
//  Lossless
//
//  Created by Yuan Liu on 2/2/15.
//  Copyright (c) 2015 Yuan Liu. All rights reserved.
//

#include "MTF.h"
#include "bitio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void MTF(char *filename){
    
    int bit_count;
    int table[256]={};
    
    BIT_FILE *output_file;
    char *output=new char[64];
    strcat(output, filename);
    strcat(output, "-MTF.out");
    output_file=OpenOutputBitFile(output);
    
    ifstream infile(filename, ios_base::in | ios_base::binary);
    unsigned char current;

    for (int i=0;i<256;i++){
        table[i]=i;
    }
    
    // from second symbol to EOF
    while(infile.good()){
        current=infile.get();
        int i;
        // Find the target
        for (i=0;i<256;i++){
            if(table[i]==current){
                OutputBits(output_file, i, 8);
                bit_count+=8;
              //  break;
            }
        }
        
        // Move the table back by 1, to make room for MTF element
        while(i-=1){
            table[i+1]=table[i];
        }
        
        // Move to front;
        table[0]=current;
        
    }
    
    
    infile.close();
    CloseOutputBitFile(output_file);
    
    cout<<"MTF encoding processed size: "<<bit_count<<" bits"<<endl;
    
}

void MTFD(char *filename){
    unsigned char current;
    int table[256]={};

    
    BIT_FILE *output_file;
    char *output=new char[64];
    strcat(output, filename);
    strcat(output, "-recov.out");
    output_file=OpenOutputBitFile(output);
    
    ifstream infile(filename, ios_base::in | ios_base::binary);
  
    
    for (int i=0;i<256;i++){
        table[i]=i;
    }
    
    int i;

    while(infile.good()){
        i=infile.get();
        
        cout<<i<<endl;
        
        OutputBits(output_file, table[i], 8);
        current=table[i];
        
        while(i--&&i>=0){
            table[i+1]=table[i];
        }
        
        table[0]=current;
    }
    
    infile.close();
    CloseOutputBitFile(output_file);
    
}




