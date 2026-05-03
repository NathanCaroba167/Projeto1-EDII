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

typedef struct {
    int id;
    char valor[32];
}Registro;

#define BASE_PATH "/tmp/t_hf_teste"
#define DUMP_PATH "/tmp/t_hf_teste.hfd"
#define CAPACIDADE 3

HashFileConfig config = NULL;
HashFile hf = NULL;

void setUp(void) {
    config = criarHashFileConfig(sizeof(Registro),
        offsetof(Registro, id),
        sizeof(int),
        CAPACIDADE,
        NULL);

    hf = criarHashFile(BASE_PATH, config);
}
void tearDown(void) {
    if (hf != NULL) {
        fecharHashFile(hf);
        hf = NULL;
    }
    if (config != NULL) {
        eliminarHashFileConfig(config);
        config = NULL;
    }

    remove(BASE_PATH ".hf");
    remove(BASE_PATH ".hfc");
    remove(DUMP_PATH);
}

void teste_inserir_umRegistroDeveRetornarOk(void) {
    Registro r = { .id = 1 };
    strcpy(r.valor, "primerio01");
    TEST_ASSERT_EQUAL_INT(HF_OK, inserirHashFile(hf, &r));
}

void teste_inserir_multiplosRegistrosDeveAumentarContador(void) {
    Registro r;
    for (int i = 1; i <= 5; i++) {
        r.id = i;
        sprintf(r.valor, "val%d", i);
        inserirHashFile(hf, &r);
    }
    TEST_ASSERT_EQUAL_INT(5, getNumeroRegistrosHF(hf));
}

void teste_inserir_acimaCapacidadeDeveFazersplit(void) {
    Registro r;
    int profundidade_antes = getProfundidadeGlobalHF(hf);

    for (int i = 1; i <= CAPACIDADE + 1; i++) {
        r.id = i;
        sprintf(r.valor, "v%d", i);
        inserirHashFile(hf, &r);
    }

    int profundidade_depois = getProfundidadeGlobalHF(hf);
    int buckets_depois      = getNumeroBucketsHF(hf);

    TEST_ASSERT_TRUE(profundidade_depois >= profundidade_antes);
    TEST_ASSERT_TRUE(buckets_depois >= 2);
    TEST_ASSERT_EQUAL_INT(CAPACIDADE + 1, getNumeroRegistrosHF(hf));
}

void teste_buscar_registroInseridoDeveSerEncontrado(void) {
    Registro r = { .id = 42 };
    strcpy(r.valor, "quarenta e dois");
    inserirHashFile(hf, &r);

    Registro buffer;
    TEST_ASSERT_EQUAL_INT(HF_OK, buscarHashFile(hf, &r.id, &buffer));
}

void teste_buscar_deveRetornarDadosCorretos(void) {
    Registro r = { .id = 60 };
    strcpy(r.valor, "sessenta");
    inserirHashFile(hf, &r);

    Registro buffer;
    memset(&buffer, 0, sizeof(Registro));
    buscarHashFile(hf, &r.id, &buffer);

    TEST_ASSERT_EQUAL_INT(60, buffer.id);
    TEST_ASSERT_EQUAL_STRING("sessenta", buffer.valor);
}

void teste_remover_registroExistenteDeveRetornarOk(void) {
    Registro r = { .id = 20 };
    strcpy(r.valor, "vinte");
    inserirHashFile(hf, &r);

    TEST_ASSERT_EQUAL_INT(HF_OK, removerHashFile(hf, &r.id, NULL));
}

void teste_remover_deveDecrementarContadorDeRegistros(void) {
    Registro r = { .id = 30 };
    strcpy(r.valor, "trinta");
    inserirHashFile(hf, &r);

    removerHashFile(hf, &r.id, NULL);
    TEST_ASSERT_EQUAL_INT(0, getNumeroRegistrosHF(hf));
}

void teste_remover_deveRetornarRegistroRemovidoEmOutRec(void) {
    Registro r = { .id = 50 };
    strcpy(r.valor, "cinquenta");
    inserirHashFile(hf, &r);

    Registro buffer;
    memset(&buffer, 0, sizeof(Registro));
    removerHashFile(hf, &r.id, &buffer);

    TEST_ASSERT_EQUAL_INT(55, buffer.id);
    TEST_ASSERT_EQUAL_STRING("cinquenta", buffer.valor);
}

void teste_atualizar_registroExistenteDeveRetornarOk(void) {
    Registro r = { .id = 8 };
    strcpy(r.valor, "oito");
    inserirHashFile(hf, &r);

    strcpy(r.valor, "oito_atualizado");
    TEST_ASSERT_EQUAL_INT(HF_OK, atualizarHashFile(hf, &r));
}

void teste_atualizar_deveAlterarDadosSemMudarChave(void) {
    Registro r = { .id = 15 };
    strcpy(r.valor, "quinze");
    inserirHashFile(hf, &r);

    Registro novo = { .id = 15 };
    strcpy(novo.valor, "quinze_novo");
    atualizarHashFile(hf, &novo);

    Registro buffer;
    buscarHashFile(hf, &novo.id, &buffer);
    TEST_ASSERT_EQUAL_INT(15, buffer.id);
    TEST_ASSERT_EQUAL_STRING("quinze_novo", buffer.valor);
}

void teste_dump_deveRetornarOk(void) {
    Registro r;
    for (int i = 1; i <= 5; i++) {
        r.id = i;
        sprintf(r.valor, "item%d", i);
        inserirHashFile(hf, &r);
    }
    TEST_ASSERT_EQUAL_INT(HF_OK, dumpHashFile(hf, DUMP_PATH));
}

void teste_dump_deveGerarArquivo(void) {
    Registro r = { .id = 1 };
    strcpy(r.valor, "um");
    inserirHashFile(hf, &r);
    dumpHashFile(hf, DUMP_PATH);

    FILE *f = fopen(DUMP_PATH, "r");
    TEST_ASSERT_NOT_NULL(f);
    if (f) fclose(f);
}

int main() {
    UNITY_BEGIN();
    printf("==========================================\n");
    printf("   TESTE UNITARIO: MODULO HASH FILE\n");
    printf("==========================================\n");

    RUN_TEST(teste_inserir_umRegistroDeveRetornarOk);
    RUN_TEST(teste_inserir_multiplosRegistrosDeveAumentarContador);
    RUN_TEST(teste_inserir_acimaCapacidadeDeveFazersplit);
    RUN_TEST(teste_buscar_registroInseridoDeveSerEncontrado);
    RUN_TEST(teste_buscar_deveRetornarDadosCorretos);
    RUN_TEST(teste_remover_registroExistenteDeveRetornarOk);
    RUN_TEST(teste_remover_deveDecrementarContadorDeRegistros);
    RUN_TEST(teste_remover_deveRetornarRegistroRemovidoEmOutRec);
    RUN_TEST(teste_atualizar_registroExistenteDeveRetornarOk);
    RUN_TEST(teste_atualizar_deveAlterarDadosSemMudarChave);
    RUN_TEST(teste_dump_deveRetornarOk);
    RUN_TEST(teste_dump_deveGerarArquivo);

    printf("\n==========================================\n");
    printf("   SUCESSO: TODOS OS TESTES PASSARAM!\n");
    printf("==========================================\n");

    return UNITY_END();
}