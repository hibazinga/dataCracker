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
#include <time.h>

#define KNORM 1    // matrix norm of K calculated by matlab
#define SNORM 1    //            --  S --
#define F 128       // # of latent factors 64 or 128
#define N 1         // # of iterations
#define alpha -0.01  // learning rate
#define lambda 0.01 // normalization factor

#define Keyword_M "/Users/ybren/Desktop/dataCracker/dataCracker/pre/user_key_word_13_ij.txt"
#define Social_M "/Users/ybren/Desktop/dataCracker/dataCracker/pre/S_13_nu_n.txt"
#define Keyword_MT "/Users/ybren/Desktop/dataCracker/dataCracker/pre/user_key_word_13_ji.txt"
#define Social_MT "/Users/ybren/Desktop/dataCracker/dataCracker/pre/S_13_n_nu.txt"
#define R_M "/Users/ybren/Desktop/dataCracker/dataCracker/pre/Rui_13_nm.txt"


/*#define Keyword_M "K.csv"
#define Social_M "S.csv"
#define Keyword_MT "KT.csv"
#define Social_MT "ST.csv"
#define R_M "R.csv"
*/
#define SPARSE 1

int main(int argc, const char * argv[])
{
    printf("1\n");
    //LFM Model
    if (SPARSE){
        
    int i=0;
    FILE *fp;
        int *RC = (int *)malloc(2*sizeof(int));
        //memset(RC, 0, sizeof(int)*2);
    
    // R
    //int Rrow=get_row(R_M);
    //printf("%d\n",Rrow);
        fp=fopen(R_M, "r");
    //int Rcol=get_column(fp);
    //printf("%d\n",Rcol);
        
        read_row_and_column(fp, RC);
        const int Rrow=RC[0];
        const int Rcol=RC[1];
        
    
        int Rline=get_row(R_M)-1;
        int* Rr=(int *)malloc(Rline*sizeof(int));
        int* Rc=(int *)malloc(Rline*sizeof(int));
        double* R=(double *)malloc(Rline*sizeof(double));
        read_sparse_matrix(Rr, Rc, R, fp);
        fclose(fp);
    /*int m,n;
    for (m=0; m<Rrow; ++m) {
        for (n=0; n<Rcol; ++n) {
            printf("%lf\t",R[m][n]);
        }
        printf("\n");
    }*/
    
    printf("2\n");
    // K
    /*
    int Krow=get_row(Keyword_M);
    fp=fopen(Keyword_M, "r");
    int Kcol=get_column(fp);
    double **K = (double **)malloc(sizeof(double)*Krow);
    for (i=0; i<Krow; i++) {
        K[i]=(double *)malloc(sizeof(double)*Kcol);
    }
    //fclose(fp);fp=fopen(Keyword_M, "r");
    read_matrix_from_file(K, Krow, Kcol, fp);
    matrix_times(1/KNORM, K, K, Krow, Kcol);
    fclose(fp);
     */
    int Kline=get_row(Keyword_M)-1;
    fp=fopen(Keyword_M, "r");
        
        read_row_and_column(fp, RC);
        const int Krow=RC[0];
        const int Kcol=RC[1];
        
        
    int* Kr=(int *)malloc(Kline*sizeof(int));
    int* Kc=(int *)malloc(Kline*sizeof(int));
    double* K=(double *)malloc(Kline*sizeof(double));
    read_sparse_matrix(Kr, Kc, K, fp);
    sparse_matrix_times(1/KNORM, K, Kline);
    fclose(fp);
    
        printf("3\n");
    //KT
    
    fp=fopen(Keyword_MT, "r");
        read_row_and_column(fp, RC);
    int* KTr=(int *)malloc(Kline*sizeof(int));
    int* KTc=(int *)malloc(Kline*sizeof(int));
    double* KT=(double *)malloc(Kline*sizeof(double));
    read_sparse_matrix(KTr, KTc, KT, fp);
    sparse_matrix_times(1/KNORM, KT, Kline);
    fclose(fp);
    
    // S
    /*
    int Srow=get_row(Social_M);
    fp=fopen(Social_M, "r");
    int Scol=get_column(fp);
    double **S = (double **)malloc(sizeof(double)*Srow);
    for (i=0; i<Srow; i++) {
        S[i]=(double *)malloc(sizeof(double)*Scol);
    }
    //fclose(fp);fp=fopen(Social_M, "r");
    read_matrix_from_file(S, Srow, Scol, fp);
    matrix_times(1/SNORM, S, S, Srow, Scol);
    fclose(fp);
    */
    int Sline=get_row(Social_M)-1;
    fp=fopen(Social_M, "r");
        
        read_row_and_column(fp, RC);
        const int Srow=RC[0];
        const int Scol=RC[1];
        
        
    int* Sr=(int *)malloc(Sline*sizeof(int));
    int* Sc=(int *)malloc(Sline*sizeof(int));
    double* S=(double *)malloc(Sline*sizeof(double));
    read_sparse_matrix(Sr, Sc, S, fp);
    sparse_matrix_times(1/SNORM, S, Sline);
    fclose(fp);
    printf("-\n");
    //ST
    fp=fopen(Social_MT, "r");
        read_row_and_column(fp, RC);
    int* STr=(int *)malloc(Sline*sizeof(int));
    int* STc=(int *)malloc(Sline*sizeof(int));
    double* ST=(double *)malloc(Sline*sizeof(double));
    read_sparse_matrix(STr, STc, ST, fp);
    sparse_matrix_times(1/SNORM, ST, Sline);
    fclose(fp);
    
    
    printf("4\n");
    
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
    printf("4.1\n");
    // Calculate M = B+PQ
    //int j;
    //1. M1=K*P1/|K|
    int M1row=Krow;
    int M1col=F;
    double **M1 = (double **)malloc(sizeof(double)*M1row);
    for (i=0; i<M1row; i++) {
        M1[i]=(double *)malloc(sizeof(double)*M1col);
        memset(M1[i], 0, sizeof(double)*M1col);
    }
    
    //matrix_multiply(K, P1, M1, Krow, Kcol, F);
    sparse_matrix_multiply(Kr, Kc, K, P1, M1, Krow, Kcol, F, Kline);
    printf("4.2\n");
    //2. M2=S*P2/|S|
    int M2row=Srow;
    int M2col=F;
    double **M2 = (double **)malloc(sizeof(double)*M2row);
    for (i=0; i<M2row; i++) {
        M2[i]=(double *)malloc(sizeof(double)*M2col);
        memset(M2[i], 0, sizeof(double)*M2col);
    }
    
    //matrix_multiply(S, P2, M2, Srow, Scol, F);
    sparse_matrix_multiply(Sr, Sc, S, P2, M2, Srow, Kcol, F,Sline);
    printf("4.3\n");
    //3. M1=M1+M2
    matrix_add(M1, M2, M1, M1row, M1col);
    
    //4. M = M1*Q
    int Mrow = Rrow;
    int Mcol = Rcol;
    double **M = (double **)malloc(sizeof(double)*Mrow);
    for (i=0; i<Mrow; i++) {
        M[i]=(double *)malloc(sizeof(double)*Mcol);
    }
    matrix_multiply(M1, Q, M, M1row, M1col, Qcol);
    printf("4.4\n");
    //5. M = M+B
    matrix_add(M, B, M, Mrow, Mcol);
    
    
    printf("5\n");
    // assertion
    
    assert(Rrow==Krow);
    assert(Rrow==Srow);
    
    
    
    // tmp matrix used for gradient descent calculation

    
    //1. tmp0 = M*QT
    double **tmp0 =(double **)malloc(sizeof(double)*Mrow);
    for (i=0; i<Mrow; i++) {
        tmp0[i]=(double *)malloc(sizeof(double)*Qrow);
        memset(tmp0[i], 0, sizeof(double)*Qrow);
    }
        
    //2. tmp1 = KT * (M * QT) = P1
    double **tmp1 = (double **)malloc(sizeof(double)*P1row);
    for (i=0; i<P1row; i++) {
        tmp1[i]=(double *)malloc(sizeof(double)*P1col);
        memset(tmp1[i], 0, sizeof(double)*P1col);
    }
        

    
    //3. tmp2 = ST * (M * QT) = P2
    double **tmp2 = (double **)malloc(sizeof(double)*P2row);
    for (i=0; i<P2row; i++) {
        tmp2[i]=(double *)malloc(sizeof(double)*P2col);
        memset(tmp2[i], 0, sizeof(double)*P2col);
    }
        
    
    //4. tmp3 = K* P1
    double **tmp3 = (double **)malloc(sizeof(double)*Krow);
    for (i=0; i<Krow; i++) {
        tmp3[i]=(double *)malloc(sizeof(double)*P1col);
        memset(tmp3[i], 0, sizeof(double)*P1col);
    }
        
    
    //5. tmp4 = S* P2
    double **tmp4 = (double **)malloc(sizeof(double)*Srow);
    for (i=0; i<Srow; i++) {
        tmp4[i]=(double *)malloc(sizeof(double)*P2col);
        memset(tmp4[i],0, sizeof(double)*P2col);
    }
        
    //6. tmp5=(tmp3+tmp4)T * M   --- F * Mcol
    
    double **tmp5 = (double **)malloc(sizeof(double)*F);
    for (i=0; i<F; i++) {
        tmp5[i]=(double *)malloc(sizeof(double)*Mcol);
        memset(tmp5[i], 0, sizeof(double)*Mcol);
    }
        
    
    printf("6\n");
    
        
        
        
        
        
        struct timespec currentTime;
        clock_gettime(0,&currentTime);
        
        
        
    // Main Process
    int step=0;
    double alpha_p=alpha;
    for (step=0;step<N;step++){
        
        for (i=0; i<M1row; i++) {
            memset(M1[i], 0, sizeof(double)*M1col);
        }
        for (i=0; i<M2row; i++) {
            memset(M2[i], 0, sizeof(double)*M2col);
        }
        
        
        printf("7\n");
        //gradient descent
        
        // M = R-M
        //matrix_substract(R, M, M, Rrow, Rcol);
        sparse_matrix_substract(Rr, Rc, R, M, Rline);
        printf("7.1\n");
        // M = alpha * M
        matrix_times(alpha_p, M, M, Mrow, Mcol);
        printf("7.2\n");
        //1. tmp0 = M*QT
        matrix_multiply1(M, Q, tmp0, Mrow, Mcol, Qrow);
        printf("7.3\n");
        // P1
        matrix_times(1+alpha_p*lambda, P1, P1, P1row, P1col);
        printf("7.4\n");
        //matrix_multiply2(K, M, tmp1, Kcol, Krow, Mcol);
        sparse_matrix_multiply(KTr, KTc, KT, tmp0, tmp1, Kcol, Krow, Mcol, Kline);
        
        matrix_add(P1, tmp1, P1, P1row, P1col);
        printf("7.5\n");
        // P2
        matrix_times(1+alpha_p*lambda, P2, P2, P2row, P2col);
        printf("7.5.1\n");
        //matrix_multiply2(S, M, tmp2, Scol, Srow, Mcol);
        
        
        printf("ST: %d, %d\n", Scol,Srow);
        printf("M : %d, %d\n", Mrow, Mcol);
        printf("tmp2: %d, %d\n",P2row, P2col);
        
        i=0;int max1=-1,max2=-1;
        for (i=0; i<Sline; i++) {
            max1=STr[i]>max1?STr[i]:max1;
            max2=STc[i]>max2?STc[i]:max2;
        }
        printf("MAX ROW: %d,  MAX COL: %d\n",max1, max2);
        
        printf("%lf %lf \n", M[Mrow-1][Mcol-1], tmp2[P2row-1][P2col-1]);
        
        
        
        
        
        
        sparse_matrix_multiply(STr, STc, ST, tmp0, tmp2, Scol, Srow, Mcol, Sline);
        printf("7.6\n");
        matrix_add(P2, tmp2, P2, P2row, P2col);
        printf("7.7\n");
        
        // Q
        matrix_times(1+alpha_p*lambda, Q, Q, Qrow, Qcol);
        printf("7.8\n");
        //matrix_multiply(K, P1, tmp3, Krow, Kcol, P1col);
        sparse_matrix_multiply(Kr, Kc, K, P1, tmp3, Krow, Kcol, P1col,Kline);
        //matrix_multiply(S, P2, tmp4, Srow, Scol, P2col);
        sparse_matrix_multiply(Sr, Sc, S, P2, tmp4, Srow, Scol, P2col,Sline );
        matrix_add(tmp3, tmp4, tmp3, Krow, P1col);
        printf("7.9\n");
        matrix_multiply2(tmp3, M, tmp5, P1col, Krow, Mcol);
        printf("7.10\n");
        matrix_add(Q, tmp5, Q, Qrow, Qcol);
        
        
        // B
        matrix_times(1+alpha_p*lambda, B, B, Brow, Bcol);
        matrix_add(M, B, B, Brow, Bcol);
        printf("7.11\n");
        
        // M:
        //1. M1=K*P1/|K|
        //matrix_multiply(K, P1, M1, Krow, Kcol, F);
        sparse_matrix_multiply(Kr, Kc, K, P1, M1, Krow, Kcol, F,Kline);
        printf("7.12\n");
        //2. M2=S*P2/|S|
        //matrix_multiply(S, P2, M2, Srow, Scol, F);
        sparse_matrix_multiply(Sr, Sc, S, P2, M2, Srow, Scol, F,Sline);
        
        printf("7.13\n");
        
        //3. M1=M1+M2
        matrix_add(M1, M2, M1, M1row, M1col);
        printf("7.14\n");
        
        //4. M = M1*Q
        matrix_multiply(M1, Q, M, M1row, M1col, Qcol);
        printf("7.15\n");
        //5. M = M+B
        matrix_add(M, B, M, Mrow, Mcol);
        printf("7.16\n");
        
        
        
        for (i=0; i<P1row; i++) {
            memset(tmp1[i], 0, sizeof(double)*P1col);
        }
        for (i=0; i<P2row; i++) {
            memset(tmp2[i], 0, sizeof(double)*P2col);
        }
        for (i=0; i<Krow; i++) {
            memset(tmp3[i], 0, sizeof(double)*P1col);
        }
        for (i=0; i<Srow; i++) {
            memset(tmp4[i],0, sizeof(double)*P2col);
        }

        
        alpha_p*=0.9;
    }
    
    // Generate Model & write to file: model.csv
    
    // write to file
    write_matrix_to_file(M, Mrow, Mcol);
    
      printf("8\n");
        
        
    //traditional ways
    /*}else{
        
        int i=0;
        FILE *fp;
        
        
        // R
        int Rrow=get_row(R_M);
        //printf("%d\n",Rrow);
        fp=fopen(R_M, "r");
        int Rcol=get_column(fp);
        //printf("%d\n",Rcol);
        double **R = (double **)malloc(sizeof(double)*Rrow);
        for (i=0; i<Rrow; i++) {
            R[i]=(double *)malloc(sizeof(double)*Rcol);
        }
        //fclose(fp);fp=fopen(R_M, "r");
        read_matrix_from_file(R, Rrow, Rcol, fp);
        fclose(fp);
        int m,n;
         for (m=0; m<Rrow; ++m) {
         for (n=0; n<Rcol; ++n) {
         printf("%lf\t",R[m][n]);
         }
         printf("\n");
         }
        
        
        // K
        int Krow=get_row(Keyword_M);
        fp=fopen(Keyword_M, "r");
        int Kcol=get_column(fp);
        double **K = (double **)malloc(sizeof(double)*Krow);
        for (i=0; i<Krow; i++) {
            K[i]=(double *)malloc(sizeof(double)*Kcol);
        }
        //fclose(fp);fp=fopen(Keyword_M, "r");
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
        //fclose(fp);fp=fopen(Social_M, "r");
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
        matrix_multiply(K, P1, M1, Krow, Kcol, F);
        
        
        //2. M2=S*P2/|S|
        int M2row=Srow;
        int M2col=F;
        double **M2 = (double **)malloc(sizeof(double)*M2row);
        for (i=0; i<M2row; i++) {
            M2[i]=(double *)malloc(sizeof(double)*M2col);
        }
        matrix_multiply(S, P2, M2, Srow, Scol, F);
        
        
        //3. M1=M1+M2
        matrix_add(M1, M2, M1, M1row, M1col);
        
        //4. M = M1*Q
        int Mrow = Rrow;
        int Mcol = Rcol;
        double **M = (double **)malloc(sizeof(double)*Mrow);
        for (i=0; i<Mrow; i++) {
            M[i]=(double *)malloc(sizeof(double)*Mcol);
        }
        matrix_multiply(M1, Q, M, M1row, M1col, Qcol);
        
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
        double alpha_p=alpha;
        for (step=0;step<N;step++){
            
            //gradient descent
            
            // M = R-M
            matrix_substract(R, M, M, Rrow, Rcol);
            
            // M = alpha * M
            matrix_times(alpha_p, M, M, Mrow, Mcol);
            
            //1. tmp0 = M*QT
            matrix_multiply1(M, Q, tmp0, Mrow, Mcol, Qrow);
            
            // P1
            matrix_times(1+alpha_p*lambda, P1, P1, P1row, P1col);
            
            matrix_multiply2(K, M, tmp1, Kcol, Krow, Mcol);
            
            matrix_add(P1, tmp1, P1, P1row, P1col);
            
            // P2
            matrix_times(1+alpha_p*lambda, P2, P2, P2row, P2col);
            
            matrix_multiply2(S, M, tmp2, Scol, Srow, Mcol);
            
            matrix_add(P2, tmp2, P2, P2row, P2col);
            
            
            // Q
            matrix_times(1+alpha_p*lambda, Q, Q, Qrow, Qcol);
            
            matrix_multiply(K, P1, tmp3, Krow, Kcol, P1col);
            
            matrix_multiply(S, P2, tmp4, Srow, Scol, P2col);
            
            matrix_add(tmp3, tmp4, tmp3, Krow, P1col);
            
            matrix_multiply2(tmp3, M, tmp5, P1col, Krow, Mcol);
            
            matrix_add(Q, tmp5, Q, Qrow, Qcol);
            
            
            // B
            matrix_times(1+alpha_p*lambda, B, B, Brow, Bcol);
            matrix_add(M, B, B, Brow, Bcol);
            
            
            // M:
            //1. M1=K*P1/|K|
            matrix_multiply(K, P1, M1, Krow, Kcol, F);
            
            //2. M2=S*P2/|S|
            matrix_multiply(S, P2, M2, Srow, Scol, F);
            
            //3. M1=M1+M2
            matrix_add(M1, M2, M1, M1row, M1col);
            
            //4. M = M1*Q
            matrix_multiply(M1, Q, M, M1row, M1col, Qcol);
            
            //5. M = M+B
            matrix_add(M, B, M, Mrow, Mcol);
            
            alpha_p*=0.9;
        }
        
        // Generate Model & write to file: model.csv
        
        // write to file
        write_matrix_to_file(M, Mrow, Mcol);
    
    }*/
    }
    return 0;
}
