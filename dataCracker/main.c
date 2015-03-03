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
#define N 100        // # of iterations
#define alpha -0.00001 // learning rate, alpha = -alpha
#define lambda 0.001 // normalization factor

/* --- CHANGE IT WITH YOUR FILE PATH! ---*/

/*#define Keyword_M "/Users/qiuhanding/Documents/249/project/dataCracker/NormalizedK/user_key_word_13_ij.txt"
#define Social_M "/Users/qiuhanding/Documents/249/project/dataCracker/NormalizedS/S_13_nu_n.txt"
#define Keyword_MT "/Users/qiuhanding/Documents/249/project/dataCracker/NormalizedK/user_key_word_13_ji.txt"
#define Social_MT "/Users/qiuhanding/Documents/249/project/dataCracker/NormalizedS/S_13_n_nu.txt"
#define R_M "/Users/qiuhanding/Documents/249/project/dataCracker/Rui/Rui_13_nm.txt"
#define R_MT "/Users/qiuhanding/Documents/249/project/dataCracker/Rui/Rui_13_mn.txt"*/
#define OUTPUT "model2.csv"

#define Keyword_M "user_key_word_2_ij.txt"
  #define Social_M "S_2_nu_n.txt"
  #define Keyword_MT "user_key_word_2_ji.txt"
  #define Social_MT "S_2_n_nu.txt"
  #define R_M "Rui_2_nm.txt"
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
    const int nu=RC[0];
    const int m=RC[1];
  
    int Rline=get_row(R_M)-1;
    int* Rr=(int *)malloc(Rline*sizeof(int));
    int* Rc=(int *)malloc(Rline*sizeof(int));
    double* R=(double *)malloc(Rline*sizeof(double));
    read_sparse_matrix(Rr, Rc, R, fp);
    fclose(fp);
    
    // K - initialize, sparse
        
    int Kline=get_row(Keyword_M)-1;
    fp=fopen(Keyword_M, "r");
        
    read_row_and_column(fp, RC);
    const int Krow=RC[0];
    const int Kcol=RC[1];
    const int k=RC[1];
        
    int* Kr=(int *)malloc(Kline*sizeof(int));
    int* Kc=(int *)malloc(Kline*sizeof(int));
    double* K=(double *)malloc(Kline*sizeof(double));
    read_sparse_matrix(Kr, Kc, K, fp);
    fclose(fp);
        
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
    const int n=RC[1];
        
        
    int* Sr=(int *)malloc(Sline*sizeof(int));
    int* Sc=(int *)malloc(Sline*sizeof(int));
    double* S=(double *)malloc(Sline*sizeof(double));
    read_sparse_matrix(Sr, Sc, S, fp);
    fclose(fp);
    
    //ST - initialize, sparse
        
    fp=fopen(Social_MT, "r");
    read_row_and_column(fp, RC);
    int* STr=(int *)malloc(Sline*sizeof(int));
    int* STc=(int *)malloc(Sline*sizeof(int));
    double* ST=(double *)malloc(Sline*sizeof(double));
    read_sparse_matrix(STr, STc, ST, fp);
    fclose(fp);    
    
    // bI,bU - random_init, matrix
    double *bU = (double*)malloc(sizeof(double)*nu);
    double *bI = (double*)malloc(sizeof(double)*m);

    random_initialize_vector(bU, nu);
    random_initialize_vector(bI, m);

    // P1 - random_init, matrix
    
    int P1row=k;
    int P1col=F;
    double **P1 = (double **)malloc(sizeof(double)*k);
    for (i=0; i<k; i++) {
      P1[i]=(double *)malloc(sizeof(double)*F);
    }
    random_initialize(P1, k, F);
    
    // P2 - random_init, matrix
    
    int P2row=n;
    int P2col=F;
    double **P2 = (double **)malloc(sizeof(double)*n);
    for (i=0; i<n; i++) {
      P2[i]=(double *)malloc(sizeof(double)*F);
    }
    random_initialize(P2, n, F);
    
    // Q - random_init, matrix
    
    int Qrow=F;
    int Qcol=m;
    double **Q = (double **)malloc(sizeof(double)*F);
    for (i=0; i<F; i++) {
      Q[i]=(double *)malloc(sizeof(double)*m);
    }
    random_initialize(Q, F, m);        

    //1. M1 = K * P1
    int M1row=nu;
    int M1col=F;
    double **M1 = (double **)malloc(sizeof(double)*nu);
    for (i=0; i<nu; i++) {
      M1[i]=(double *)malloc(sizeof(double)*F);
      memset(M1[i], 0, sizeof(double)*F);
    }

    sparse_matrix_multiply(Kr, Kc, K, P1, M1, nu, k, F, Kline);
        
    //2. M1 += S * P2
    sparse_matrix_multiply(Sr, Sc, S, P2, M1, nu, n, F,Sline);
        
    //4. M = M1*Q  ////////Qiuhan
    double *M = (double*)malloc(sizeof(double)*Rline);
    matrix_multiply_result_in_sparse(M1, Q, M, F, Rr, Rc, Rline);

   //5. M = M+B  /////////Qiuhan
    matrix_add1(M, bI, bU, M, Rr, Rc, Rline);

    // assertion Rrow==Krow==Srow== Group User #
    assert(Rrow==Krow);
    assert(Rrow==Srow);
    
    
    // tmp matrix used for gradient descent calculation

    //1. tmp0 = M*QT ->Rui*QT
    double **tmp0 =(double **)malloc(sizeof(double)*nu);
    for (i=0; i<nu; i++) {
      tmp0[i]=(double *)malloc(sizeof(double)*F);
      memset(tmp0[i], 0, sizeof(double)*F);
    }
        
    //2. tmp1 = KT * (M * QT) = P1 ->gradient of P1
    double **tmp1 = (double **)malloc(sizeof(double)*k);
    for (i=0; i<k; i++) {
      tmp1[i]=(double *)malloc(sizeof(double)*F);
      memset(tmp1[i], 0, sizeof(double)*F);
    }

    //3. tmp2 = ST * (M * QT) = P2 ->gradient of P2
    double **tmp2 = (double **)malloc(sizeof(double)*n);
    for (i=0; i<n; i++) {
      tmp2[i]=(double *)malloc(sizeof(double)*F);
      memset(tmp2[i], 0, sizeof(double)*F);
    }
    
    //4. tmp3 = K * P1 + S * P2
    double **tmp3 = (double **)malloc(sizeof(double)*nu);
    for (i=0; i<nu; i++) {
      tmp3[i]=(double *)malloc(sizeof(double)*F);
      memset(tmp3[i], 0, sizeof(double)*F);
    }
        
    //5. tmp5 = tmp3T * M   --- F * Mcol  ->gradient of Q
    
    double **tmp5 = (double **)malloc(sizeof(double)*F);
    for (i=0; i<F; i++) {
      tmp5[i]=(double *)malloc(sizeof(double)*m);
      memset(tmp5[i], 0, sizeof(double)*m);
    }
        
    // Main Process
    int step=0;
    double alpha_p=alpha;
    for (step=0;step<N;step++){

      printf("---Step: %d---\n",step);
        
      for (i=0; i<M1row; i++) {
	memset(M1[i], 0, sizeof(double)*M1col);
      }

      //gradient descent
        
      // M = R-M
      sparse_matrix_substract(R, M, Rline);
      // calculating cost:
      double c = 0;
      for (int i = 0; i < Rline; i++)
	c += M[i]*M[i];
      printf("error in step %d is %f\n", step, c);
      for (int i = 0; i < P1row; i++)
	for (int j = 0; j < P1col; j++)
	  c+= lambda*(P1[i][j]*P1[i][j]);
      for (int i = 0; i < P2row; i++)
	for (int j = 0; j < P2col; j++)
	  c+= lambda*(P2[i][j]*P2[i][j]);
      for (int i = 0; i < Qrow; i++)
	for (int j = 0; j < Qcol; j++)
	  c+= lambda*(Q[i][j]*Q[i][j]);
      for (int i = 0; i < Rrow; i++)
      c+= lambda*bU[i]*bU[i];
      for (int i = 0; i < Rcol; i++)
      c+= lambda*bI[i]*bI[i];
      printf("cost in step %d is %f\n", step, c);
      //0. M = alpha*M
      sparse_matrix_times(alpha_p, M, Rline);
      //1. tmp0 = M*QT
      sparse_matrix_multiply1(Rr, Rc, M, Q, tmp0, nu, m, F, Rline);
      //2. tmp1 = KT*tmp0
      sparse_matrix_multiply(KTr, KTc, KT, tmp0, tmp1, k, nu, F, Kline);
      //3. tmp2 = ST*tmp0
      sparse_matrix_multiply(STr, STc, ST, tmp0, tmp2, n, nu, F, Sline);
      //4. tmp3 = K*P1
      sparse_matrix_multiply(Kr, Kc, K, P1, tmp3, nu, k, F,Kline);
      //5. tmp3 += S*P2
      sparse_matrix_multiply(Sr, Sc, S, P2, tmp3, nu, n, F,Sline);
      //6. tmp5 = tmp3T*MT
      sparse_matrix_multiply2(tmp3, Rc, Rr, M, tmp5, F, nu, m, Rline);
      //7. P1 = (1+alpha*lambda)*P1
      matrix_times(1+alpha_p*lambda, P1, P1, P1row, P1col);
      //8. P2 = (1+alpha*lambda)*P2
      matrix_times(1+alpha_p*lambda, P2, P2, P2row, P2col);
      //9. Q = (1+alpha*lambda)*Q
      matrix_times(1+alpha_p*lambda, Q, Q, Qrow, Qcol);
      //10. bI = (1+alpha*lambda)*bI
      sparse_matrix_times(1+alpha_p*lambda, bI, m);
      //11. bU = (1+alpha*lambda)*bU
      sparse_matrix_times(1+alpha_p*lambda, bU, nu);
      //12. P1 = P1+tmp1
      matrix_add(P1, tmp1, P1, P1row, P1col);
      //13. P2 = P2+tmp2
      matrix_add(P2, tmp2, P2, P2row, P2col);
      //14. Q = Q + tmp5
      matrix_add(Q, tmp5, Q, Qrow, Qcol);
      //15. bU = bU + M
      vector_add(bU, M, Rr, Rline);
      //16. bI = bI + MT
      vector_add(bI, M, Rc, Rline);
      
      //memset(bI,0, sizeof(double)*Rcol);
      //memset(bU,0, sizeof(double)*Rrow);

      // M:
      //1. M1=K*P1
      sparse_matrix_multiply(Kr, Kc, K, P1, M1, nu, k, F, Kline);
      //2. M1+=S*P2
      sparse_matrix_multiply(Sr, Sc, S, P2, M1, nu, n, F, Sline);
        
      //3. M = M1*Q
      matrix_multiply_result_in_sparse(M1, Q, M, F, Rr, Rc, Rline);
      //4. M = M+B
      matrix_add1(M, bI, bU, M, Rr, Rc, Rline);

      for (i=0; i<nu; i++) {
	memset(tmp0[i],0, sizeof(double)*F);
      }
      for (i=0; i<k; i++) {
	memset(tmp1[i], 0, sizeof(double)*F);
      }
      for (i=0; i<n; i++) {
	memset(tmp2[i], 0, sizeof(double)*F);
      }
      for (i=0; i<nu; i++) {
	memset(tmp3[i], 0, sizeof(double)*F);
      }
      for (i=0; i<F; i++) {
	memset(tmp5[i],0, sizeof(double)*m);
      }

      if (step%10 == 9)
	alpha_p*=0.9;

    }
    for (i=0; i<nu; i++) {
      free(tmp0[i]);
    }
    free(tmp0);
    for (i=0; i<k; i++) {
      free(tmp1[i]);
    }
    free(tmp1);
    for (i=0; i<n; i++) {
      free(tmp2[i]);
    }
    free(tmp2);
    for (i=0; i<nu; i++) {
      free(tmp3[i]);
    }
    free(tmp3);
    for (i=0; i<F; i++) {
      free(tmp5[i]);
    }
    free(tmp5);
    free(K);
    free(Kr);
    free(Kc);
    free(S);
    free(Sr);
    free(Sc);
    free(KT);
    free(KTr);
    free(KTc);
    free(ST);
    free(STr);
    free(STc);
    free(R);
    free(Rr);
    free(Rc);
    for (i=0; i<k; i++) {
      free(P1[i]);
    }
    free(P1);
    for (i=0; i<n; i++) {
      free(P2[i]);
    }
    free(P2);

    // Generate Model & write to file: model.csv
    printf("finalize result\n");
    fp=fopen(OUTPUT, "w");
    finalize_result(M1, Q, M1row, M1col, Qcol, bI, bU,fp);
    fclose(fp);
    printf("8\n");
        
  }
  return 0;
}
