//
//  main.cpp
//  Lossless
//
//  Created by Yuan Liu on 1/26/15.
//  Copyright (c) 2015 Yuan Liu. All rights reserved.
//

#include <iostream>
#include <string>
#include "Huffman.h"
#include "shannon.h"
#include "runLength.h"
#include "adaptiveHuff.h"
#include "MTF.h"
#include "statistics.h"


using namespace std;


void runAlgo(bool isEncode, int algo, char* filename);
void help();

int main(int argc, char *argv[]) {
    // insert code here...
    string default_file="text.dat";
    
    char *filename;
    
    bool isEncoding=true;
    int algorithm=0;
    
    if(argv[1]!=NULL&&argv[2]==NULL){
        filename=argv[1];
        getInfo(filename);
        help();
        return 1;
    }
    
    //print help information and quit if no parameters, print statistics if only filename presents
    if(argv[1]==NULL||argv[2]==NULL||argv[3]==NULL){
       // filename=argv[1];
      //  getInfo(filename);
        adaptiveCompr("image.dat.raw");
        help();
        return 1;
    }
    //Function to calculate sumbol frequencies and entropy
    
    
    
    
    if(strcmp(argv[1],"encode")==0){
        isEncoding=true;
    }else if(strcmp(argv[1],"decode")==0){
        isEncoding=false;
    }else{
        cout<<"ERROR: Mode parameter is wrong! should be [encode/decode]"<<endl;
        help();
        return 1;
    }
    
    
    if(strcmp(argv[2],"sf")==0){
        algorithm=0;
    }else if(strcmp(argv[2],"hm")==0){
        algorithm=1;
    }else if(strcmp(argv[2],"ah")==0){
        algorithm=2;
    }else if(strcmp(argv[2],"rl")==0){
        algorithm=3;
    }else if(strcmp(argv[2],"mrl")==0){
        algorithm=4;
    }else if(strcmp(argv[2],"mtf")==0){
        algorithm=5;
    }else{
        cout<<"ERROR: Algorithm parameter is wrong! [sf/hm/ah]"<<endl;
        help();
        return 1;
    }
    
    
    if(argv[3]!=NULL){
        filename=argv[3];
    }else{
        cout<<"WARNING: filename missing!"<<endl;
        help();
        return 1;
    }
    
    
    runAlgo(isEncoding, algorithm, filename);
    
    
    return 0;
}


void runAlgo(bool isEncode, int algo, char* filename){
    if(isEncode){
        if(algo==0){
            
            shannon_routine(filename);
        }else if(algo==1){
            huffman_routine(filename);
        }else if(algo==2){
            adaptiveCompr(filename);
        }else if(algo==3){
            basicRLC(filename);
        }else if(algo==4){
            modifiedRLC(filename);
        }else if(algo==5){
            MTF(filename);
        }
    }else{
        if(algo==3){
            basicRLDC(filename);
        }else if(algo==4){
            modifiedRLDC(filename);
        }else if(algo==5){
            MTFD(filename);
        }
    }
}

void help(){
    cout<<"For data statistics: frequencies, entropy"<<endl;
    cout<<"./lossless [filename]"<<endl;
    cout<<"For Shannon-Fano, Huffman and Adaptive Huffman encoding, follow the format below"<<endl;
    cout<<"./lossless encode [sf/hm/ah] [filename]"<<endl;
    cout<<"For run length encoding and decoding, follow the format below"<<endl;
    cout<<"./lossless [encode/decode] [rl/mrl/mtf] [filename]"<<endl;
}
