//
// Created by natha on 03/12/2025.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../unity/unity.h"
#include "../include/geo.h"

#define GEO_SIMPLES "/temp/t_geo_simples.geo"
#define GEO_COM_CQ "/temp/t_geo_cq.geo"
#define GEO_VAZIO "/temp/t_geo_vazio.geo"
#define GEO_INVALIDO "/temp/t_geo_invalido.geo"
#define GEO_MISTO "/temp/t_geo_misto.geo"
#define BASE_HF "/temp/t_geo_hf"
#define SVG_PATH "/temp/t_geo_saida.svg"

Geo geo = NULL;
HashFileConfig config = NULL;
HashFile hf = NULL;

void setUp(void) {
    FILE* f = fopen(GEO_SIMPLES, "w");
    fprintf(f, "q cep01 0.0 100.0 50.0 30.0\n");
    fprintf(f, "q cep02 0.0 100.0 50.0 30.0\n");
    fclose(f);

    f = fopen(GEO_COM_CQ, "w");
    fprintf(f, "cq 2.0 blue red\n");
    fprintf(f, "q cep10 0.0 100.0 40.0 20.0\n");
    fprintf(f, "cq 1.5 green yellow\n");
    fprintf(f, "q cep11 50.0 100.0 40.0 20.0\n");
    fclose(f);

    f = fopen(GEO_VAZIO, "w");
    fclose(f);

    f = fopen(GEO_MISTO, "w");
    fprintf(f, "\n");
    fprintf(f, "desconhecido ignorar isso tudo\n");
    fprintf(f, "q cep20 10.0 50.0 30.0 20.0\n");
    fprintf(f, "\n");
    fprintf(f, "q cep21 50.0 50.0 30.0 20.0\n");
    fclose(f);

    config = criarHashFileConfigQuadras(4);
    hf = criarHashFile(BASE_HF,config);

}
void tearDown(void) {
    if (hf != NULL) {
        fecharHashFile(hf);
        hf = NULL;
    }
    if (geo != NULL) {
        eliminarGeo(geo);
        geo = NULL;
    }
    if (config != NULL) {
        eliminarHashFileConfig(config);
        config = NULL;
    }

    remove(GEO_SIMPLES);
    remove(GEO_COM_CQ);
    remove(GEO_VAZIO);
    remove(GEO_MISTO);
    remove(BASE_HF ".hf");
    remove(BASE_HF ".hfc");
    remove(SVG_PATH);
}

void teste_criar_deveRetornarNaoNulo(void) {
    geo = criarGeo(GEO_SIMPLES);
    TEST_ASSERT_NOT_NULL(geo);
}

void teste_eliminar_comNullNaoCrash(void) {
    eliminarGeo(NULL);
}

void teste_processar_quadrasDevemEstarNoHashfile(void) {
    geo = criarGeo(GEO_SIMPLES);
    criarQuadrasNoHash(geo, hf, NULL,NULL);
    TEST_ASSERT_EQUAL_INT(HF_OK, buscarHashFile(hf, "cep01", NULL));
    TEST_ASSERT_EQUAL_INT(HF_OK, buscarHashFile(hf, "cep02", NULL));
}

void teste_processar_quadraNaoInseridaDeveSerNotFound(void) {
    geo = criarGeo(GEO_SIMPLES);
    criarQuadrasNoHash(geo, hf, NULL,NULL);
    TEST_ASSERT_EQUAL_INT(HF_OK, buscarHashFile(hf, "cepXX", NULL));

}

void teste_processar_dadosDaQuadraDevemEstarCorretos(void) {
    geo = criarGeo(GEO_SIMPLES);
    criarQuadrasNoHash(geo, hf, NULL,NULL);

    char buffer[512];
    buscarHashFile(hf, "cep01", buffer);
    Quadra q = registroParaQuadraBuffer(buffer);
    TEST_ASSERT_NOT_NULL(q);

    TEST_ASSERT_EQUAL_STRING("cep01", getCEPQuadra(q));
    TEST_ASSERT_TRUE(getXQuadra(q) == 0.0);
    TEST_ASSERT_TRUE(getYQuadra(q) == 100.0);
    TEST_ASSERT_TRUE(getWQuadra(q) == 50.0);
    TEST_ASSERT_TRUE(getHQuadra(q) == 30.0);

    eliminarQuadra(q);
}

