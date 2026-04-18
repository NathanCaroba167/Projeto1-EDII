//
// Created by natha on 04/10/2025.
//

#ifndef SVG_H
#define SVG_H

#include "quadra.h"

/*
 Módulo encarregado de gerar um arquivo SVG a partir das quadras e ações do qry e executar os comandos com as operações de abrir,
 inicializar, desenhar, gerar e fechar
 */

/*
 Ponteiro do tipo FILE Arquivo
 Ponteiro char Nome
 */
typedef FILE* Arquivo;
typedef char* Nome;

/// @brief Cria e abre o arquivo SVG para escrita (sobrescrevendo o conteúdo existentes).
/// @param arquivo Ponteiro para o nome do arquivo que será criado e aberto.
/// @return Ponteiro para o arquivo (FILE*).
/// @warning
Arquivo abrirSVG(Nome arquivo);

/// @brief Adiciona a tag iniciais do arquivo SVG.
/// @param svg Ponteiro para o arquivo que será inicializado.
/// @warning
void inicializarSVG(Arquivo svg);

/// @brief Desenha uma quadra no arquivo SVG.
/// @param svg Ponteiro para o arquivo que receberá o desenho.
/// @param q Quadra que será desenhado.
/// @warning
void desenharQuadraSVG(Arquivo svg, Quadra q);

/// @brief Desenha um texto no arquivo SVG.
/// @param svg Ponteiro para o arquivo que receberá o desenho.
/// @param x Coordenada x âncora do texto.
/// @param y Coordenada y âncora do texto.
/// @param texto Texto a ser desenhado.
/// @param cor Cor do texto a ser desenhado.
/// @param tamanho Tamanho da fonte do texto.
/// @warning
void desenharTextoSVG(Arquivo svg, double x, double y, char* texto, char* cor, int tamanho);

/// @brief Desenha uma cruz vermelha no arquivo SVG.
/// @param svg Ponteiro para o arquivo que receberá o desenho.
/// @param x Coordenada x do centro da cruz.
/// @param y Coordenada y do centro da cruz.
void desenharCruzSVG(Arquivo svg, double x, double y);

/// @brief Desenha um círculo preto no arquivo SVG.
/// @param svg Ponteiro para o arquivo que receberá o desenho.
/// @param x Coordenada x do centro do círculo.
/// @param y Coordenada y do centro do círculo.
/// @warning
void desenharCirculoSVG(Arquivo svg, double x, double y);

/// @brief Desenha um quadrado vermelho com CPF interno no arquivo SVG.
/// @param svg Ponteiro para o arquivo que receberá o desenho.
/// @param x Coordenada x do canto superior esquerdo do quadrado.
/// @param y Coordenada y do canto superior esquerdo do quadrado.
/// @param Cpf Cpf a ser inserido dentro do quadrado.
/// @warning
void desenharQuadradoSVG(Arquivo svg, double x, double y, char* Cpf);

/// @brief Desenha um X vermelho no arquivo SVG.
/// @param svg Ponteiro para o arquivo que receberá o desenho.
/// @param x Coordenada x do ponto de ancoragem da quadra removida
/// @param y Coordenada y do ponto de ancoragem da quadra removida.
/// @warning
void desenharXVermelhoSVG(Arquivo svg, double x, double y);

/// @brief Fecha o arquivo SVG, adicionando as tags de fechamento.
/// @param svg Ponteiro para o arquivo que será fechado.
/// @warning
void fecharSVG(Arquivo svg);

#endif //SVG_H