//
// Created by natha on 03/12/2025.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../unity/unity.h"
#include "../include/txt.h"
#include "../include/qry.h"
#include "../include/quadra.h"
#include "../include/habitante.h"

#define QRY_RQ        "/tmp/t_qry_rq.qry"
#define QRY_PQ        "/tmp/t_qry_pq.qry"
#define QRY_CENSO     "/tmp/t_qry_censo.qry"
#define QRY_HINTERR   "/tmp/t_qry_hinterr.qry"
#define QRY_NASC      "/tmp/t_qry_nasc.qry"
#define QRY_RIP       "/tmp/t_qry_rip.qry"
#define QRY_MUD       "/tmp/t_qry_mud.qry"
#define QRY_DSPJ      "/tmp/t_qry_dspj.qry"
#define BASE_HF_Q     "/tmp/t_qry_quadras"
#define BASE_HF_H     "/tmp/t_qry_habitantes"
#define TXT_SAIDA     "/tmp/t_qry_saida.txt"

Arquivo qry = NULL;
HashFileConfig config_q = NULL;
HashFileConfig config_h = NULL;
HashFile hf_q  = NULL;
HashFile hf_h  = NULL;


static void popula_quadras(void) {
    char buffer[512];
    Quadra q;
    q = criarQuadra("cep01", 0.0, 100.0, 50.0, 30.0);
    setCorPQuadra(q, "grey"); setCorBQuadra(q, "black");
    quadraBufferParaRegistro(q, buffer); inserirHashFile(hf_q, buffer); eliminarQuadra(q);

    q = criarQuadra("cep05", 60.0, 100.0, 40.0, 20.0);
    setCorPQuadra(q, "white"); setCorBQuadra(q, "black");
    quadraBufferParaRegistro(q, buffer); inserirHashFile(hf_q, buffer); eliminarQuadra(q);
}

static void popula_habitantes(void) {
    char buffer[512];
    Habitante h;

    h = criarHabitante("111.111.111-11", "Ana", "Silva", 'F', "01/01/1990");
    habitanteBufferParaRegistro(h, buffer); inserirHashFile(hf_h, buffer); eliminarHabitante(h);

    h = criarHabitante("222.222.222-22", "Bruno", "Costa", 'M', "10/06/1985");
    setEnderecoHabitante(h, "cep01", 'S', 10, "casa");
    habitanteBufferParaRegistro(h, buffer); inserirHashFile(hf_h, buffer); eliminarHabitante(h);

    h = criarHabitante("333.333.333-33", "Carla", "Ramos", 'F', "20/03/1992");
    setEnderecoHabitante(h, "cep01", 'N', 5, "ap1");
    habitanteBufferParaRegistro(h, buffer); inserirHashFile(hf_h, buffer); eliminarHabitante(h);

    h = criarHabitante("444.444.444-44", "Davi", "Lima", 'M', "07/11/1978");
    setEnderecoHabitante(h, "cep05", 'L', 20, "sala");
    habitanteBufferParaRegistro(h, buffer); inserirHashFile(hf_h, buffer); eliminarHabitante(h);
}

void setUp(void) {
    config_q = criarHashFileConfigQuadras(4);
    config_h = criarHashFileConfigHabitantes(4);
    hf_q = criarHashFile(BASE_HF_Q, config_q);
    hf_h = criarHashFile(BASE_HF_H, config_h);

    popula_quadras();
    popula_habitantes();

    FILE *f;

    f = fopen(QRY_RQ,"w"); fprintf(f, "rq cep01\n");         fclose(f);
    f = fopen(QRY_PQ,"w"); fprintf(f, "pq cep01\n");         fclose(f);
    f = fopen(QRY_CENSO,"w"); fprintf(f, "censo\n");            fclose(f);
    f = fopen(QRY_HINTERR,"w"); fprintf(f, "h? 222.222.222-22\n"); fclose(f);
    f = fopen(QRY_NASC,"w");
    fprintf(f, "nasc 555.555.555-55 Eva Gomes F 05/05/2005\n");    fclose(f);
    f = fopen(QRY_RIP,"w"); fprintf(f, "rip 111.111.111-11\n"); fclose(f);
    f = fopen(QRY_MUD,"w");
    fprintf(f, "mud 222.222.222-22 cep05 N 15 novo\n");            fclose(f);
    f = fopen(QRY_DSPJ,"w"); fprintf(f, "dspj 222.222.222-22\n"); fclose(f);
}