void teste_processar_cq_deveAtualizarCorP(void) {
    geo = criarGeo(GEO_COM_CQ);
    criarQuadrasNoHash(geo, hf, NULL,NULL);
    TEST_ASSERT_EQUAL_STRING("green", getCorPEstilo(geo));
}

void teste_processar_cq_deveAtualizarCorB(void) {
    geo = criarGeo(GEO_COM_CQ);
    criarQuadrasNoHash(geo, hf, NULL,NULL);
    TEST_ASSERT_EQUAL_STRING("yellow", getCorBEstilo(geo));
}

void teste_processar_cq_deveAtualizarSW(void) {
    geo = criarGeo(GEO_COM_CQ);
    criarQuadrasNoHash(geo, hf, NULL,NULL);
    TEST_ASSERT_TRUE(getSWEstilo(geo) == 1.5);
}

void teste_processar_cq_primeiraQuadraDeveUsarPrimeiraCQ(void) {
    geo = criarGeo(GEO_COM_CQ);
    criarQuadrasNoHash(geo, hf, NULL,NULL);

    char buffer[512];
    buscarHashFile(hf, "cep10", buffer);
    Quadra q = registroParaQuadraBuffer(buffer);
    TEST_ASSERT_NOT_NULL(q);

    TEST_ASSERT_EQUAL_STRING("blue", getCorPQuadra(q));
    TEST_ASSERT_EQUAL_STRING("red", getCorBQuadra(q));
    TEST_ASSERT_TRUE(getSWEstilo(q) == 2.0);

    eliminarQuadra(q);
}

void teste_processar_cq_segundaQuadraDeveUsarSegundaCQ(void) {
    geo = criarGeo(GEO_COM_CQ);
    criarQuadrasNoHash(geo, hf, NULL,NULL);

    char buffer[512];
    buscarHashFile(hf, "cep11", buffer);
    Quadra q = registroParaQuadraBuffer(buffer);
    TEST_ASSERT_NOT_NULL(q);

    TEST_ASSERT_EQUAL_STRING("green", getCorPQuadra(q));
    TEST_ASSERT_EQUAL_STRING("yellow", getCorBQuadra(q));
    TEST_ASSERT_TRUE(getSWEstilo(q) == 1.5);

    eliminarQuadra(q);
}

void teste_processar_mistoDeveInserirApenasQuadrasValidas(void) {
    geo = criarGeo(GEO_MISTO);
    int n = criarQuadrasNoHash(geo, hf, NULL,NULL);

    TEST_ASSERT_EQUAL_INT(2, n);
    TEST_ASSERT_EQUAL_INT(HF_OK, buscarHashFile(hf, "cep20", NULL));
    TEST_ASSERT_EQUAL_INT(HF_OK, buscarHashFile(hf, "cep21", NULL));
}

int main() {
    UNITY_BEGIN();
    printf("==========================================\n");
    printf("   TESTE UNITARIO: MODULO GEO\n");
    printf("==========================================\n");

    RUN_TEST(teste_criar_deveRetornarNaoNulo);
    RUN_TEST(teste_eliminar_comNullNaoCrash);
    RUN_TEST(teste_processar_cq_deveAtualizarCorB);
    RUN_TEST(teste_processar_cq_deveAtualizarCorP);
    RUN_TEST(teste_processar_cq_deveAtualizarSW);
    RUN_TEST(teste_processar_cq_primeiraQuadraDeveUsarPrimeiraCQ);
    RUN_TEST(teste_processar_cq_segundaQuadraDeveUsarSegundaCQ);
    RUN_TEST(teste_processar_dadosDaQuadraDevemEstarCorretos);
    RUN_TEST(teste_processar_quadrasDevemEstarNoHashfile);
    RUN_TEST(teste_processar_quadraNaoInseridaDeveSerNotFound);
    RUN_TEST(teste_processar_mistoDeveInserirApenasQuadrasValidas);

    printf("\n==========================================\n");
    printf("   SUCESSO: TODOS OS TESTES PASSARAM!\n");
    printf("==========================================\n");

    return UNITY_END();;
}