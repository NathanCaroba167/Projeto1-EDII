//
// Created by natha on 03/12/2025.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../unity/unity.h"
#include "../include/svg.h"

#define PATH_SVG "/tmp/t_svg_saida.svg"
#define MAX_LINHA 512

Quadra qdr = NULL;
Quadra qdr2 = NULL;
Arquivo svg = NULL;

void setUp(void) {
    qdr = criarQuadra("cep15", 10.0, 50.0, 30.0, 20.0);
    setCorPQuadra(qdr, "blue");
    setCorBQuadra(qdr, "red");
    setSWQuadra(qdr, 1.0);
    qdr2 = criarQuadra("cep16", 0.0, 100.0, 50.0, 30.0);
    setCorPQuadra(qdr, "gray");
    setCorBQuadra(qdr, "black");
    setSWQuadra(qdr, 1.0);
    svg = abrirSVG(PATH_SVG);
}
void tearDown(void) {
    if (svg){ fclose(svg); svg = NULL; }
    remove(PATH_SVG);
}


static int arquivo_contem(const char *substring) {
    if (svg) { (svg); svg = NULL; }

    FILE *f = fopen(PATH_SVG, "r");
    if (!f) return 0;

    char linha[MAX_LINHA];
    while (fgets(linha, sizeof(linha), f)) {
        if (strstr(linha, substring)) {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

void teste_abrir_deveCriarArquivoNoDisco(void) {
    fclose(svg); svg = NULL;
    FILE *f = fopen(PATH_SVG, "r");
    TEST_ASSERT_NOT_NULL(f);
    if (f) fclose(f);
}

void teste_Quadra_deveGerarQuadraCorretamente(void) {
    desenharQuadraSVG(svg, qdr);
    TEST_ASSERT_TRUE(arquivo_contem("<rect"));
    TEST_ASSERT_TRUE(arquivo_contem("fill=\"blue\""));
    TEST_ASSERT_TRUE(arquivo_contem("stroke=\"red\""));
    TEST_ASSERT_TRUE(arquivo_contem("y=\"30.000000\""));
}

void teste_texto_deveGerarTagText(void) {
    desenharTextoSVG(svg, 5.0, 10.0, "hello", "black", 5);
    TEST_ASSERT_TRUE(arquivo_contem("<text"));
    TEST_ASSERT_TRUE(arquivo_contem("cep15"));
    TEST_ASSERT_TRUE(arquivo_contem("fill=\"blue\""));
    TEST_ASSERT_TRUE(arquivo_contem("8px"));
}

void teste_cruz_deveGerarDoisSegmentos(void) {
    desenharCruzSVG(svg, 20.0, 30.0);
    fclose(svg); svg = NULL;

    FILE *f = fopen(PATH_SVG, "r");
    TEST_ASSERT_NOT_NULL(f);
    char linha[MAX_LINHA];
    int linhas_line = 0;
    while (fgets(linha, sizeof(linha), f))
        if (strstr(linha, "<line")) linhas_line++;
    fclose(f);
    TEST_ASSERT_EQUAL_INT(2, linhas_line);
    TEST_ASSERT_TRUE(arquivo_contem("stroke=\"red\""));
}

void teste_xvermelho_deveGerarDoisSegmentos(void) {
    desenharXVermelhoSVG(svg, 20.0, 30.0);
    fclose(svg); svg = NULL;

    FILE *f = fopen(PATH_SVG, "r");
    TEST_ASSERT_NOT_NULL(f);
    char linha[MAX_LINHA];
    int linhas_line = 0;
    while (fgets(linha, sizeof(linha), f))
        if (strstr(linha, "<line")) linhas_line++;
    fclose(f);
    TEST_ASSERT_EQUAL_INT(2, linhas_line);
    TEST_ASSERT_TRUE(arquivo_contem("stroke=\"red\""));
}

void teste_circulo_deveGerarTagCircle(void) {
    desenharCirculoSVG(svg, 15.0, 25.0);
    TEST_ASSERT_TRUE(arquivo_contem("<circle"));
    TEST_ASSERT_TRUE(arquivo_contem("fill=\"black\""));
    TEST_ASSERT_TRUE(arquivo_contem("cx=\"15.00\""));
    TEST_ASSERT_TRUE(arquivo_contem("cy=\"25.00\""));
}

void teste_quadrado_deveGerarTagRect(void) {
    desenharQuadradoVermelhoSVG(svg, 10.0, 10.0, "111.111.111-11");
    TEST_ASSERT_TRUE(arquivo_contem("<rect"));
}

void teste_quadrado_deveConterTextoInterno(void) {
    desenharQuadradoVermelhoSVG(svg, 10.0, 10.0, "111.111.111-11");
    TEST_ASSERT_TRUE(arquivo_contem("111.111.111-11"));
}

void teste_quadrado_deveTerBordaVermelha(void) {
    desenharQuadradoVermelhoSVG(svg, 10.0, 10.0, "cpf");
    TEST_ASSERT_TRUE(arquivo_contem("stroke=\"red\""));
    TEST_ASSERT_TRUE(arquivo_contem("<text"));
}

void teste_multiplosElementos_devemTodosAparererNoArquivo(void) {
    desenharQuadraSVG(svg, qdr2);
    desenharTextoSVG(svg, 5.0, 80.0, "cep01", "black", 5);
    desenharCruzSVG(svg, 25.0, 85.0);
    desenharCirculoSVG(svg, 25.0, 85.0);
    desenharXVermelhoSVG(svg, 25.0, 85.0);
    desenharQuadradoVermelhoSVG(svg, 20.0, 75.0, "999");

    fclose(svg); svg = NULL;

    FILE *f = fopen(PATH_SVG, "r");
    TEST_ASSERT_NOT_NULL(f);

    char linha[MAX_LINHA];
    int retangulos = 0, textos = 0, linhas = 0, circles = 0;
    while (fgets(linha, sizeof(linha), f)) {
        if (strstr(linha, "<rect")) retangulos++;
        if (strstr(linha, "<text")) textos++;
        if (strstr(linha, "<line")) linhas++;
        if (strstr(linha, "<circle")) circles++;
    }
    fclose(f);

    TEST_ASSERT_TRUE(retangulos   >= 2);
    TEST_ASSERT_TRUE(textos   >= 2);
    TEST_ASSERT_TRUE(linhas   >= 4);
    TEST_ASSERT_EQUAL_INT(1, circles);
}

int main() {
    UNITY_BEGIN();
    printf("==========================================\n");
    printf("   TESTE UNITARIO: MODULO SVG\n");
    printf("==========================================\n");

    RUN_TEST(teste_abrir_deveCriarArquivoNoDisco);
    RUN_TEST(teste_texto_deveGerarTagText);
    RUN_TEST(teste_cruz_deveGerarDoisSegmentos);
    RUN_TEST(teste_xvermelho_deveGerarDoisSegmentos);
    RUN_TEST(teste_circulo_deveGerarTagCircle);
    RUN_TEST(teste_quadrado_deveGerarTagRect);
    RUN_TEST(teste_quadrado_deveConterTextoInterno);
    RUN_TEST(teste_quadrado_deveTerBordaVermelha);
    RUN_TEST(teste_multiplosElementos_devemTodosAparererNoArquivo);

    printf("\n==========================================\n");
    printf("   SUCESSO: TODOS OS TESTES PASSARAM!\n");
    printf("==========================================\n");

    return UNITY_END();
}