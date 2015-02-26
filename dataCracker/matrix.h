//
//  matrix.h
//  dataCracker
//
//  Created by Yanbin Ren on 2/24/15.
//  Copyright (c) 2015 Data Crackers. All rights reserved.
//

#ifndef dataCracker_matrix_h
#define dataCracker_matrix_h

#endif


#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void matrix_add(double** A, double** B, double** ans,int m,int n);
void matrix_substract(double** A, double** B, double** ans,int m,int n);
void matrix_times(double c, double** A, double** ans, int m,int n);
void matrix_multiply(double** A, double** B, double** ans,int m,int k,int n);
void write_matrix_to_file(double **A,int m,int n);
void read_matrix_from_file(double **A,int m,int n,FILE *fp);
int get_column(FILE *fp);
int get_row(char* s);
char* readline(FILE* f);
char* read_next_token(char *line,int index);
void matrix_transpose(double** A, double** B,int m,int n);
void random_initialize(double **A,int m,int n);
void matrix_multiply1(double** A, double** B, double** ans,int m,int k,int n);
void matrix_multiply2(double** A, double** B, double** ans,int m,int k,int n);
void matrix_multiply3(double** A, double** B, double** ans,int m,int k,int n);


// ans=A+B
void matrix_add(double** A, double** B, double** ans,int m,int n){
    int i,j;
    for (i=0; i<m; i++) {
        for (j=0; j<n; j++) {
            ans[i][j]=A[i][j]+B[i][j];
        }
    }
}

// ans=A-B
void matrix_substract(double** A, double** B, double** ans,int m,int n){
    int i,j;
    for (i=0; i<m; i++) {
        for (j=0; j<n; j++) {
            ans[i][j]=A[i][j]-B[i][j];
        }
    }
}

// ans=c*A
void matrix_times(double c, double** A, double** ans, int m,int n){
    int i,j;
    for (i=0; i<m; i++) {
        for (j=0; j<n; j++) {
            ans[i][j]=A[i][j]*c;
        }
    }
}


// ans=A*B
// M*K  *  K*N  =   M*N
void matrix_multiply(double** A, double** B, double** ans,int m,int k,int n){
    int i1,i2;
    for (i1=0; i1<m; i1++) {
        for (i2=0; i2<n; i2++) {
            int tmp=0;
            int j1;
            for (j1=0; j1<k; j1++) {
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
            fputc('\t',fp);
        }
        fputc('\n', fp);
    }
}



// '\t' seperated
void read_matrix_from_file(double **A,int m,int n,FILE *fp){
    char *line;
    int i=0;
    int j=0;
    while ((line=readline(fp))) {
        int index=0;
        for (j=0; j<n; j++) {
            char *p=read_next_token(line, index);
            index+=(1+strlen(p));
            A[i][j]=atof(p);
        }
        i++;
    }
}


//initialize matrix with random 0/1
void random_initialize(double **A,int m,int n){
    srand((unsigned)time(NULL));
    int i,j;
    for (i=0; i<m; i++) {
        for (j=0; j<n; j++) {
            A[i][j]=RAND_MAX/rand();
        }
    }
}


int get_column(FILE *fp){
    int column=1;
    char *line=readline(fp);
    while (*line!='\0' && *line!='\n' && *line != '\r' && *line !=EOF) {
        if(*line=='\t') column++;
        line++;
    }
    return column;
}

int get_row(char *filename){
    FILE * fp;
    char buffer[10];
    char *s=(char *)malloc((6+1+strlen(filename))*sizeof(char));
    s[strlen(s)-1]='\0';
    s[0]='w';
    s[1]='c';
    s[2]=' ';
    s[3]='-';
    s[4]='l';
    s[5]=' ';
    int i;
    for (i=0; i<strlen(filename); i++) {
        s[i+6]=filename[i];
    }
    fp=popen(s,"r");
    fgets(buffer,sizeof(buffer),fp);
    pclose(fp);
    int row= atoi(buffer);
    return row;
}

char* readline(FILE* f)
{
    char* line = (char*) calloc(1, sizeof(char) );
    char c;
    int len = 0;
    while ( (c = fgetc(f) ) != EOF && c != '\n')
    {
        line = (char*) realloc(line, sizeof(char) * (len + 2) );
        line[len++] = c;
        line[len] = '\0';
    }
    return line;
}

char* read_next_token(char *line,int index){
    char* token = (char*) calloc(1, sizeof(char) );
    int len=0;
    while (line[index]!='\0' && line[index]!='\t') {
        token = (char*) realloc(token, sizeof(char) * (len + 2) );
        token[len++] = line[index++];
        token[len] = '\0';
    }
    return token;
}

// Transpose Matrix A -> B,  B = (A)T, A = m*n, B= n*m
void matrix_transpose(double** A, double** B,int m,int n){
    int i,j;
    for (i=0; i<m; ++i) {
        for (j=0; j<n; ++j) {
            B[j][i]=A[i][j];
        }
    }
}

// ans=A*BT
void matrix_multiply1(double** A, double** B, double** ans,int m,int k,int n){
    int i1,i2;
    for (i1=0; i1<m; i1++) {
        for (i2=0; i2<n; i2++) {
            int tmp=0;
            int j1;
            for (j1=0; j1<k; j1++) {
                tmp+=A[i1][j1]*B[i2][j1];
            }
            ans[i1][i2]=tmp;
        }
    }
}

// ans=AT*B
void matrix_multiply2(double** A, double** B, double** ans,int m,int k,int n){
    int i1,i2;
    for (i1=0; i1<m; i1++) {
        for (i2=0; i2<n; i2++) {
            int tmp=0;
            int j1;
            for (j1=0; j1<k; j1++) {
                tmp+=A[j1][i1]*B[j1][i2];
            }
            ans[i1][i2]=tmp;
        }
    }
}

// ans = AT *BT
void matrix_multiply3(double** A, double** B, double** ans,int m,int k,int n){
    int i1,i2;
    for (i1=0; i1<m; i1++) {
        for (i2=0; i2<n; i2++) {
            int tmp=0;
            int j1;
            for (j1=0; j1<k; j1++) {
                tmp+=A[j1][i1]*B[i2][j1];
            }
            ans[i1][i2]=tmp;
        }
    }
}


