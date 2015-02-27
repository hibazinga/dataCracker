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

//#define KNORM 1   // matrix norm of K calculated by matlab
//#define SNORM 1   //            --  S --
#define F 128       // # of latent factors 64 or 128
#define N 50        // # of iterations
#define alpha -0.01 // learning rate, alpha = -alpha
#define lambda 0.01 // normalization factor

/* --- CHANGE IT WITH YOUR FILE PATH! ---*/

#define Keyword_M "/Users/qiuhanding/Documents/249/project/dataCracker/NormalizedK/user_key_word_13_ij.txt"
#define Social_M "/Users/qiuhanding/Documents/249/project/dataCracker/NormalizedS/S_13_nu_n.txt"
#define Keyword_MT "/Users/qiuhanding/Documents/249/project/dataCracker/NormalizedK/user_key_word_13_ji.txt"
#define Social_MT "/Users/qiuhanding/Documents/249/project/dataCracker/NormalizedS/S_13_n_nu.txt"
#define R_M "/Users/qiuhanding/Documents/249/project/dataCracker/Rui/Rui_13_nm.txt"
#define R_MT "/Users/qiuhanding/Documents/249/project/dataCracker/Rui/Rui_13_mn.txt"
#define OUTPUT "model.csv"

/*#define Keyword_M "K.csv"
  #define Social_M "S.csv"
  #define Keyword_MT "KT.csv"
  #define Social_MT "ST.csv"
  #define R_M "R.csv"
*/

#define SPARSE 1


/***********************
 
 Matrix:
 
 Rui = Rrow * Rcol = All User * All Item
 
 M = R-bar = B + P * Q = B + (K*P1 + S*P2) * Q
 
 K = Krow * Kcol = Group User * Keyword  n*k
 
 KT = Kcol * Krow  k*n
 
 S = Srow * Scol = Group User * All User n*N
 
 ST = Scol * Srow N*n
 
 P1 = Kcol * F k*f
 
 P2 = Scol * F N*f
 
 Q = F * Rcol f*m
 
*************************/

