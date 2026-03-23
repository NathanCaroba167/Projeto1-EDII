//
// Created by natha on 21/03/2026.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "../unity/unity.h"

#include "../include/hashfile.h"

void setUp(void){}
void tearDown(void) {}

void teste_inicializacao_DeveCriarDiretorioVazio(void) {
    Diretorio* dir = criarHash();
    TEST_ASSERT_NOT_NULL(dir);
    TEST_ASSERT_EQUAL_INT(0, getProfundidadeGlobal(dir));
    TEST_ASSERT_EQUAL_INT(1, getTamanhoDiretorio(dir));
    TEST_ASSERT_EQUAL_INT(0, getContadorBucket(dir,0));
    liberarHash(dir);
    printf("OK - Inicializacao\n");
}

void teste_insercao_Elementos_SemSplit(void) {
    Diretorio* dir = criarHash();
    Conteudo* c1 = criarConteudo(10);
    Conteudo* c2 = criarConteudo(20);

    inserirHash(dir, c1);
    inserirHash(dir, c2);

    TEST_ASSERT_EQUAL_INT(2, getContadorBucket(dir, 0));

    Conteudo* achado = NULL;
    TEST_ASSERT_EQUAL_INT(1, getHash(dir, 10, &achado));
    TEST_ASSERT_NOT_NULL(achado);
    TEST_ASSERT_EQUAL_INT(10, getCodigoConteudo(achado));
    free(achado);
    TEST_ASSERT_EQUAL_INT(0, getHash(dir, 99, &achado));

    liberarHash(dir);
    printf("OK - Insercao e Busca\n");

}

void teste_insercao_DeveDividirBucket_E_DuplicarDiretorio(void) {
    Diretorio* dir = criarHash();

    Conteudo* c1 = criarConteudo(1);
    Conteudo* c2 = criarConteudo(2);
    Conteudo* c3 = criarConteudo(3);
    Conteudo* c4 = criarConteudo(4);

    inserirHash(dir, c1);
    inserirHash(dir, c2);
    inserirHash(dir, c3);

    TEST_ASSERT_EQUAL_INT(0, getProfundidadeGlobal(dir));
    inserirHash(dir,c4);

    TEST_ASSERT_EQUAL_INT(1, getProfundidadeGlobal(dir));
    TEST_ASSERT_EQUAL_INT(2, getTamanhoDiretorio(dir));

    Conteudo* achado = NULL;
    TEST_ASSERT_EQUAL_INT(1, getHash(dir, 1, &achado));
    free(achado);

    TEST_ASSERT_EQUAL_INT(1, getHash(dir, 4, &achado));
    free(achado);

    liberarHash(dir);
    printf("OK - Divisao e Duplicacao de Diretorio\n");

}

void teste_remover_DeveRemoverConteudo(void) {
    Diretorio* dir = criarHash();

    Conteudo* c1 = criarConteudo(1);

    inserirHash(dir, c1);
    removerHash(dir, c1);
    TEST_ASSERT_EQUAL_INT(1, getProfundidadeGlobal(dir));

    liberarHash(dir);
    printf("OK - Remocao\n");

}

int main() {
    UNITY_BEGIN();
    printf("==========================================\n");
    printf("   TESTE UNITARIO: MODULO HASH FILE\n");
    printf("==========================================\n");

    RUN_TEST(teste_inicializacao_DeveCriarDiretorioVazio);
    RUN_TEST(teste_insercao_Elementos_SemSplit);
    RUN_TEST(teste_insercao_DeveDividirBucket_E_DuplicarDiretorio);
    RUN_TEST(teste_remover_DeveRemoverConteudo);

    printf("\n==========================================\n");
    printf("   SUCESSO: TODOS OS TESTES PASSARAM!\n");
    printf("==========================================\n");

    return UNITY_END();
}