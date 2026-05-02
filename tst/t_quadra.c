//
// Created by natha on 06/04/2026.
//
#include <string.h>
#include <stdio.h>
#include "../unity/unity.h"

#include "../include/quadra.h"

#define BASE_PATH_Q "/tmp/t_quadra_hf"

Quadra qdr = NULL;
HashFileConfig config = NULL;
HashFile hf = NULL;

void setUp(void) {
    qdr = criarQuadra("cep15", 10.0, 20.0, 50.0, 30.0);
    config = criarHashFileConfigQuadras(4);
    hf  = criarHashFile(BASE_PATH_Q, config);
}
void tearDown(void) {
    if (hf)  { fecharHashFile(hf); hf  = NULL; }
    if (qdr) { eliminarQuadra(qdr); qdr = NULL; }
    if (config) { eliminarHashFileConfig(config); config = NULL; }
    remove(BASE_PATH_Q ".hf");
    remove(BASE_PATH_Q ".hfc");
    remove(BASE_PATH_Q ".hfd");
}

void teste_gets_deveRetornarDadosCorretos(void) {
    TEST_ASSERT_EQUAL_STRING("cep15", getCEPQuadra(qdr));
    TEST_ASSERT_TRUE(getXQuadra(qdr) == 10.0);
    TEST_ASSERT_TRUE(getYQuadra(qdr) == 20.0);
    TEST_ASSERT_TRUE(getWQuadra(qdr) == 50.0);
    TEST_ASSERT_TRUE(getHQuadra(qdr) == 30.0);
    TEST_ASSERT_TRUE(getSWQuadra(qdr) == 1.0);
    TEST_ASSERT_EQUAL_STRING("", getCorPQuadra(qdr));
    TEST_ASSERT_EQUAL_STRING("", getCorBQuadra(qdr));
}

void teste_sets_deveAlterarCoresSWCorretamente(void) {
    setCorPQuadra(qdr, "blue");
    TEST_ASSERT_EQUAL_STRING("blue", getCorPQuadra(qdr));
    setCorBQuadra(qdr, "red");
    TEST_ASSERT_EQUAL_STRING("red", getCorBQuadra(qdr));
    setSWQuadra(qdr, 2.5);
    TEST_ASSERT_TRUE(getSWQuadra(qdr) == 2.5);
}

void teste_paraRegistro_devePreencherBuffer(void) {
    char buffer[512];
    memset(buffer, 0, sizeof(buffer));
    setCorPQuadra(qdr, "cyan");
    setCorBQuadra(qdr, "magenta");
    setSWQuadra(qdr, 3.0);

    quadraBufferParaRegistro(qdr, buffer);

    Quadra q2 = registroParaQuadraBuffer(buffer);
    TEST_ASSERT_NOT_NULL(q2);

    TEST_ASSERT_EQUAL_STRING("cep15", getCEPQuadra(q2));
    TEST_ASSERT_TRUE(getXQuadra(q2) == 10.0);
    TEST_ASSERT_TRUE(getYQuadra(q2) == 20.0);
    TEST_ASSERT_TRUE(getWQuadra(q2) == 50.0);
    TEST_ASSERT_TRUE(getHQuadra(q2) == 30.0);
    TEST_ASSERT_TRUE(getSWQuadra(q2) == 3.0);
    TEST_ASSERT_EQUAL_STRING("cyan",    getCorPQuadra(q2));
    TEST_ASSERT_EQUAL_STRING("magenta", getCorBQuadra(q2));

    eliminarQuadra(q2);
}

void teste_hf_inserirQuadraDeveRetornarOk(void) {
    char buffer[512];
    setCorPQuadra(qdr, "grey");
    setCorBQuadra(qdr, "black");
    quadraBufferParaRegistro(qdr, buffer);
    TEST_ASSERT_EQUAL_INT(HF_OK, inserirHashFile(hf, buffer));
}

void teste_hf_buscarQuadraInseridaDeveEncontrar(void) {
    char buffer[512];
    quadraBufferParaRegistro(qdr, buffer);
    inserirHashFile(hf, buffer);

    char buffer2[512];
    char *cep = getCEPQuadra(qdr);
    TEST_ASSERT_EQUAL_INT(HF_OK, buscarHashFile(hf, cep, buffer2));
}

void teste_hf_buscarQuadraRecuperaDadosCorretos(void) {
    char buffer[512];
    setCorPQuadra(qdr, "blue");
    setCorBQuadra(qdr, "red");
    setSWQuadra(qdr, 2.0);
    quadraBufferParaRegistro(qdr, buffer);
    inserirHashFile(hf, buffer);

    char buffer2[512];
    buscarHashFile(hf, getCEPQuadra(qdr), buffer2);
    Quadra q2 = registroParaQuadraBuffer(buffer2);

    TEST_ASSERT_EQUAL_STRING("cep15", getCEPQuadra(q2));
    TEST_ASSERT_EQUAL_STRING("blue",  getCorPQuadra(q2));
    TEST_ASSERT_EQUAL_STRING("red",   getCorBQuadra(q2));
    TEST_ASSERT_TRUE(getSWQuadra(q2) == 2.0);

    eliminarQuadra(q2);
}

