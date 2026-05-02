//
// Created by natha on 30/03/2026.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include "../unity/unity.h"
#include "../include/habitante.h"

#define BASE_PATH_H "/tmp/t_habitante_hf"

Habitante hab = NULL;
HashFileConfig config = NULL;
HashFile hf = NULL;

void setUp(void) {
    hab = criarHabitante("111.222.333-44", "Joana", "Silva", 'F', "05/01/1975");
    config = criarHashFileConfigHabitantes(4);
    hf = criarHashFile(BASE_PATH_H, config);
}
void tearDown(void) {
    if (hf != NULL) {
        fecharHashFile(hf);
        hf = NULL;
    }

    if (hab != NULL) {
        eliminarHabitante(hab);
        hab = NULL;
    }

    if (config != NULL) {
        eliminarHashFileConfig(config);
        config = NULL;
    }

    remove(BASE_PATH_H ".hf");
    remove(BASE_PATH_H ".hfc");
    remove(BASE_PATH_H ".hfd");
}

void teste_gets_deveRetornarHabitanteCorreto(void) {
    TEST_ASSERT_EQUAL_STRING("111.222.333-44", getCPFHabitante(hab));
    TEST_ASSERT_EQUAL_STRING("Joana", getCPFHabitante(hab));
    TEST_ASSERT_EQUAL_STRING("Silva", getSobrenomeHabitante(hab));
    TEST_ASSERT_EQUAL_INT('F', getSexoHabitante(hab));
    TEST_ASSERT_EQUAL_STRING("10/05/1975", getNascHabitante(hab));
}

void teste_sets_deveAlterarHabitanteCorreto(void) {
    setNomeHabitante(hab, "Roberto");
    TEST_ASSERT_EQUAL_STRING("Roberto", getNomeHabitante(hab));
    setSobrenomeHabitante(hab, "Souza");
    TEST_ASSERT_EQUAL_STRING("Souza", getSobrenomeHabitante(hab));
}

void teste_setEndereco_devePreencherEndereco(void) {
    setEnderecoHabitante(hab, "cep07", 'S', 45, "apto-3");
    TEST_ASSERT_EQUAL_STRING("cep07", getCEPEnderecoHabitante(hab));
    TEST_ASSERT_EQUAL_INT('S', getFaceEnderecoHabitante(hab));
    TEST_ASSERT_EQUAL_INT(45, getNumeroEnderecoHabitante(hab));
    TEST_ASSERT_EQUAL_STRING("apto-3", getComplementoHabitante(hab));
    TEST_ASSERT_TRUE(isMorador(hab));
}

void teste_removerEndereco_deveTornarSemTeto(void) {
    setEnderecoHabitante(hab, "cep07", 'S', 45, "apto-3");
    removerEnderecoHabitante(hab);
    TEST_ASSERT_TRUE(!isMorador(hab));
}

void teste_mudancaDeEndereco_deveSubstituirEnderecoAnterior(void) {
    setEnderecoHabitante(hab, "cep10", 'N', 10, "apto-3");
    setEnderecoHabitante(hab, "cep11", 'L', 11, "apto-4");
    TEST_ASSERT_EQUAL_STRING("cep11", getCEPEnderecoHabitante(hab));
    TEST_ASSERT_EQUAL_INT('L', getFaceEnderecoHabitante(hab));
    TEST_ASSERT_EQUAL_INT(11, getNumeroEnderecoHabitante(hab));
    TEST_ASSERT_EQUAL_STRING("apto-4", getComplementoHabitante(hab));
}

void teste_hf_inserirHabitanteDeveRetornarOk(void) {
    char buffer[512];
    habitanteBufferParaRegistro(hab, buffer);
    TEST_ASSERT_EQUAL_INT(HF_OK, inserirHashFile(hf, buffer));
}

void teste_hf_buscarHabitanteInseridoDeveEncontrar(void) {
    char buffer[512];
    habitanteBufferParaRegistro(hab, buffer);
    inserirHashFile(hf, buffer);

    char buffer2[512];
    char *cpf = getCPFHabitante(hab);
    TEST_ASSERT_EQUAL_INT(HF_OK, buscarHashFile(hf, cpf, buffer2));
}

void teste_hf_buscarRecuperaDadosPessoaisCorretos(void) {
    char buffer[512];
    habitanteBufferParaRegistro(hab, buffer);
    inserirHashFile(hf, buffer);

    char buffer2[512];
    buscarHashFile(hf, getCPFHabitante(hab), buffer2);
    Habitante h2 = registroParaHabitanteBuffer(buffer2);

    TEST_ASSERT_EQUAL_STRING("111.222.333-44", getCPFHabitante(h2));
    TEST_ASSERT_EQUAL_STRING("Joana", getNomeHabitante(h2));
    TEST_ASSERT_EQUAL_STRING("Silva", getSobrenomeHabitante(h2));
    TEST_ASSERT_EQUAL_INT('F', getSexoHabitante(h2));
    TEST_ASSERT_EQUAL_STRING("05/01/1975", getNascHabitante(h2));

    eliminarHabitante(h2);
}

void teste_hf_buscarRecuperaEnderecoCorreto(void) {
    char buffer[512];
    setEnderecoHabitante(hab, "cep05", 'N', 30, "ap-1");
    habitanteBufferParaRegistro(hab, buffer);
    inserirHashFile(hf, buffer);

    char buffer2[512];
    buscarHashFile(hf, getCPFHabitante(hab), buffer2);
    Habitante h2 = registroParaHabitanteBuffer(buffer2);

    TEST_ASSERT_TRUE(isMorador(h2));
    TEST_ASSERT_EQUAL_STRING("cep05", getCEPEnderecoHabitante(h2));
    TEST_ASSERT_EQUAL_INT('N', getFaceEnderecoHabitante(h2));
    TEST_ASSERT_EQUAL_INT(30, getNumeroEnderecoHabitante(h2));
    TEST_ASSERT_EQUAL_STRING("ap-1", getComplementoHabitante(h2));

    eliminarHabitante(h2);
}

int main() {
    UNITY_BEGIN();
    printf("==========================================\n");
    printf("   TESTE UNITARIO: MODULO HABITANTE\n");
    printf("==========================================\n");

    RUN_TEST(teste_gets_deveRetornarHabitanteCorreto);
    RUN_TEST(teste_sets_deveAlterarHabitanteCorreto);
    RUN_TEST(teste_setEndereco_devePreencherEndereco);
    RUN_TEST(teste_removerEndereco_deveTornarSemTeto);
    RUN_TEST(teste_mudancaDeEndereco_deveSubstituirEnderecoAnterior);
    RUN_TEST(teste_hf_buscarHabitanteInseridoDeveEncontrar);
    RUN_TEST(teste_hf_buscarRecuperaDadosPessoaisCorretos);
    RUN_TEST(teste_hf_buscarRecuperaEnderecoCorreto);
    RUN_TEST(teste_hf_inserirHabitanteDeveRetornarOk);

    printf("\n==========================================\n");
    printf("   SUCESSO: TODOS OS TESTES PASSARAM!\n");
    printf("==========================================\n");

    return UNITY_END();
}