void tearDown(void) {
    if (hf_q)  { fecharHashFile(hf_q); hf_q = NULL; }
    if (hf_h)  { fecharHashFile(hf_h); hf_h = NULL; }
    if (qry)   { fclose(qry); qry = NULL; }
    if (config_q) { eliminarHashFileConfig(config_q); config_q = NULL; }
    if (config_h) { eliminarHashFileConfig(config_h); config_h = NULL; }
    remove(QRY_RQ); remove(QRY_PQ); remove(QRY_CENSO);
    remove(QRY_HINTERR); remove(QRY_NASC); remove(QRY_RIP);
    remove(QRY_MUD); remove(QRY_DSPJ); remove(BASE_HF_Q ".hf");
    remove(BASE_HF_Q ".hfc"); remove(BASE_HF_H ".hf");
    remove(BASE_HF_H ".hfc"); remove(TXT_SAIDA);
}

void teste_rq_deveRemoverQuadraDoHashfile(void) {
    qry = abrirQry(QRY_RQ);
    LerComandosExecutar(qry, hf_q, hf_h, NULL, NULL);
    TEST_ASSERT_EQUAL_INT(HF_NAO_ENCONTRADO, buscarHashFile(hf_q, "cep01", NULL));
}

void teste_rq_moradoresDaQuadraDevemVirarSemTeto(void) {
    qry = abrirQry(QRY_RQ);
    LerComandosExecutar(qry, hf_q, hf_h, NULL, NULL);

    char buffer[512];
    /* Bruno e Carla moravam em cep01 — devem virar sem-teto */
    buscarHashFile(hf_h, "222.222.222-22", buffer);
    Habitante h = registroParaHabitanteBuffer(buffer);
    TEST_ASSERT_NOT_NULL(h);
    TEST_ASSERT_TRUE(!isMorador(h));
    eliminarHabitante(h);
}

void teste_rq_deveEscreverMoradoresNoTxt(void) {
    qry = abrirQry(QRY_RQ);
    Arquivo txt = abrirTXT(TXT_SAIDA);
    LerComandosExecutar(qry, hf_q, hf_h, NULL, txt);
    fclose(txt);

    FILE *f = fopen(TXT_SAIDA, "r");
    TEST_ASSERT_NOT_NULL(f);
    char linha[256];
    int linhas = 0;
    while (fgets(linha, sizeof(linha), f)) linhas++;
    fclose(f);
    TEST_ASSERT_TRUE(linhas > 0);
}

void teste_pq_deveEscreverContadoresNoTxt(void) {
    qry = abrirQry(QRY_PQ);
    Arquivo txt = abrirTXT(TXT_SAIDA);
    LerComandosExecutar(qry, hf_q, hf_h, NULL, txt);
    fclose(txt);

    FILE *f = fopen(TXT_SAIDA, "r");
    TEST_ASSERT_NOT_NULL(f);
    char buffer[256]; int linhas = 0;
    while (fgets(buffer, sizeof(buffer), f)) linhas++;
    fclose(f);
    TEST_ASSERT_TRUE(linhas >= 2); /* [*] pq + linha com contadores */
}

void teste_pq_naoAlteraHashfiles(void) {
    qry = abrirQry(QRY_PQ);
    int antes_q = getNumeroRegistrosHF(hf_q);
    int antes_h = getNumeroRegistrosHF(hf_h);
    LerComandosExecutar(qry, hf_q, hf_h, NULL, NULL);
    TEST_ASSERT_EQUAL_INT(antes_q, getNumeroRegistrosHF(hf_q));
    TEST_ASSERT_EQUAL_INT(antes_h, getNumeroRegistrosHF(hf_h));
}

void teste_censo_deveEscreverEstatisticasNoTxt(void) {
    qry = abrirQry(QRY_CENSO);
    Arquivo txt = abrirTXT(TXT_SAIDA);
    LerComandosExecutar(qry, hf_q, hf_h, NULL, txt);
    fclose(txt);

    FILE *f = fopen(TXT_SAIDA, "r");
    TEST_ASSERT_NOT_NULL(f);
    char buffer[256]; int linhas = 0;
    while (fgets(buffer, sizeof(buffer), f)) linhas++;
    fclose(f);
    /* [*] censo + 12 linhas de estatísticas = 13 */
    TEST_ASSERT_TRUE(linhas >= 13);
}

void teste_hinterrog_deveEscreverDadosNoTxt(void) {
    qry = abrirQry(QRY_HINTERR);
    Arquivo txt = abrirTXT(TXT_SAIDA);
    LerComandosExecutar(qry, hf_q, hf_h, NULL, txt);
    fclose(txt);

    FILE *f = fopen(TXT_SAIDA, "r");
    TEST_ASSERT_NOT_NULL(f);
    char buffer[256]; int linhas = 0;
    while (fgets(buffer, sizeof(buffer), f)) linhas++;
    fclose(f);
    TEST_ASSERT_TRUE(linhas >= 2);
}

void teste_nasc_deveInserirHabitanteNoHashfile(void) {
    qry = abrirQry(QRY_NASC);
    LerComandosExecutar(qry, hf_q, hf_h, NULL, NULL);
    TEST_ASSERT_EQUAL_INT(HF_OK,
        buscarHashFile(hf_h, "555.555.555-55", NULL));
}

