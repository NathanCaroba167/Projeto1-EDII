//
// Created by natha on 21/03/2026.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "../hashfile.h"

void teste_inicializacao() {
    Diretorio* dir = criarHash();
    assert(dir != NULL);
    assert(dir->profundidade_global == 0);
    assert(dir->tamanho_diretorio == 1);
    assert(dir->buckets[0]->profundidade_local == 0);
    assert(dir->buckets[0]->cont == 0);
    liberarHash(dir);
    printf("OK - Inicializacao\n");
}

void teste_insercao() {
    Diretorio* dir = criarHash();
    Conteudo c1 = {10};
    Conteudo c2 = {20};

    inserirHash(dir, c1);
    inserirHash(dir, c2);

    assert(dir->buckets[0]->cont == 2);

    Conteudo achado;
    assert(getHash(dir, 10, &achado) == true);
    assert(achado.id == 10);
    assert(getHash(dir, 99, &achado) == false);

    liberarHash(dir);
    printf("OK - Insercao e Busca\n");

}

void teste_dividir_bucket_and_duplicar_diretorio() {
    HashFile dir = criarHash();

    Conteudo c1 = {1};
    Conteudo c2 = {2};
    Conteudo c3 = {3};
    Conteudo c4 = {4};

    inserirHash(dir, c1);
    inserirHash(dir, c2);
    inserirHash(dir, c3);

    assert(dir->profundidade_global == 0);
    inserirHash(dir,c4);

    assert(dir->profundidade == 1);
    assert(dir->tamanho_diretorio == 2);

    Conteudo achado;
    assert(getHash(dir, 1, &achado) == true);
    assert(getHash(dir, 5, &achado) == true);

    liberarHash(dir);
    printf("OK - Divisao e Duplicacao de Diretorio\n");

}


int main() {
    printf("==========================================\n");
    printf("   TESTE UNITARIO: MODULO HASH FILE\n");
    printf("==========================================\n");

    teste_inicializacao();
    teste_insercao();
    teste_dividir_bucket_and_duplicar_diretorio();

    printf("\n==========================================\n");
    printf("   SUCESSO: TODOS OS TESTES PASSARAM!\n");
    printf("==========================================\n");

    return 0;
}