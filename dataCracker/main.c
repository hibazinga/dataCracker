//
//  main.c
//  dataCracker
//
//  Created by Yanbin Ren on 2/24/15.
//  Copyright (c) 2015 Data Crackers. All rights reserved.
//

#include <stdio.h>
#include "matrix.h"
#include <assert.h>

#define KNORM 10    // matrix norm of K calculated by matlab
#define SNORM 10    //            --  S --
#define F 128       // # of latent factors 64 or 128
#define N 1000      // # of iterations
#define alpha 0.02  // learning rate
#define lambda 0.01 // normalization factor

#define Keyword_M "Keyword_matrix"
#define Social_M "Social_matrix"
#define R_M "Rui_matrix"


int main(int argc, const char * argv[])
{
    //LFM Model
    
    int i=0;
    FILE *fp;
    
    
    // R
    int Rrow=get_row(R_M);
    fp=fopen(R_M, "r");
    int Rcol=get_column(fp);
    double **R = (double **)malloc(sizeof(double)*Rrow);
    for (i=0; i<Rrow; i++) {
        R[i]=(double *)malloc(sizeof(double)*Rcol);
    }
    read_matrix_from_file(R, Rrow, Rcol, fp);
    fclose(fp);
    
    
    // K
    int Krow=get_row(Keyword_M);
    fp=fopen(Keyword_M, "r");
    int Kcol=get_column(fp);
    double **K = (double **)malloc(sizeof(double)*Krow);
    for (i=0; i<Krow; i++) {
        K[i]=(double *)malloc(sizeof(double)*Kcol);
    }
    read_matrix_from_file(K, Krow, Kcol, fp);
    matrix_times(1/KNORM, K, K, Krow, Kcol);
    fclose(fp);
    
    // S
    int Srow=get_row(Social_M);
    fp=fopen(Social_M, "r");
    int Scol=get_column(fp);
    double **S = (double **)malloc(sizeof(double)*Srow);
    for (i=0; i<Srow; i++) {
        S[i]=(double *)malloc(sizeof(double)*Scol);
    }
    read_matrix_from_file(S, Srow, Scol, fp);
    matrix_times(1/SNORM, S, S, Srow, Scol);
    fclose(fp);
    
    // B
    
    int Brow=Rrow;
    int Bcol=Rcol;
    double **B = (double **)malloc(sizeof(double)*Brow);
    for (i=0; i<Brow; i++) {
        B[i]=(double *)malloc(sizeof(double)*Bcol);
    }
    random_initialize(B, Brow, Bcol);
    
    // P1
    
    int P1row=Kcol;
    int P1col=F;
    double **P1 = (double **)malloc(sizeof(double)*P1row);
    for (i=0; i<P1row; i++) {
        P1[i]=(double *)malloc(sizeof(double)*P1col);
    }
    random_initialize(P1, P1row, P1col);
    
    // P2
    
    int P2row=Scol;
    int P2col=F;
    double **P2 = (double **)malloc(sizeof(double)*P2row);
    for (i=0; i<P2row; i++) {
        P2[i]=(double *)malloc(sizeof(double)*P2col);
    }
    random_initialize(P2, P2row, P2col);
    
    // Q
    
    int Qrow=F;
    int Qcol=Rcol;
    double **Q = (double **)malloc(sizeof(double)*Qrow);
    for (i=0; i<Qrow; i++) {
        Q[i]=(double *)malloc(sizeof(double)*Qcol);
    }
    random_initialize(Q, Qrow, Qcol);
    
    // Calculate M = B+PQ
    //int j;
    //1. M1=K*P1/|K|
    int M1row=Krow;
    int M1col=F;
    double **M1 = (double **)malloc(sizeof(double)*M1row);
    for (i=0; i<M1row; i++) {
        M1[i]=(double *)malloc(sizeof(double)*M1col);
    }
    matrix_multiply(K, P1, M1, Krow, F);
    
    
    //2. M2=S*P2/|S|
    int M2row=Srow;
    int M2col=F;
    double **M2 = (double **)malloc(sizeof(double)*M2row);
    for (i=0; i<M2row; i++) {
        M2[i]=(double *)malloc(sizeof(double)*M2col);
    }
    matrix_multiply(S, P2, M2, Srow, F);
    
    
    //3. M1=M1+M2
    matrix_add(M1, M2, M1, M1row, M1col);
    
    //4. M = M1*Q
    int Mrow = Rrow;
    int Mcol = Rcol;
    double **M = (double **)malloc(sizeof(double)*Mrow);
    for (i=0; i<Mrow; i++) {
        M[i]=(double *)malloc(sizeof(double)*Mcol);
    }
    matrix_multiply(M1, Q, M, M1row, Qcol);
    
    //5. M = M+B
    matrix_add(M, B, M, Mrow, Mcol);
    
    
    
    // assertion
    
    assert(Rrow==Krow);
    assert(Rrow==Srow);
    
    
    
    // tmp matrix used for gradient descent calculation

    
    //1. tmp0 = M*QT
    double **tmp0 =(double **)malloc(sizeof(double)*Mrow);
    for (i=0; i<Mrow; i++) {
        tmp0[i]=(double *)malloc(sizeof(double)*Qrow);
    }
    
    //2. tmp1 = KT * (M * QT) = P1
    double **tmp1 = (double **)malloc(sizeof(double)*P1row);
    for (i=0; i<P1row; i++) {
        tmp1[i]=(double *)malloc(sizeof(double)*P1col);
    }

    
    //3. tmp2 = ST * (M * QT) = P2
    double **tmp2 = (double **)malloc(sizeof(double)*P2row);
    for (i=0; i<P2row; i++) {
        tmp2[i]=(double *)malloc(sizeof(double)*P2col);
    }
    
    
    //4. tmp3 = K* P1
    double **tmp3 = (double **)malloc(sizeof(double)*Krow);
    for (i=0; i<Krow; i++) {
        tmp3[i]=(double *)malloc(sizeof(double)*P1col);
    }
    
    
    //5. tmp4 = S* P2
    double **tmp4 = (double **)malloc(sizeof(double)*Srow);
    for (i=0; i<Srow; i++) {
        tmp4[i]=(double *)malloc(sizeof(double)*P2col);
    }

    //6. (tmp3+tmp4)T * M   --- F * Mcol
    
    double **tmp5 = (double **)malloc(sizeof(double)*F);
    for (i=0; i<F; i++) {
        tmp5[i]=(double *)malloc(sizeof(double)*Mcol);
    }
    
    
    
    
    // Main Process
    int step=0;
    int alpha_p=alpha;
    for (step=0;step<N;step++){
        
        //gradient descent
        
        // M = R-M
        matrix_substract(R, M, M, Rrow, Rcol);
        
        // M = alpha * M
        matrix_times(alpha_p, M, M, Mrow, Mcol);
        
        //1. tmp0 = M*QT
        matrix_multiply1(M, Q, tmp0, Mrow, Qrow);
        
        // P1
        matrix_times(1-alpha_p*lambda, P1, P1, P1row, P1col);
        
        matrix_multiply2(K, M, tmp1, Kcol, Mcol);
        
        matrix_add(P1, tmp1, P1, P1row, P1col);
        
        // P2
        matrix_times(1-alpha_p*lambda, P2, P2, P2row, P2col);
        
        matrix_multiply2(S, M, tmp2, Scol, Mcol);
        
        matrix_add(P2, tmp2, P2, P2row, P2col);
        
        
        // Q
        matrix_times(1-alpha_p*lambda, Q, Q, Qrow, Qcol);
        
        matrix_multiply(K, P1, tmp3, Krow, P1col);
        
        matrix_multiply(S, P2, tmp4, Srow, P2col);
        
        matrix_add(tmp3, tmp4, tmp3, Krow, P1col);
        
        matrix_multiply2(tmp3, M, tmp5, P1col, Mcol);
        
        matrix_add(Q, tmp5, Q, Qrow, Qcol);
        
        
        // B
        matrix_times(1-alpha_p*lambda, B, B, Brow, Bcol);
        matrix_add(M, B, B, Brow, Bcol);
        
        
        // M:
        //1. M1=K*P1/|K|
        matrix_multiply(K, P1, M1, Krow, F);
        
        //2. M2=S*P2/|S|
        matrix_multiply(S, P2, M2, Srow, F);
        
        //3. M1=M1+M2
        matrix_add(M1, M2, M1, M1row, M1col);
        
        //4. M = M1*Q
        matrix_multiply(M1, Q, M, M1row, Qcol);
        
        //5. M = M+B
        matrix_add(M, B, M, Mrow, Mcol);
        
        alpha_p*=0.9;
    }
    
    // Generate Model & write to file: model.csv
    
    // write to file
    write_matrix_to_file(M, Mrow, Mcol);
    
    return 0;
}
