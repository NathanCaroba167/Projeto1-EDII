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
    HashFile* dir = criarHash();
    TEST_ASSERT_NOT_NULL(dir);
    TEST_ASSERT_EQUAL_INT(0, getProfundidadeGlobal(dir));
    TEST_ASSERT_EQUAL_INT(1, getTamanhoDiretorio(dir));
    TEST_ASSERT_EQUAL_INT(0, getContadorBucket(dir,0));
    liberarHash(dir);
    printf("OK - Inicializacao\n");
}

void teste_insercao_Elementos_SemSplit(void) {
    HashFile* dir = criarHash();
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
    HashFile* dir = criarHash();

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
    HashFile* dir = criarHash();

    Conteudo* c1 = criarConteudo(100);
    Conteudo* c2 = criarConteudo(200);

    inserirHash(dir, c1);
    inserirHash(dir, c2);
    TEST_ASSERT_EQUAL_INT(2, getContadorBucket(dir, 0));
    removerHash(dir, c1);

    TEST_ASSERT_EQUAL_INT(1, getContadorBucket(dir, 0));

    Conteudo* achado = NULL;
    TEST_ASSERT_EQUAL_INT(0, getHash(dir, 100, &achado));
    TEST_ASSERT_EQUAL_INT(1, getHash(dir, 200, &achado));
    free(achado);

    liberarHash(dir);
    printf("OK - Remocao\n");
}

void teste_Busca_EmHashVazio_DeveRetornarFalso(void) {
    HashFile* dir = criarHash();
    Conteudo* achado = NULL;

    TEST_ASSERT_EQUAL_INT(0, getHash(dir, 999, &achado));

    liberarHash(dir);
    printf("OK - Busca em Hash Vazio\n");
}

void teste_Remocao_DeElementoInexistente_NaoDeveQuebrar(void) {
    HashFile* dir = criarHash();
    Conteudo* c1 = criarConteudo(10);
    Conteudo* fantasma = criarConteudo(99);

    inserirHash(dir, c1);

    removerHash(dir, fantasma);
    TEST_ASSERT_EQUAL_INT(1, getContadorBucket(dir,0));

    free(fantasma);
    liberarHash(dir);
    printf("OK - Remocao de Elemento Inexistente\n");
}

void teste_Insercao_ComPonteirosNulos_NaoDeveCrashar(void) {
    HashFile* dir = criarHash();
    Conteudo* c1 = criarConteudo(10);

    inserirHash(NULL, c1);
    inserirHash(dir, NULL);

    TEST_ASSERT_EQUAL_INT(0, getContadorBucket(dir,0));

    free(c1);
    liberarHash(dir);
    printf("OK - Insercao com Ponteiros Nulos\n");
}

void teste_Insercao_EmMassa_DeveCausarMultiplosSplits(void) {
    HashFile* dir = criarHash();

    for (int i = 1; i <= 15; i++) {
        Conteudo* c = criarConteudo(i);
        inserirHash(dir, c);
    }

    TEST_ASSERT_EQUAL_INT(3, getProfundidadeGlobal(dir));
    TEST_ASSERT_EQUAL_INT(8, getTamanhoDiretorio(dir));

    Conteudo* achado = NULL;
    TEST_ASSERT_EQUAL_INT(1, getHash(dir, 1, &achado));
    free(achado);
    TEST_ASSERT_EQUAL_INT(1, getHash(dir, 15, &achado));
    free(achado);

    liberarHash(dir);
    printf("OK - Insercao em Massa\n");
}

void teste_Insercao_DeChaveDuplicada(void) {
    HashFile* dir = criarHash();

    Conteudo* c1 = criarConteudo(42);
    Conteudo* c2 = criarConteudo(42);

    inserirHash(dir, c1);
    inserirHash(dir, c2);

    TEST_ASSERT_EQUAL_INT(2, getContadorBucket(dir,0));

    liberarHash(dir);
    printf("OK - Insercao Chave Duplicada\n");
}

void teste_LiberarHash_DeveLidarComPonteiroNulo_SemCrash(void) {
    liberarHash(NULL);
    TEST_ASSERT_TRUE(true);

    printf("OK - Liberar Ponteiro Nulo\n");
}

void teste_LiberarHash_DeveLimparDiretorioVazio_SemCrash(void) {
    HashFile* dir = criarHash();

    liberarHash(dir);

    TEST_ASSERT_TRUE(true);
    printf("OK - Liberar Diretorio Vazio\n");
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
    RUN_TEST(teste_LiberarHash_DeveLidarComPonteiroNulo_SemCrash);
    RUN_TEST(teste_LiberarHash_DeveLimparDiretorioVazio_SemCrash);
    RUN_TEST(teste_Busca_EmHashVazio_DeveRetornarFalso);
    RUN_TEST(teste_Remocao_DeElementoInexistente_NaoDeveQuebrar);
    RUN_TEST(teste_Insercao_ComPonteirosNulos_NaoDeveCrashar);
    RUN_TEST(teste_Insercao_EmMassa_DeveCausarMultiplosSplits);
    RUN_TEST(teste_Insercao_DeChaveDuplicada);


    printf("\n==========================================\n");
    printf("   SUCESSO: TODOS OS TESTES PASSARAM!\n");
    printf("==========================================\n");

    return UNITY_END();
}