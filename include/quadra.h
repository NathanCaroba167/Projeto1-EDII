//
// Created by natha on 30/03/2026.
//

#ifndef QUADRA_H
#define QUADRA_H

/*
 Módulo encarregado de coordenar uma quadra com operações de criar, get, set e eliminar
 */

/*
 Ponteiro void Quadra
 */
typedef void* Quadra;

/// @brief Cria um quadra com os atributos indicados.
/// @param cep Identificador único que se refere ao quadra.
/// @param x Coordenada x do ponto âncora (canto inferior esquerdo) da quadra.
/// @param y Coordenada y do ponto âncora (canto inferior esquerdo) da quadra.
/// @param w Largura do quadra.
/// @param h Altura do quadra.
/// @param corB Cor de borda da quadra no padrão RGB (Ex: #FFFFFF).
/// @param corP Cor de preenchimento da quadra no padrão RGB (Ex: #AA543B).
/// @return Ponteiro para o quadra criado.
/// @warning
Quadra CriarQuadra(char* cep,double x,double y,double w,double h,char* corB,char* corP );

/// @brief Pega o CEP da quadra sem modifica-lo.
/// @param q Ponteiro para o quadra analisada.
/// @return Um inteiro que representa o CEP da quadra.
/// @warning
char* getCEPQuadra(Quadra q);

/// @brief Define o CEP da quadra.
/// @param q Ponteiro para o quadra onde o ID será definido.
/// @param cep CEP que será definido.
/// @warning
void setCEPQuadra(Quadra q,char* cep);

/// @brief Pega a coordenada x da quadra sem modifica-la.
/// @param q Ponteiro para o quadra analisada.
/// @return Um double que representa a coordenada x da quadra.
/// @warning
double getXQuadra(Quadra q);

/// @brief Define a nova coordenada x do quadra.
/// @param q Ponteiro para o quadra onde a coordenada x será definida.
/// @param x Nova coordenada x que será definida.
/// @warning
void setXQuadra(Quadra q,double x);

/// @brief Pega a coordenada y do quadra sem modifica-la.
/// @param q Ponteiro para o quadra analisada.
/// @return Um double que representa a coordenada y da quadra.
/// @warning
double getYQuadra(Quadra q);

/// @brief Define a nova coordenada y do quadra.
/// @param q Ponteiro para o quadra onde a coordenada y será definida.
/// @param y Nova coordenada y que será definida.
/// @warning
void setYQuadra(Quadra q,double y);

/// @brief Pega a largura do quadra sem modifica-lo.
/// @param q Ponteiro para o quadra analisada.
/// @return Um double que representa a largura da quadra.
/// @warning
double getWQuadra(Quadra q);

/// @brief Define a nova largura do quadra.
/// @param q Ponteiro para o quadra onde a largura será definida.
/// @param w Nova largura que será definido.
/// @warning
void setWQuadra(Quadra q, double w);

/// @brief Pega a altura do quadra sem modifica-lo.
/// @param q Ponteiro para o quadra analisada.
/// @return Um double que representa a altura da quadra.
/// @warning
double getHQuadra(Quadra q);

/// @brief Define a nova altura do quadra.
/// @param q Ponteiro para o quadra onde a altura será definida.
/// @param h Nova altura que será definido.
/// @warning
void setHQuadra(Quadra q,double h);

/// @brief Pega a cor de borda da quadra sem modifica-la.
/// @param q Ponteiro para o quadra analisada.
/// @return Uma string com a cor de borda da quadra.
/// @warning
char* getCorBQuadra(Quadra q);

/// @brief Define a nova cor de borda da quadra.
/// @param q Ponteiro para o quadra onde a cor de borda será definida.
/// @param corB Nova cor de borda que será definida.
/// @warning
void setCorBQuadra(Quadra q,char* corB);

/// @brief Pega a cor de preenchimento da quadra sem modifica-la.
/// @param q Ponteiro para o quadra analisada.
/// @return Uma string com a cor de preenchimento da quadra.
/// @warning
char* getCorPQuadra(Quadra q);

/// @brief Define a nova cor de preenchimento da quadra.
/// @param q Ponteiro para o quadra onde a cor de preenchimento será definida.
/// @param corP Nova cor de preenchimento que será definida.
/// @warning
void setCorPQuadra(Quadra q,char* corP);

/// @brief Pega a espessura da borda da quadra sem modifica-la.
/// @param q Ponteiro para a quadra analisada.
/// @return Um double que representa a espessura da borda.
/// @warning
double getSWQuadra(Quadra q);

/// @brief Define a nova espessura da borda da quadra.
/// @param q Ponteiro para a quadra onde a espessura será definida
/// @param sw Nova espessura da borda
/// @warning
void setSWQuadra(Quadra q,double sw);

/// @brief Libera a memória alocada para a quadra, incluindo as strings de cor.
/// @param q Ponteiro para o quadra que será eliminado.
/// @warning
void eliminarQuadra(Quadra q);

#endif //QUADRA_H