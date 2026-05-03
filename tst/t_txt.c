//
// Created by natha on 03/12/2025.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../unity/unity.h"

#include "../include/txt.h"

#define TXT_PATH "/tmp/t_txt_saida.txt"
#define LINHA_MAX 512

Arquivo txt = NULL;

void setUp(void) {
    txt = abrirTXT(TXT_PATH);
}

void tearDown(void) {
    if (txt) { fclose(txt); txt = NULL; }
    remove(TXT_PATH);
}

static int arquivo_contem(const char *substring) {
    if (txt) { fclose(txt); txt = NULL; }

    FILE *f = fopen(TXT_PATH, "r");
    if (!f) return 0;

    char linha[LINHA_MAX];
    while (fgets(linha, sizeof(linha), f)) {
        if (strstr(linha, substring)) {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

static int contar_linhas(void) {
    if (txt) { fclose(txt); txt = NULL; }

    FILE *f = fopen(TXT_PATH, "r");
    if (!f) return 0;

    char linha[LINHA_MAX];
    int n = 0;
    while (fgets(linha, sizeof(linha), f)) n++;
    fclose(f);
    return n;
}

void teste_abrir_deveCriarArquivoNoDisco(void) {
    fclose(txt); txt = NULL;
    FILE *f = fopen(TXT_PATH, "r");
    TEST_ASSERT_NOT_NULL(f);
    if (f) fclose(f);
}

void teste_habitante_semTeto_deveEscreverDados(void) {
    escreverHabitante(txt,
                 "111.111.111-11", "Ana", "Silva",
                 'F', "01/01/1990",
                 0, NULL, '\0', -1, NULL);

    TEST_ASSERT_TRUE(arquivo_contem("cpf: 111.111.111-11"));
    TEST_ASSERT_TRUE(arquivo_contem("nome: Ana Silva"));
    TEST_ASSERT_TRUE(arquivo_contem("sexo: F"));
    TEST_ASSERT_TRUE(arquivo_contem("nasc: 01/01/1990"));
    TEST_ASSERT_TRUE(arquivo_contem("sem-teto"));
}

void teste_habitante_morador_deveEscreverEndereco(void) {
    escreverHabitante(txt,
                 "222.222.222-22", "Bruno", "Costa",
                 'M', "10/06/1985",
                 1, "cep05", 'S', 45, "apto 3");

    TEST_ASSERT_TRUE(arquivo_contem("cpf: 222.222.222-22"));
    TEST_ASSERT_TRUE(arquivo_contem("nome: Bruno Costa"));
    TEST_ASSERT_TRUE(arquivo_contem("endereco: cep05/S/45 apto 3"));
}

void teste_censo_deveEscreverTodasLinhas(void) {
    escreverCenso(txt, 10, 6, 4, 6, 3, 3, 4, 2, 2);
    TEST_ASSERT_EQUAL_INT(12, contar_linhas());
}

void teste_censo_deveConterDadosCensoCorretos(void) {
    escreverCenso(txt, 10, 6, 4, 6, 3, 3, 4, 2, 2);
    TEST_ASSERT_TRUE(arquivo_contem("total habitantes: 10"));
    TEST_ASSERT_TRUE(arquivo_contem("total moradores: 6"));
    TEST_ASSERT_TRUE(arquivo_contem("moradores/habitantes: 60.00%"));
    TEST_ASSERT_TRUE(arquivo_contem("sem-tetos: 4"));
}

int main() {
    printf("==========================================\n");
    printf("   TESTE UNITARIO: MODULO TXT\n");
    printf("==========================================\n");

    RUN_TEST(teste_abrir_deveCriarArquivoNoDisco);

    RUN_TEST(teste_habitante_semTeto_deveEscreverDados);
    RUN_TEST(teste_habitante_morador_deveEscreverEndereco);

    RUN_TEST(teste_censo_deveEscreverTodasLinhas);
    RUN_TEST(teste_censo_deveConterDadosCensoCorretos);

    printf("\n==========================================\n");
    printf("   SUCESSO: TODOS OS TESTES PASSARAM!\n");
    printf("==========================================\n");

    return 0;
}