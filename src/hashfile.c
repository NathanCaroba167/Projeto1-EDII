//
// Created by natha on 16/03/2026.
//
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "hashfile.h"

#define CAPACIDADE_BUCKET 3

typedef struct Conteudo{
    int codigo;
}Conteudo;

typedef struct {
    int profundidade_local;
    int contador;
    Conteudo conteudo[CAPACIDADE_BUCKET];
} Bucket;

typedef struct Diretorio{
    int profundidade_global;
    int tamanho_diretorio;
    Bucket** buckets;
} Diretorio;

int funcaoHash(int chave) {
    return chave;
}

int getHashIndex(int valorHash, int profundidade) {
    return  valorHash & ((1 << profundidade) - 1);
}

Bucket* criarBucket(int profundidade) {
    Bucket* b = (Bucket*) malloc(sizeof(Bucket));
    b->profundidade_local = profundidade;
    b->contador = 0;
    return b;
}

Diretorio* criarHash() {
    Diretorio* dir = (Diretorio*) malloc(sizeof(Diretorio));
    dir->profundidade_global = 0;
    dir->tamanho_diretorio = 1;
    dir->buckets = (Bucket**) malloc(sizeof(Bucket*));
    dir->buckets[0] = criarBucket(0);
    return dir;
}

void inserirHash(Diretorio* dir, Conteudo* conteudo) {
    int valorHash = funcaoHash(conteudo->codigo);
    int indice = getHashIndex(valorHash, dir->profundidade_global);
    Bucket* b = dir->buckets[indice];

    if (b-> contador < CAPACIDADE_BUCKET) {
        b->conteudo[b->contador] = *conteudo;
        b->contador++;
        return;
    }

    if (b->profundidade_local == dir->profundidade_global) {
        int tamanho_antigo = dir->tamanho_diretorio;

        dir->tamanho_diretorio *= 2;
        dir->profundidade_global++;

        dir->buckets = (Bucket**)realloc(dir->buckets, sizeof(Bucket*) * dir->tamanho_diretorio);
        if (dir->buckets == NULL) {
            printf("Erro: Falha no realloc do diretório!\n");
            exit(1);
        }

        for (int i = 0; i <tamanho_antigo; i++){
            dir->buckets[i + tamanho_antigo] = dir->buckets[i];
        }
    }

    int profundidade_local_antigo = b->profundidade_local;

    Bucket* novo_bucket = criarBucket(profundidade_local_antigo + 1);
    b->profundidade_local++;

    int mask = 1 << profundidade_local_antigo;

    for (int i = 0; i < dir->tamanho_diretorio; i++) {
        if (dir->buckets[i] == b) {
            if ((i & mask) != 0) {
                dir->buckets[i] = novo_bucket;
            }
        }
    }

    Conteudo conteudo_temp[CAPACIDADE_BUCKET];
    for (int i = 0; i < CAPACIDADE_BUCKET; i++) {
        conteudo_temp[i] = b->conteudo[i];
    }
    b->contador = 0;

    for (int i = 0; i < CAPACIDADE_BUCKET; i++) {
        inserirHash(dir,&conteudo_temp[i]);
    }

    inserirHash(dir,conteudo);

}

bool getHash(Diretorio* dir, int codigo, Conteudo** conteudo) {
    int valorHash = funcaoHash(codigo);
    int indice = getHashIndex(valorHash, dir->profundidade_global);
    Bucket* b = dir->buckets[indice];

    for (int i = 0; i < b->contador; i++) {
        if (b->conteudo[i].codigo == codigo) {
            *conteudo = (Conteudo*) malloc(sizeof(Conteudo));
            **conteudo = b->conteudo[i];
            return true;
        }
    }
    return false;
}

void liberarHash(Diretorio* dir) {
    if (dir == NULL) return;

    for (int i = 0; i < dir->tamanho_diretorio; i++) {
        if (dir->buckets[i] != NULL) {
            Bucket* b_to_free = dir->buckets[i];

            for (int j = i + 1; j < dir->tamanho_diretorio; j++) {
                if (dir->buckets[j] == b_to_free) {
                    dir->buckets[j] = NULL;
                }
            }

            free(b_to_free);
            dir->buckets[i] = NULL;
        }
    }

    free(dir->buckets);

    free(dir);
}

Conteudo* criarConteudo(int codigo) {
    Conteudo* novo = (Conteudo*) malloc(sizeof(Conteudo));
    novo->codigo = codigo;
    return novo;
}

int getProfundidadeGlobal(Diretorio* dir) {
    if(dir == NULL) return -1;
    return dir->profundidade_global;
}

int getTamanhoDiretorio(Diretorio* dir) {
    if(dir == NULL) return -1;
    return dir->tamanho_diretorio;
}

int getContadorBucket(Diretorio* dir, int indice) {
    if(dir == NULL || indice < 0 || indice >= dir->tamanho_diretorio) return -1;
    return dir->buckets[indice]->contador;
}

int getCodigoConteudo(Conteudo* cont) {
    if(cont == NULL) return -1;
    return cont->codigo;
}




