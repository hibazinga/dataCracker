//
//  main.c
//  dataCracker
//
//  Created by Yanbin Ren on 2/24/15.
//  Copyright (c) 2015 Data Crackers. All rights reserved.
//

#include <stdio.h>
#include "matrix.h"


#define KNORM 10    // matrix norm of K calculated by matlab
#define SNORM 10    //            --  S --
#define F 128       // # of latent factors 64 or 128
#define N 1000      // # of iterations
#define alpha 0.02  // learning rate
#define lambda 0.01 // normalization factor


int main(int argc, const char * argv[])
{
    /* test for read_next_token();
     
    char *str="123\t456\t678";
    int i;
    int index=0;
    for(i=0;i<3;i++){
        char *p=read_next_token(str,index);
        index+=(1+strlen(p));
        printf("%s\n",p);
    }*/
    
    //LFM Model
    
    // K
    //printf("hello\n");
    int row = get_row("backup.c");
    printf("world\n");
    printf("%d",row);
    return 0;
}
