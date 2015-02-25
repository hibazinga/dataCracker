//
//  matrix.h
//  dataCracker
//
//  Created by Yanbin Ren on 2/24/15.
//  Copyright (c) 2015 Yanbin Ren. All rights reserved.
//

#ifndef dataCracker_matrix_h
#define dataCracker_matrix_h

#endif


#include <stdio.h>
#include <stdlib.h>

// M*K  *  K*N  =   M*N
void matrix_mutiply(double** A, double** B, double** ans,int m,int k,int n){
    int i1,i2;
    for (i1=0; i1<m; i1++) {
        for (i2=0; i2<n; i2++) {
            int tmp=0;
            int j1;
            for (j1=0; j1<n; j1++) {
                tmp+=A[i1][j1]*B[j1][i2];
            }
            ans[i1][i2]=tmp;
        }
    }
}
// A[i][j],   A[i][j+1],   ...,
// A[i+1][j], A[i+1][j+1], ...,
void write_matrix_to_file(double **A,int m,int n){
    FILE *fp;
    fp=fopen("model.csv", "w");
    int i,j;
    for (i=0; i<m; ++i) {
        for (j=0; j<n; ++j) {
            fprintf(fp, "%lf", A[i][j]);
            fputc(',',fp);
        }
        fputc('\n', fp);
    }
}