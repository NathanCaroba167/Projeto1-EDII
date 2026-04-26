//
// Created by natha on 01/04/2026.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <math.h>
#include <errno.h>

#include "../include/quadra.h"

#define CEP_MAX 10
#define COR_MAX 10

typedef struct {
    char cep[CEP_MAX];
    double x;
    double y;
    double w;
    double h;
    char corB[COR_MAX];
    char corP[COR_MAX];
    double sw;
}quadra;

Quadra criarQuadra(char* cep,double x,double y,double w,double h) {
    quadra* q = (quadra*)malloc(sizeof(quadra));

    if(q == NULL) {
        printf("Erro ao alocar memória ao criarQuadra!\n");

        perror("Motivo do erro");
        exit(1);
    }

    strncpy(q->cep,cep, CEP_MAX - 1);
    q->cep[CEP_MAX - 1] = '\0';

    q->x = x;
    q->y = y;
    q->w = w;
    q->h = h;

    q->corP[0] = '\0';
    q->corB[0] = '\0';

    q->sw = 1.0;

    return q;
}

char* getCEPQuadra(Quadra q) {
    return ((quadra*) q)->cep;

}

void setCEPQuadra(Quadra q,char* cep) {
    quadra* quadr = (quadra*)q;
    strncpy(quadr->cep,cep, CEP_MAX - 1);
    quadr->cep[CEP_MAX - 1] = '\0';
}

double getXQuadra(Quadra q) {
    return ((quadra*)q)->x;

}

void setXQuadra(Quadra q,double x) {
    ((quadra*) q)->x = x;

}

double getYQuadra(Quadra q) {
    return ((quadra*)q)->y;

}

void setYQuadra(Quadra q,double y) {
    ((quadra*) q)->y = y;

}

double getWQuadra(Quadra q) {
    return ((quadra*)q)->w;

}

void setWQuadra(Quadra q, double w) {
    ((quadra*) q)->w = w;

}

double getHQuadra(Quadra q) {
    return ((quadra*)q)->h;

}

void setHQuadra(Quadra q,double h) {
    ((quadra*) q)->h = h;

}

char* getCorBQuadra(Quadra q) {
    return ((quadra*)q)->corB;

}

void setCorBQuadra(Quadra q,char* corB) {
    quadra* quadr = (quadra*)q;
    strncpy(quadr->corB,corB, COR_MAX - 1);
    quadr->corB[COR_MAX - 1] = '\0';

}

char* getCorPQuadra(Quadra q) {
    return ((quadra*)q)->corP;

}

void setCorPQuadra(Quadra q,char* corP) {
    quadra* quadr = (quadra*)q;
    strncpy(quadr->corP,corP, COR_MAX - 1);
    quadr->corP[COR_MAX - 1] = '\0';

}

double getSWQuadra(Quadra q) {
    return ((quadra*)q)->sw;

}

void setSWQuadra(Quadra q, double sw) {
    ((quadra*) q)->sw = sw;
}

void formatoQuadra(void* registro, char* buffer, size_t tamanhoBuffer) {
    quadra* q = (quadra*)registro;
    snprintf(buffer, tamanhoBuffer, "%s | %.2f | %.2f | %.2f | %.2f | %.2f | %s | %s ",q->cep, q->x, q->y, q->w, q->h, q->sw, q->corP, q->corB);

}

HashFileConfig criarHashFileConfigQuadras(int capacidadeBucket) {
    return criarHashFileConfig(sizeof(quadra), offsetof(quadra,cep), sizeof(((quadra*)0)->cep), capacidadeBucket, formatoQuadra);
}

Quadra registroParaQuadraBuffer(void* buffer) {
    quadra* quadraBuffer = calloc(1, sizeof(quadra));
    if(quadraBuffer == NULL) {
        return NULL;
    }

    memcpy(quadraBuffer, buffer, sizeof(quadra));

    return quadraBuffer;
}

void eliminarQuadra(Quadra q) {
    quadra* quadr = (quadra*)q;
    // Libera a struct principal
    free(quadr);

}


