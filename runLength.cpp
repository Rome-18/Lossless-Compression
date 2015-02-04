//
//  runLength.cpp
//  Lossless
//
//  Created by Yuan Liu on 1/30/15.
//  Copyright (c) 2015 Yuan Liu. All rights reserved.
//

#include "runLength.h"
#include "bitio.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void basicRLC(char *filename){
    int counter=0;
 //   int size=0;
    int bit_count;
    unsigned char previous;
    
    
    BIT_FILE *output_file;
    char *output=new char[64];
    strcat(output, filename);
    strcat(output, "-RL.out");
    output_file=OpenOutputBitFile(output);
    
    ifstream infile(filename, ios_base::in | ios_base::binary);
    unsigned char sym;
    
    // first symbol
    sym=infile.get();
    previous=sym;
    counter++;
    
    // from second symbol to EOF
    while(infile.good()){
        sym=infile.get();
     //   size++;
        
        // if new symbol comes or counter is full
        if(sym!=previous|| counter==255){
            
            OutputBits(output_file, counter, 8);
            OutputBits(output_file, previous, 8);
            //cout<<(char)previous;
            bit_count+=16;
            counter=1;
            previous=sym;
        }else{
            counter++;
        }
    }
    
    
    infile.close();
    CloseOutputBitFile(output_file);
    
   // cout<<"Original: "<<size*8<<" bits"<<endl;
    cout<<"Run length compressed size: "<<bit_count<<" bits"<<endl;
    cout<<"Name of compressed file is: "<<output<<endl;
    
}

void basicRLDC(char* filename){
    unsigned char sym;
    int bit_count=0;
    int counter=0;
    bool isCounter=true;
    

    
    
    BIT_FILE *output_file;
    char *output=new char[64];
    strcat(output, filename);
    strcat(output, "-recov.out");
    output_file=OpenOutputBitFile(output);
    
    
    ifstream infile(filename, ios_base::in | ios_base::binary);

    
    while(infile.good()){
        
        sym=infile.get();
        
        if(isCounter){
            counter=sym;
            isCounter=false;
        }else{
            for (int i=0; i<counter;i++){
                OutputBits(output_file, sym, 8);
                //cout<<(char)sym;
                
                bit_count+=8;
            }
            isCounter=true;
        }
    }
    
    infile.close();
    CloseOutputBitFile(output_file);
    //cout<<endl;
    cout<<"RLDC recovered size: "<<bit_count<<" bits"<<endl;
    
}

void modifiedRLC(char* filename){
    int counter=0;
    int bit_count=0;
    
    // Prepare output file
    BIT_FILE *output_file;
    char *output=new char[64];
    strcat(output, filename);
    strcat(output, "-MRL.out");
    output_file=OpenOutputBitFile(output);
    
    // Prepare input file
    ifstream infile(filename, ios_base::in | ios_base::binary);
    unsigned char sym;
    unsigned char previous;
    
    // first symbol
    sym=infile.get();
    previous=sym;
    counter++;
    
    // from 2nd symbol to EOF
    while(infile.good()){
        sym=infile.get();
        if(sym!=previous||counter==127){
                if(counter==1){
                    if(previous>=128){
                        OutputBits(output_file, 128+counter, 8);
                        OutputBits(output_file, previous, 8);
                        bit_count+=16;
                    }else{
                        OutputBits(output_file, previous, 8);
                        bit_count+=8;
                    }
                }else{
                    OutputBits(output_file, 128+counter, 8);
                    OutputBits(output_file, previous, 8);
                    bit_count+=16;
                }
            counter=1;
            previous=sym;
        }
        else{
            counter++;
        }
    }
    
    infile.close();
    CloseOutputBitFile(output_file);
    
    cout<<"Run length compressed size: "<<bit_count<<" bits"<<endl;
    cout<<"Name of compressed file is: "<<output<<endl;
    
    
}


void modifiedRLDC(char *filename){
    unsigned char sym;
    int bit_count=0;
    int counter=0;
    bool isCounter=true;
    
    
    
    
    BIT_FILE *output_file;
    char *output=new char[64];
    strcat(output, filename);
    strcat(output, "-recov.out");
    output_file=OpenOutputBitFile(output);
    
    
    ifstream infile(filename, ios_base::in | ios_base::binary);
    
    
    while(infile.good()){
        sym=infile.get();
        
        // if symbol>127, it means the MSB is 1, otherwise, MSB=0
        if(sym>127){
            if(isCounter){
                counter=sym-128;
                isCounter=false;
            }else{
                for (int i=0;i<counter;i++){
                    OutputBits(output_file, sym, 8);
                    bit_count+=8;
                }
                isCounter=true;
            }
        }else{
            if(!isCounter){
                for(int i=0;i<counter;i++){
                    OutputBits(output_file, sym, 8);
                    bit_count+=8;
                }
                isCounter=true;
            }else{
                OutputBits(output_file, sym, 8);
                bit_count+=8;
            }
        }
        
    }
    
    infile.close();
    CloseOutputBitFile(output_file);
    cout<<"MRLDC recovered size: "<<bit_count<<" bits"<<endl;
}

