//
// Created by natha on 06/04/2026.
//

#include <stdlib.h>

#include "../unity/unity.h"

#include "../include/arquivos.h"

void setUp(void){}
void tearDown(void) {}

void teste_getNomeBase_caminhoComExtensao(void) {
    char* nome = getNomeBase("testes/t001.geo");
    TEST_ASSERT_EQUAL_STRING("t001",nome);
    free(nome);
}

void teste_getNomeBase_semExtensao(void) {
    char* nome = getNomeBase("arquivo");
    TEST_ASSERT_EQUAL_STRING("arquivo",nome);
    free(nome);
}

void teste_getNomeBase_caminhoTotal(void) {
    char* nome = getNomeBase("/home/aluno/ed2/testes/cidade.geo");
    TEST_ASSERT_EQUAL_STRING("cidade",nome);
    free(nome);
}

void teste_getNomeBase_extensaoPm(void) {
    char* nome = getNomeBase("pessoas.pm");
    TEST_ASSERT_EQUAL_STRING("pessoas",nome);
    free(nome);
}

void teste_getNomeBase_extensaoQry(void) {
    char* nome = getNomeBase("consultas.qry");
    TEST_ASSERT_EQUAL_STRING("consultas",nome);
    free(nome);
}

void teste_getNomeBase_retornoNaoNulo(void) {
    char* nome = getNomeBase("qualquer.geo");
    TEST_ASSERT_NOT_NULL(nome);
    free(nome);
}

int main() {
    UNITY_BEGIN();
    printf("==========================================\n");
    printf("   TESTE UNITARIO: MODULO ARQUIVOS\n");
    printf("==========================================\n");

    RUN_TEST(teste_getNomeBase_caminhoComExtensao);
    RUN_TEST(teste_getNomeBase_semExtensao);
    RUN_TEST(teste_getNomeBase_caminhoTotal);
    RUN_TEST(teste_getNomeBase_extensaoPm);
    RUN_TEST(teste_getNomeBase_extensaoQry);
    RUN_TEST(teste_getNomeBase_retornoNaoNulo);

    printf("\n==========================================\n");
    printf("   SUCESSO: TODOS OS TESTES PASSARAM!\n");
    printf("==========================================\n");

    return UNITY_END();
}