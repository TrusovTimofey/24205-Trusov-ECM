#include <math.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <cblas.h>

typedef struct {
    size_t size;
    float* data;
} Matrix;

Matrix* newMatrix(size_t size) {
    Matrix* m = malloc(sizeof(Matrix));
    if(!m) return NULL;
    
    m->size = size;
    m->data = calloc(size * size, sizeof(float));
    
    if(!m->data){
        free(m);
        if(!m) return NULL;
    }
    
    return m;
}

void deleteMatrix(Matrix* m) {
    if (m) {
        free(m->data);
        free(m);
    }
}

void fillRandom(Matrix* m) {
    const size_t n = m->size * m->size;

    for (size_t i = 0; i < n; ++i) {
        m->data[i] = ((float)rand()) / RAND_MAX;
    }
}

float* at(Matrix* m, size_t x, size_t y){
    return &m->data[y * m->size + x];
}

void swap(float* x, float* y){
    float temp = *x;
    *x=*y;
    *y=temp;
}


void transpose(Matrix* m) {
    for (size_t y = 1; y < m->size; ++y)
    {
        for (size_t x = 0; x < y; x++)
        {
            swap(at(m,x,y), at(m,y,x));
        } 
    }
    
}

Matrix* newIdentity(size_t size) {
    Matrix* m = newMatrix(size);
    for(size_t i=0; i < size; ++i) *at(m,i,i)=1.f;
    return m;
}

float normOne(const Matrix* m) {
    float res = 0.0f;

    for (size_t y = 0; y < m->size; ++y) {
        const float* row = m->data + y * m->size;
        float sum = 0.0f;

        for (size_t x = 0; x < m->size; ++x) sum += fabsf(row[x]);
        if (sum > res) res = sum;
    }

    return res;
}

float normInf(const Matrix* m) {
    float res = 0.0f;
    const size_t n = m->size * m->size;

    for (size_t col = 0; col < m->size; ++col) {
        float sum = 0.0f;

        for(size_t index = col; index < n; index+=m->size) sum+= m->data[index];
        if (sum > res) res = sum;
    }

    return res;
}


void copyData(const Matrix* from, Matrix* to) {
    memcpy(to->data,from->data,to->size*to->size*sizeof(float));
}

void add(Matrix* m, const Matrix* operand) {
    cblas_saxpy(m->size * m->size, 1, operand->data, 1, m->data, 1);
}

void sub(Matrix* m, const Matrix* operand) {
    cblas_saxpy(m->size * m->size, -1, operand->data, 1, m->data, 1);
}

void mulOn(Matrix* m, float scalar) {
    cblas_sscal(m->size * m->size, scalar, m->data, 1);
}

void mul(Matrix* m, const Matrix* operand) {
    float* res = malloc(m->size * m->size * sizeof(float));

    cblas_sgemm(
        CblasRowMajor,
        CblasNoTrans,
        CblasNoTrans,
        m->size,
        m->size,
        m->size,
        1.0f,
        m->data,
        m->size,
        operand->data,
        m->size,
        0.0f,
        res,
        m->size
    );

    free(m->data);
    m->data=res;
}

void inverse(Matrix* A, size_t approximation) {
    Matrix* B = newMatrix(A->size);
    copyData(A,B);
    transpose(B);
    mulOn(B,1/(normOne(A)*normInf(A)));

    Matrix* BA = newMatrix(A->size);
    copyData(B,BA);
    mul(BA,A);
    
    Matrix* R = newIdentity(A->size);
    sub(R,BA);

    copyData(R,BA);

    deleteMatrix(A);
    A=newIdentity(B->size);

    for (size_t i = 0; i < approximation; i++)
    {
        add(A,BA);
        mul(BA,R);
    }

    mul(A,B);

    deleteMatrix(B);
    deleteMatrix(BA);
    deleteMatrix(R);
}

void printM(Matrix* m) {
    for (size_t y = 0; y < m->size; y++)
    {
        for (size_t x = 0; x < m->size; x++)
        {
            printf(" %6.3f",*at(m,x,y));
        }
        printf("\n");
    }
    printf("\n");
}

void test(size_t aprox) {
    Matrix* m = newMatrix(3);
    float data[9] = {
        1.f, 2.f, 3.f,
        0.f, 1.f, 4.f,
        0.f, 0.f, 1.f,};
    memcpy(m->data,&data,9*4);
    
    printf("matrix:\n");
    printM(m);

    inverse(m,aprox);
    printf("inversed:\n");
    printM(m);

    printf("expected:\n 1 -2  5\n 0  1 -4\n 0  0  1\n");
}

int main(void) {
    //test(1000000);
    
    srand((unsigned int)time(NULL));
    Matrix* m = newMatrix(2048);
    fillRandom(m);

    time_t strt, end;

    time(&strt);
    inverse(m, 10);
    time(&end);

    printf("time: %ld s\n", end - strt);

    deleteMatrix(m);

    return 0;
}
