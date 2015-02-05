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
    
    int bit_count=0;
    int table[256]={};
    
    BIT_FILE *output_file;
    char *output=new char[64];
    strcat(output, filename);
    strcat(output, "-MTF.out");
    output_file=OpenOutputBitFile(output);
    
    ifstream infile(filename, ios_base::in | ios_base::binary);
    unsigned char current;

    // Initial table
    for (int index=0;index<256;index++){
        table[index]=index;
    }
   // cout<<endl;
    int idx;
    
    //
    while(infile.good()){
        
        current=infile.get();
        if(current==255){
            break;
        }

//        // Move the table back by 1, to make room for MTF element
//
        for(idx=0;idx<256;idx++){
            if(table[idx]==static_cast<int>(current)){
                OutputBits(output_file, (unsigned long)idx, 8);
                cout<<" "<<table[idx]<<current;
                bit_count+=8;
                break;
            }
        }
        
        
        for(;idx!=0;--idx){
            table[idx]=table[idx-1];
        }
        
        // Move to front;
      
        table[0]=current;
  
        
    }

    cout<<endl;
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
    int idx;
    while(infile.good()){
        i=infile.get();
        
        if(i<0){
            break;
        }
        
       // cout<<i<<endl;
        
        OutputBits(output_file, table[i], 8);
        current=table[i];
        idx=i;
       // cout<<idx<<endl;
        for(;idx!=0;--idx){
            table[idx]=table[idx-1];
        }

        
        table[0]=current;
    }
    
    infile.close();
    CloseOutputBitFile(output_file);
    
}