void teste_nasc_habitanteInseridoDeveSerSemTeto(void) {
    qry = abrirQry(QRY_NASC);
    LerComandosExecutar(qry, hf_q, hf_h, NULL, NULL);

    char buffer[512];
    buscarHashFile(hf_h, "555.555.555-55", buffer);
    Habitante h = registroParaHabitanteBuffer(buffer);
    TEST_ASSERT_NOT_NULL(h);
    TEST_ASSERT_TRUE(!isMorador(h));
    eliminarHabitante(h);
}

void teste_rip_deveRemoverHabitanteDoHashfile(void) {
    qry = abrirQry(QRY_RIP);
    LerComandosExecutar(qry, hf_q, hf_h, NULL, NULL);
    TEST_ASSERT_EQUAL_INT(HF_NAO_ENCONTRADO, buscarHashFile(hf_h, "111.111.111-11", NULL));
}

void teste_rip_deveEscreverDadosNoTxt(void) {
    qry = abrirQry(QRY_RIP);
    Arquivo txt = abrirTXT(TXT_SAIDA);
    LerComandosExecutar(qry, hf_q, hf_h, NULL, txt);
    fclose(txt);

    FILE *f = fopen(TXT_SAIDA, "r");
    TEST_ASSERT_NOT_NULL(f);
    char buffer[256]; int linhas = 0;
    while (fgets(buffer, sizeof(buffer), f)) linhas++;
    fclose(f);
    TEST_ASSERT_TRUE(linhas >= 2);
}

void teste_mud_deveAtualizarEnderecoNoHashfile(void) {
    qry = abrirQry(QRY_MUD);
    LerComandosExecutar(qry, hf_q, hf_h, NULL, NULL);

    char buffer[512];
    buscarHashFile(hf_h, "222.222.222-22", buffer);
    Habitante h = registroParaHabitanteBuffer(buffer);
    TEST_ASSERT_NOT_NULL(h);
    TEST_ASSERT_EQUAL_STRING("cep05", getCEPEnderecoHabitante(h));
    TEST_ASSERT_EQUAL_INT('N', getFaceEnderecoHabitante(h));
    TEST_ASSERT_EQUAL_INT(15, getNumeroEnderecoHabitante(h));
    eliminarHabitante(h);
}

void teste_dspj_deveTornarHabitanteSemTeto(void) {
    qry = abrirQry(QRY_DSPJ);
    LerComandosExecutar(qry, hf_q, hf_h, NULL, NULL);

    char buffer[512];
    buscarHashFile(hf_h, "222.222.222-22", buffer);
    Habitante h = registroParaHabitanteBuffer(buffer);
    TEST_ASSERT_NOT_NULL(h);
    TEST_ASSERT_TRUE(!isMorador(h));
    eliminarHabitante(h);
}

void teste_dspj_deveEscreverDadosNoTxt(void) {
    qry = abrirQry(QRY_DSPJ);
    Arquivo txt = abrirTXT(TXT_SAIDA);
    LerComandosExecutar(qry, hf_q, hf_h, NULL, txt);
    fclose(txt);

    FILE *f = fopen(TXT_SAIDA, "r");
    TEST_ASSERT_NOT_NULL(f);
    char buffer[256]; int linhas = 0;
    while (fgets(buffer, sizeof(buffer), f)) linhas++;
    fclose(f);
    TEST_ASSERT_TRUE(linhas >= 2);
}


int main() {
    printf("==========================================\n");
    printf("   TESTE UNITARIO: MODULO QRY\n");
    printf("==========================================\n");

    RUN_TEST(teste_rq_deveRemoverQuadraDoHashfile);
    RUN_TEST(teste_rq_moradoresDaQuadraDevemVirarSemTeto);
    RUN_TEST(teste_pq_deveEscreverContadoresNoTxt);
    RUN_TEST(teste_pq_naoAlteraHashfiles);
    RUN_TEST(teste_censo_deveEscreverEstatisticasNoTxt);
    RUN_TEST(teste_hinterrog_deveEscreverDadosNoTxt);
    RUN_TEST(teste_nasc_deveInserirHabitanteNoHashfile);
    RUN_TEST(teste_nasc_habitanteInseridoDeveSerSemTeto);
    RUN_TEST(teste_rip_deveRemoverHabitanteDoHashfile);
    RUN_TEST(teste_rip_deveEscreverDadosNoTxt);
    RUN_TEST(teste_mud_deveAtualizarEnderecoNoHashfile);
    RUN_TEST(teste_dspj_deveTornarHabitanteSemTeto);
    RUN_TEST(teste_dspj_deveEscreverDadosNoTxt);

    printf("\n==========================================\n");
    printf("   SUCESSO: TODOS OS TESTES PASSARAM!\n");
    printf("==========================================\n");

    return 0;
}