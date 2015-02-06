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
    
    // First symbol
    current=infile.get();

    // Initial table
    for (int index=0;index<256;index++){
        table[index]=index;
    }

    int idx;
    
    //
    while(infile.good()){
        

       
        // Find the symbol index in table
        for(idx=0;idx<256;idx++){
            if(table[idx]==static_cast<int>(current)){
                OutputBits(output_file, (unsigned int)idx, 8);
               // cout<<" "<<table[idx]<<current;
                bit_count+=8;
                break;
            }
        }
        

        // Move the table back by 1, to make room for MTF element
        for(;idx!=0;--idx){
            table[idx]=table[idx-1];
        }
        
        // Move to front;
      
        table[0]=current;
        // get new symbol;
        current=infile.get();
        
    }
    

    //cout<<endl;
    infile.close();
    CloseOutputBitFile(output_file);
    
    
    cout<<"MTF transformation processed size: "<<bit_count<<" bits"<<endl;
    
}

void MTFD(char *filename){
    unsigned char current;
    unsigned char table[256]={};
    int bit_count=0;
    
    BIT_FILE *output_file;
    char *output=new char[64];
    strcat(output, filename);
    strcat(output, "-recov.out");
    output_file=OpenOutputBitFile(output);
  
    // Initialize table
    for (int i=0;i<256;i++){
        table[i]=i;
    }
    
    ifstream infile(filename, ios_base::in | ios_base::binary);
  
    
    int input;
    // First code
    input=infile.get();
    
    int idx;
    while(infile.good()){
        
       // cout<<i<<endl;
        current=table[input];
        OutputBits(output_file, current, 8);
        bit_count+=8;
        
        idx=input;
        for(;idx!=0;--idx){
            table[idx]=table[idx-1];
        }
       // cout<<input<<"is "<<current<<endl;
 
        table[0]=current;
        // Read new symbol;
        input=infile.get();
        //cout<<input;
    }
    
    cout<<endl;
    cout<<endl;
    cout<<"MTF recovered size: "<<bit_count<<" bits"<<endl;
    
    
    infile.close();
    CloseOutputBitFile(output_file);
    
}




