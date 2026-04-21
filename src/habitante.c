//
// Created by natha on 30/03/2026.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <stdbool.h>

#include "../include/habitante.h"

#define CPF_MAX 20
#define NOME_MAX 50
#define SOBRENOME_MAX 50
#define NASCIMENTO_MAX 10
#define CEP_MAX 10
#define COMPLEMENTO_MAX 10

typedef struct {
    char cpf[CPF_MAX];
    char nomeHabitante[NOME_MAX];
    char sobrenomeHabitante[SOBRENOME_MAX];
    char sexo;
    char nascimento[NASCIMENTO_MAX];
    bool morador;
    char enderecoCep[CEP_MAX];
    char enderecoFace;
    int enderecoNumero;
    char enderecoComplemento[COMPLEMENTO_MAX];
}habitante;

Habitante criarHabitante(char* cpf, char* nome, char* sobrenome, char sexo, char* nasc) {
    habitante* h = calloc(1, sizeof(habitante));

    if (h==NULL) {
        printf("Erro ao alocar memória ao CriarHabitante!\n");

        perror("Motivo do erro");
        exit(1);
    }

    strncpy(h->cpf,cpf,  CPF_MAX - 1);
    strncpy(h->nomeHabitante,nome, NOME_MAX - 1);
    strncpy(h->sobrenomeHabitante,sobrenome, SOBRENOME_MAX - 1);
    h->sexo = sexo;
    strncpy(h->nascimento,nasc, NASCIMENTO_MAX - 1);
    h->morador = false;

    h->cpf[CPF_MAX - 1] = '\0';
    h->nomeHabitante[NOME_MAX - 1] = '\0';
    h->sobrenomeHabitante[SOBRENOME_MAX - 1] = '\0';
    h->nascimento[NASCIMENTO_MAX - 1] = '\0';

    h->enderecoCep[0] = '\0';
    h->enderecoFace = '\0';
    h->enderecoNumero = -1;
    h->enderecoComplemento[0] = '\0';

    return h;
}

char* getCPFHabitante(Habitante hab) {
    return ((habitante*)hab)->cpf;

}

char* getNomeHabitante(Habitante hab) {
    return ((habitante*)hab)->nomeHabitante;

}

void setNomeHabitante(Habitante hab, char* nome) {
    strncpy(((habitante*)hab)->nomeHabitante,nome, NOME_MAX - 1);

}

char* getSobrenomeHabitante(Habitante hab) {
    return ((habitante*)hab)->sobrenomeHabitante;

}

void setSobrenomeHabitante(Habitante hab, char* sobrenome) {
    strncpy(((habitante*)hab)->sobrenomeHabitante,sobrenome, SOBRENOME_MAX - 1);
}

char getSexoHabitante(Habitante hab) {
    return ((habitante*)hab)->sexo;

}

char* getNascHabitante(Habitante hab) {
    return ((habitante*)hab)->nascimento;
}

bool ehMorador(Habitante hab) {
    return ((habitante*)hab)->morador == true;
}

void setEnderecoHabitante(Habitante hab, char* cep, char face, int num, char* complemento) {
    habitante* h = (habitante*)hab;

    strncpy(h->enderecoCep,cep, CEP_MAX - 1);
    strncpy(h->enderecoComplemento,complemento, COMPLEMENTO_MAX - 1);

    h->enderecoCep[CEP_MAX - 1] = '\0';
    h->enderecoComplemento[COMPLEMENTO_MAX - 1] = '\0';

    h->enderecoFace = face;
    h->enderecoNumero = num;
    h->morador = true;
}

void removerEnderecoHabitante(Habitante hab) {
    habitante* h = (habitante*)hab;

    h->morador = false;
    h->enderecoCep[0] = '\0';
    h->enderecoComplemento[0] = '\0';
    h->enderecoFace = '\0';
    h->enderecoNumero = -1;
}

void eliminarHabitante(Habitante hab) {
    free(hab);
}
