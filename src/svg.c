//
// Created by natha on 04/10/2025.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

#include "../include/svg.h"

Arquivo abrirSVG(Nome arquivoSvg) {
    Arquivo svg = fopen(arquivoSvg,"w");
    if (svg == NULL) {
        printf("Erro ao abrir o arquivo svg!\n");

        perror("Motivo do erro");
        exit(1);
    }
    return svg;
}

void inicializarSVG(Arquivo svg) {
    if (svg == NULL) return;
    fprintf(svg,"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n");
    fprintf(svg,"<svg xmlns:svg=\"http://www.w3.org/2000/svg\" xmlns=\"http://www.w3.org/2000/svg\" width=\"10000\" height=\"10000\">\n");
    fprintf(svg,"\t<g>\n");
}

void desenharCirculoSVG(Arquivo svg, double x, double y) {
    if (svg == NULL) return;
    fprintf(svg, "<circle cx=\"%lf\" "
                 "cy=\"%lf\" "
                 "r=\"%lf\" "
                 "stroke=\"black\" "
                 "fill=\"black\" "
                 "opacity=\"%lf\" "
                 "stroke-width=\"%lf\" "
                 "/>\n",
                 x,
                 y,
                 1.0,
                 0.5,
                 1.5);
}

void desenharQuadraSVG(Arquivo svg, Quadra q) {
    if (svg == NULL || q == NULL) return;
    fprintf(svg, "\t<rect x=\"%lf\" "
                 "y=\"%lf\" "
                 "width=\"%lf\" "
                 "height=\"%lf\" "
                 "stroke=\"%s\" "
                 "fill=\"%s\" "
                 "opacity=\"%lf\" "
                 "stroke-width=\"%lf\" "
                 "/>\n",
                 getXQuadra(q),
                 getYQuadra(q),
                 getWQuadra(q),
                 getHQuadra(q),
                 getCorBQuadra(q),
                 getCorPQuadra(q),
                 0.5,
                 1.5);

}

void desenharCruzSVG(Arquivo svg, double x, double y) {
    if (svg == NULL) return;

    double tamanhoCruz = 1;
    fprintf(svg, "\t<line x1=\"%lf\" "
                 "y1=\"%lf\" "
                 "x2=\"%lf\" "
                 "y2=\"%lf\" "
                 "stroke=\"red\" "
                 "stroke-width=\"%lf\" "
                 "/>\n",
                 x,
                 y - tamanhoCruz,
                 x,
                 y + tamanhoCruz,
                 1.5);

    fprintf(svg, "\t<line x1=\"%lf\" "
                 "y1=\"%lf\" "
                 "x2=\"%lf\" "
                 "y2=\"%lf\" "
                 "stroke=\"red\" "
                 "stroke-width=\"%lf\" "
                 "/>\n",
                 x - tamanhoCruz,
                 y,
                 x + tamanhoCruz,
                 y,
                 1.5);

}

void desenharXVermelhoSVG(Arquivo svg, double x, double y) {
    if (svg == NULL) return;

    double tamanhoX = 1;
    fprintf(svg, "\t<line x1=\"%lf\" "
                 "y1=\"%lf\" "
                 "x2=\"%lf\" "
                 "y2=\"%lf\" "
                 "stroke=\"red\" "
                 "stroke-width=\"%lf\" "
                 "/>\n",
                 x - tamanhoX,
                 y - tamanhoX,
                 x + tamanhoX,
                 y + tamanhoX,
                 1.5);

    fprintf(svg, "\t<line x1=\"%lf\" "
                 "y1=\"%lf\" "
                 "x2=\"%lf\" "
                 "y2=\"%lf\" "
                 "stroke=\"red\" "
                 "stroke-width=\"%lf\" "
                 "/>\n",
                 x + tamanhoX,
                 y - tamanhoX,
                 x - tamanhoX,
                 y + tamanhoX,
                 1.5);

}

void desenharTextoSVG(Arquivo svg, double x, double y, char* texto, char* cor, int tamanho) {
    if (svg == NULL) return;
    fprintf(svg, "\t<text x=\"%lf\" "
                 "y=\"%lf\" "
                 "fill=\"%s\" "
                 "font-size=\"%dpt\" "
                 "text-anchor=\"middle\" "
                 ">%s</text>\n",
                 x,
                 y,
                 cor,
                 tamanho,
                 texto);

}

void desenharQuadradoVermelhoSVG(Arquivo svg, double x, double y, char* Cpf) {
    if (svg == NULL) return;
    double tamanhoLadoQuadrado = 1;
    fprintf(svg, "\t<rect x=\"%lf\" "
                 "y=\"%lf\" "
                 "width=\"%lf\" "
                 "height=\"%lf\" "
                 "stroke=\"red\" "
                 "fill=\"none\" "
                 "opacity=\"%lf\" "
                 "stroke-width=\"%lf\" "
                 "/>\n",
                 x,
                 y,
                 tamanhoLadoQuadrado,
                 tamanhoLadoQuadrado,
                 0.5,
                 1.5);

    fprintf(svg, "\t<text x=\"%lf\" "
                 "y=\"%lf\" "
                 "fill=\"red\" "
                 "font-size=\"3px\" "
                 "text-anchor=\"middle\" "
                 ">%s</text>\n",
                 x + 1,
                 y + tamanhoLadoQuadrado - 1,
                 Cpf);
}

void fecharSVG(Arquivo svg) {
    if (svg == NULL) return;
    fprintf(svg, "</g>\n");
    fprintf(svg,"</svg>\n");
    fclose(svg);

    printf("Arquivo SVG gerado com sucesso!\n");

}