void teste_hf_removerQuadraDeveRetornarOk(void) {
    char buffer[512];
    quadraBufferParaRegistro(qdr, buffer);
    inserirHashFile(hf, buffer);

    char *cep = getCEPQuadra(qdr);
    TEST_ASSERT_EQUAL_INT(HF_OK, removerHashFile(hf, cep, NULL));
}

void teste_hf_quadraRemovidaNaoDeveSerEncontrada(void) {
    char buffer[512];
    quadraBufferParaRegistro(qdr, buffer);
    inserirHashFile(hf, buffer);

    char *cep = getCEPQuadra(qdr);
    removerHashFile(hf, cep, NULL);
    TEST_ASSERT_EQUAL_INT(HF_NAO_ENCONTRADO, buscarHashFile(hf, cep, NULL));
}

void teste_hf_atualizarQuadraDeveAlterarDados(void) {
    char buffer[512];
    quadraBufferParaRegistro(qdr, buffer);
    inserirHashFile(hf, buffer);

    setCorPQuadra(qdr, "purple");
    quadraBufferParaRegistro(qdr, buffer);
    TEST_ASSERT_EQUAL_INT(HF_OK, atualizarHashFile(hf, buffer));

    char buffer2[512];
    buscarHashFile(hf, getCEPQuadra(qdr), buffer2);
    Quadra q2 = registroParaQuadraBuffer(buffer2);
    TEST_ASSERT_EQUAL_STRING("purple", getCorPQuadra(q2));
    eliminarQuadra(q2);
}

void teste_hf_multiplosInsertsEBuscas(void) {
    char buffer[512];
    char buffer2[512];
    const char *ceps[] = { "cep01", "cep02", "cep03", "cep05", "cep07" };
    int n = 5;

    for (int i = 0; i < n; i++) {
        Quadra q = criarQuadra(ceps[i], (double)i, (double)i, 10.0, 10.0);
        quadraBufferParaRegistro(q, buffer);
        inserirHashFile(hf, buffer);
        eliminarQuadra(q);
    }

    for (int i = 0; i < n; i++) {
        TEST_ASSERT_EQUAL_INT(HF_OK, buscarHashFile(hf, ceps[i], buffer2));
        Quadra q2 = registroParaQuadraBuffer(buffer2);
        TEST_ASSERT_EQUAL_STRING(ceps[i], getCEPQuadra(q2));
        eliminarQuadra(q2);
    }
}

void teste_hf_persistenciaAposReabrirHashFile(void) {
    char buffer[512];
    setCorPQuadra(qdr, "orange");
    quadraBufferParaRegistro(qdr, buffer);
    inserirHashFile(hf, buffer);

    fecharHashFile(hf);
    hf = abrirHashFile(BASE_PATH_Q, config);
    TEST_ASSERT_NOT_NULL(hf);

    char buffer2[512];
    TEST_ASSERT_EQUAL_INT(HF_OK, buscarHashFile(hf, getCEPQuadra(qdr), buffer2));
    Quadra q2 = registroParaQuadraBuffer(buffer2);
    TEST_ASSERT_EQUAL_STRING("orange", getCorPQuadra(q2));
    eliminarQuadra(q2);
}

int main() {
    UNITY_BEGIN();
    printf("==========================================\n");
    printf("   TESTE UNITARIO: MODULO QUADRA\n");
    printf("==========================================\n");

    RUN_TEST(teste_gets_deveRetornarDadosCorretos);
    RUN_TEST(teste_sets_deveAlterarCoresSWCorretamente);

    RUN_TEST(teste_paraRegistro_devePreencherBuffer);

    RUN_TEST(teste_hf_inserirQuadraDeveRetornarOk);
    RUN_TEST(teste_hf_buscarQuadraInseridaDeveEncontrar);
    RUN_TEST(teste_hf_buscarQuadraRecuperaDadosCorretos);
    RUN_TEST(teste_hf_removerQuadraDeveRetornarOk);
    RUN_TEST(teste_hf_quadraRemovidaNaoDeveSerEncontrada);
    RUN_TEST(teste_hf_atualizarQuadraDeveAlterarDados);
    RUN_TEST(teste_hf_multiplosInsertsEBuscas);
    RUN_TEST(teste_hf_persistenciaAposReabrirHashFile);

    printf("\n==========================================\n");
    printf("   SUCESSO: TODOS OS TESTES PASSARAM!\n");
    printf("==========================================\n");

    return UNITY_END();
}