int main(int argc, const char * argv[])
{
  printf("1\n");
  //LFM Model
  if (SPARSE){ //use sparse matrix
        
    int i=0;
    FILE *fp;
    int *RC = (int *)malloc(2*sizeof(int));
    
    // R - initialize, sparse
        
    fp=fopen(R_M, "r");
    read_row_and_column(fp, RC);
    const int Rrow=RC[0];
    const int Rcol=RC[1];
    
    int Rline=get_row(R_M)-1;
    int* Rr=(int *)malloc(Rline*sizeof(int));
    int* Rc=(int *)malloc(Rline*sizeof(int));
    double* R=(double *)malloc(Rline*sizeof(double));
    read_sparse_matrix(Rr, Rc, R, fp);
    fclose(fp);

    fp=fopen(R_MT, "r");
    read_row_and_column(fp, RC);
    const int RTrow=RC[0];
    const int RTcol=RC[1];
    
    int* RTr=(int *)malloc(Rline*sizeof(int));
    int* RTc=(int *)malloc(Rline*sizeof(int));
    double* RT=(double *)malloc(Rline*sizeof(double));
    read_sparse_matrix(RTr, RTc, RT, fp);
    fclose(fp);

    printf("2\n");

    // K - initialize, sparse
        
    int Kline=get_row(Keyword_M)-1;
    fp=fopen(Keyword_M, "r");
        
    read_row_and_column(fp, RC);
    const int Krow=RC[0];
    const int Kcol=RC[1];
        
        
    int* Kr=(int *)malloc(Kline*sizeof(int));
    int* Kc=(int *)malloc(Kline*sizeof(int));
    double* K=(double *)malloc(Kline*sizeof(double));
    read_sparse_matrix(Kr, Kc, K, fp);
    fclose(fp);
    
    printf("3\n");
        
    //KT - initialize, sparse
    
    fp=fopen(Keyword_MT, "r");
    read_row_and_column(fp, RC);
    int* KTr=(int *)malloc(Kline*sizeof(int));
    int* KTc=(int *)malloc(Kline*sizeof(int));
    double* KT=(double *)malloc(Kline*sizeof(double));
    read_sparse_matrix(KTr, KTc, KT, fp);
    fclose(fp);
    
    // S - initialize, sparse

    int Sline=get_row(Social_M)-1;
    fp=fopen(Social_M, "r");
        
    read_row_and_column(fp, RC);
    const int Srow=RC[0];
    const int Scol=RC[1];
        
        
    int* Sr=(int *)malloc(Sline*sizeof(int));
    int* Sc=(int *)malloc(Sline*sizeof(int));
    double* S=(double *)malloc(Sline*sizeof(double));
    read_sparse_matrix(Sr, Sc, S, fp);
    fclose(fp);
        
    printf("3.5\n");
    
    //ST - initialize, sparse
        
    fp=fopen(Social_MT, "r");
    read_row_and_column(fp, RC);
    int* STr=(int *)malloc(Sline*sizeof(int));
    int* STc=(int *)malloc(Sline*sizeof(int));
    double* ST=(double *)malloc(Sline*sizeof(double));
    read_sparse_matrix(STr, STc, ST, fp);
    fclose(fp);
    
    
    printf("4\n");
    
    // bI,bU - random_init, matrix
    int Brow=Rrow;
    int Bcol=Rcol;
    double *bU = (double*)malloc(sizeof(double)*Brow);
    double *bI = (double*)malloc(sizeof(double)*Bcol);
    //double **B = (double **)malloc(sizeof(double)*Brow);
    //for (i=0; i<Brow; i++) {
    //    B[i]=(double *)malloc(sizeof(double)*Bcol);
    //}
    random_initialize_vector(bU, Brow);
    random_initialize_vector(bI, Bcol);

    // P1 - random_init, matrix
    
    int P1row=Kcol;
    int P1col=F;
    double **P1 = (double **)malloc(sizeof(double)*P1row);
    for (i=0; i<P1row; i++) {
      P1[i]=(double *)malloc(sizeof(double)*P1col);
    }
    random_initialize(P1, P1row, P1col);
    
    // P2 - random_init, matrix
    
    int P2row=Scol;
    int P2col=F;
    double **P2 = (double **)malloc(sizeof(double)*P2row);
    for (i=0; i<P2row; i++) {
      P2[i]=(double *)malloc(sizeof(double)*P2col);
    }
    random_initialize(P2, P2row, P2col);
    
    // Q - random_init, matrix
    
    int Qrow=F;
    int Qcol=Rcol;
    double **Q = (double **)malloc(sizeof(double)*Qrow);
    for (i=0; i<Qrow; i++) {
      Q[i]=(double *)malloc(sizeof(double)*Qcol);
    }
    random_initialize(Q, Qrow, Qcol);
        
    printf("4.1\n");
        

    /* Calculate  M = B + P * Q
     *              = B + (K*P1 + S*P2) * Q
     *              = B + (M1 + M2) * Q
     */
    //int j;
        
    //1. M1 = K * P1
    int M1row=Krow;
    int M1col=F;
    double **M1 = (double **)malloc(sizeof(double)*M1row);
    for (i=0; i<M1row; i++) {
      M1[i]=(double *)malloc(sizeof(double)*M1col);
      memset(M1[i], 0, sizeof(double)*M1col);
    }

    sparse_matrix_multiply(Kr, Kc, K, P1, M1, Krow, Kcol, F, Kline);
        
    printf("4.2\n");        
        
    //2. M2 = S * P2

    int M2row=Srow;
    int M2col=F;
    double **M2 = (double **)malloc(sizeof(double)*M2row);
    for (i=0; i<M2row; i++) {
      M2[i]=(double *)malloc(sizeof(double)*M2col);
      memset(M2[i], 0, sizeof(double)*M2col);
    }

    sparse_matrix_multiply(Sr, Sc, S, P2, M2, Srow, Kcol, F,Sline);
        
    printf("4.3\n");
        
        
    //3. M1 = M1 + M2
    matrix_add(M1, M2, M1, M1row, M1col);
    
    //4. M = M1*Q  ////////Qiuhan
    double *M = (double*)malloc(sizeof(double)*Rline);
    double *MT = (double*)malloc(sizeof(double)*Rline);
    matrix_multiply_result_in_sparse(M1, Q, M, F, Rr, Rc, Rline);

    //Matrix of Rbar, but not used in calculation
    double **Temp = (double **)malloc(sizeof(double)*Rrow);
    for (i=0; i<Rrow; i++) {
      Temp[i]=(double *)malloc(sizeof(double)*Rcol);
    }
    //matrix_multiply(M1, Q, M, M1row, M1col, Qcol);

    printf("4.4\n");
    //5. M = M+B  /////////Qiuhan
    matrix_add1(M, bI, bU, M, Rr, Rc, Rline);
    //6. MT
    matrix_load(M, Temp, Rr, Rc, Rline);
    matrix_load2(Temp, MT, RTr, RTc, Rline);

    for (i=0; i<Rrow; i++) {
      memset(Temp[i],0, sizeof(double)*Rcol);
    }
    printf("5\n");
        
        
        
    // assertion Rrow==Krow==Srow== Group User #
    assert(Rrow==Krow);
    assert(Rrow==Srow);
    
    
    
    // tmp matrix used for gradient descent calculation

    //1. tmp0 = M*QT ->Rui*QT
    double **tmp0 =(double **)malloc(sizeof(double)*Rrow);
    for (i=0; i<Rrow; i++) {
      tmp0[i]=(double *)malloc(sizeof(double)*Qrow);
      memset(tmp0[i], 0, sizeof(double)*Qrow);
    }
        
    //2. tmp1 = KT * (M * QT) = P1 ->gradient of P1
    double **tmp1 = (double **)malloc(sizeof(double)*P1row);
    for (i=0; i<P1row; i++) {
      tmp1[i]=(double *)malloc(sizeof(double)*P1col);
      memset(tmp1[i], 0, sizeof(double)*P1col);
    }
        

    //3. tmp2 = ST * (M * QT) = P2 ->gradient of P2
    double **tmp2 = (double **)malloc(sizeof(double)*P2row);
    for (i=0; i<P2row; i++) {
      tmp2[i]=(double *)malloc(sizeof(double)*P2col);
      memset(tmp2[i], 0, sizeof(double)*P2col);
    }
    
    //4. tmp3 = K * P1
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
        
    //6. tmp5=(tmp3+tmp4)T * M   --- F * Mcol  ->gradient of Q
    
    double **tmp5 = (double **)malloc(sizeof(double)*F);
    for (i=0; i<F; i++) {
      tmp5[i]=(double *)malloc(sizeof(double)*Rcol);
      memset(tmp5[i], 0, sizeof(double)*Rcol);
    }

    printf("6\n");  
        
    // Main Process
    int step=0;
    double alpha_p=alpha;
    for (step=0;step<N;step++){

      printf("---Step: %d---\n",step);
        
      //time Stamp
      time_t t = time(0);
      char tmp[64];
      strftime( tmp, sizeof(tmp), "%Y/%m/%d %X %A [%j day of this year] TimeZone:%z",localtime(&t) );
      puts( tmp );
        
        
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
      sparse_matrix_substract(R, M, Rline);
      sparse_matrix_substract(RT, MT, Rline);
      printf("7.1\n");
      //0. M = alpha*M
      sparse_matrix_times(alpha_p, M, Rline);
      sparse_matrix_times(alpha_p, MT, Rline);
      //1. tmp0 = M*QT
      sparse_matrix_multiply1(Rr, Rc, M, Q, tmp0, Rrow, Rcol, Qrow, Rline);
      //2. tmp1 = KT*tmp0
      sparse_matrix_multiply(KTr, KTc, KT, tmp0, tmp1, Kcol, Krow, Rcol, Kline);
      //3. tmp2 = ST*tmp0
      sparse_matrix_multiply(STr, STc, ST, tmp0, tmp2, Scol, Srow, Rcol, Sline);
      //4. tmp3 = K*P1
      sparse_matrix_multiply(Kr, Kc, K, P1, tmp3, Krow, Kcol, P1col,Kline);
      //5. tmp4 = S*P2
      sparse_matrix_multiply(Sr, Sc, S, P2, tmp4, Srow, Scol, P2col,Sline);
      //6. tmp3 = tmp3 + tmp4
      matrix_add(tmp3, tmp4, tmp3, Krow, P1col);
      //7. tmp5 = tmp3T*MT
      sparse_matrix_multiply2(tmp3, RTr, RTc, MT, tmp5, F, Rrow, Rcol, Rline);
      //8. P1 = (1+alpha*lambda)*P1
      matrix_times(1+alpha_p*lambda, P1, P1, P1row, P1col);
      //9. P2 = (1+alpha*lambda)*P2
      matrix_times(1+alpha_p*lambda, P2, P2, P2row, P2col);
      //10. Q = (1+alpha*lambda)*Q
      matrix_times(1+alpha_p*lambda, Q, Q, Qrow, Qcol);
      //11. bI = (1+alpha*lambda)*bI
      sparse_matrix_times(1+alpha_p*lambda, bI, Bcol);
      //12. bU = (1+alpha*lambda)*bU
      sparse_matrix_times(1+alpha_p*lambda, bI, Bcol);
      //13. P1 = P1+tmp1
      matrix_add(P1, tmp1, P1, P1row, P1col);
      //14. P2 = P2+tmp2
      matrix_add(P2, tmp2, P2, P2row, P2col);
      //15. Q = Q + tmp5
      matrix_add(Q, tmp5, Q, Qrow, Qcol);
      //16. bU = bU + M
      vector_add(bU, M, Rr, Rline);
      //17. bI = bI + MT
      vector_add(bI, MT, RTr, Rline);

      // M:
      //1. M1=K*P1/|K|
      //matrix_multiply(K, P1, M1, Krow, Kcol, F);
      sparse_matrix_multiply(Kr, Kc, K, P1, M1, Krow, Kcol, F, Kline);
      printf("7.12\n");
      //2. M2=S*P2/|S|
      //matrix_multiply(S, P2, M2, Srow, Scol, F);
      sparse_matrix_multiply(Sr, Sc, S, P2, M2, Srow, Scol, F, Sline);
        
      printf("7.13\n");
        
      //3. M1=M1+M2
      matrix_add(M1, M2, M1, M1row, M1col);
      printf("7.14\n");
        
      //4. M = M1*Q
      matrix_multiply_result_in_sparse(M1, Q, M, F, Rr, Rc, Rline);
      printf("7.15\n");
      //5. M = M+B
      matrix_add1(M, bI, bU, M, Rr, Rc, Rline);
      //6. MT
      matrix_load(M, Temp, Rr, Rc, Rline);
      matrix_load2(Temp, MT, RTr, RTc, Rline);
      printf("7.16\n");
        
        
      for (i=0; i<Rrow; i++) {
	memset(tmp0[i],0, sizeof(double)*P1col);
      }
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
      for (i=0; i<Qrow; i++) {
	memset(tmp5[i],0, sizeof(double)*Qcol);
      }
      for (i=0; i<Rrow; i++) {
	memset(Temp[i],0, sizeof(double)*Rcol);
      }

      alpha_p*=0.9;

    }
    printf("end of gd\n");
    // Generate Model & write to file: model.csv
    // Recalculate M*Q
    matrix_multiply(M1, Q, Temp, M1row, M1col, Qcol);
    // M = M+B
    matrix_add2(Temp, bI, bU, Temp, Rrow, Rcol);

    write_matrix_to_file(Temp, Rrow, Rcol);
    
    printf("8\n");
        
  }
  return 0;
}
