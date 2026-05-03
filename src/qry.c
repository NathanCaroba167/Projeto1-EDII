//
// Created by natha on 09/10/2025.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <math.h>

#include "../include/txt.h"
#include "../include/qry.h"

#include "../include/habitante.h"
#include "../include/quadra.h"
#include "../include/svg.h"

#define TAMANHO_MAX_BUFFER 256
#define TAMANHO_MAX_BUFFER_REGISTRO 1024
#define MAX_CPF 20

typedef struct {
    char* cepRemovido;
    char** cpfs;
    int contador;
    int capacidade;
} ColetaMoradores;

typedef struct {
    char* cepAnalisado;
    int norte;
    int sul;
    int leste;
    int oeste;
} ContagemMoradores;

typedef struct {
    int totalHabitantes;
    int totalMoradores;
    int homens;
    int mulheres;
    int moradoresHomens;
    int moradoresMulheres;
    int semTeto;
    int semTetoHomens;
    int semTetoMulheres;
} Censo;

void callbackColetaMoradores(void* registro, void* structAuxiliar) {
    ColetaMoradores* cm =(ColetaMoradores*)structAuxiliar;
    Habitante hab = registroParaHabitanteBuffer(registro);
    if (hab == NULL) {
        return;
    }

    if (isMorador(hab) && strcmp(getCEPEnderecoHabitante(hab), cm->cepRemovido) == 0) {

        if (cm->contador >= cm->capacidade) {
            cm->capacidade *= 2;
            cm->cpfs = realloc(cm->cpfs, (size_t)cm->capacidade * sizeof(char*));
        }

        cm->cpfs[cm->contador] = malloc(MAX_CPF);
        strncpy(cm->cpfs[cm->contador], getCPFHabitante(hab), MAX_CPF - 1);
        cm->cpfs[cm->contador][MAX_CPF - 1] = '\0';
        cm->contador++;
    }
    eliminarHabitante(hab);
}

void callbackContagemMoradores(void* registro, void* structAuxiliar) {
    ContagemMoradores* cm =(ContagemMoradores*)structAuxiliar;
    Habitante hab = registroParaHabitanteBuffer(registro);
    if (hab == NULL) {
        return;
    }

    if (isMorador(hab) && strcmp(getCEPEnderecoHabitante(hab),cm->cepAnalisado) == 0) {
        char face = getFaceEnderecoHabitante(hab);
        if (face == 'N') {
            cm->norte++;
        }else if (face == 'S') {
            cm->sul++;
        }else if (face == 'L') {
            cm->leste++;
        }else if (face == 'O') {
            cm->oeste++;
        }
    }
    eliminarHabitante(hab);
}

void callbackCenso(void* registro, void* structAuxiliar) {
    Censo* c = (Censo*)structAuxiliar;
    Habitante hab = registroParaHabitanteBuffer(registro);
    if (hab == NULL) {
        return;
    }

    c->totalHabitantes++;
    char sexo = getSexoHabitante(hab);
    if (sexo == 'M') {
        c->homens++;
    }else {
        c->mulheres++;
    }

    if (isMorador(hab)) {
        c->totalMoradores++;
        if (sexo == 'M') {
            c->moradoresHomens++;
        }else {
            c->moradoresMulheres++;
        }
    }else {
        c->semTeto++;
        if (sexo == 'M') {
            c->semTetoHomens++;
        }else {
            c->semTetoMulheres++;
        }
    }
    eliminarHabitante(hab);
}

void posicaoFaceNaQuadra(double x, double y, double w, double h, char face, double *px, double *py) {
    switch (face) {
        case 'N':
            *px = x + w / 2.0;
            *py = y - h;
            break;
        case 'S':
            *px = x + w / 2.0;
            *py = y;
            break;
        case 'L':
            *px = x + w;
            *py = y - h / 2.0;
            break;
        case 'O':
            *px = x;
            *py = y - h / 2.0;
            break;
        default:
            printf("Lado de face incorreto!!\n");
            break;
    }
}

void comandoRq(char* cep, HashFile hfQuadras, HashFile hfHabitantes, Arquivo svg, Arquivo txt) {
    char buffer[TAMANHO_MAX_BUFFER_REGISTRO];

    if (buscarHashFile(hfQuadras, cep, buffer) != 0) {
        return;
    }
    Quadra qdr = registroParaQuadraBuffer(buffer);
    if (qdr == NULL) {
        return;
    }

    double x = getXQuadra(qdr);
    double y = getYQuadra(qdr);
    double h = getHQuadra(qdr);
    eliminarQuadra(qdr);

    if (removerHashFile(hfQuadras, cep, NULL) != 0) {
        return;
    }

    ColetaMoradores cm;
    cm.cepRemovido = cep;
    cm.contador = 0;
    cm.capacidade = 8;
    cm.cpfs = malloc((size_t)cm.capacidade * sizeof(char*));
    if (cm.cpfs == NULL) {
        return;
    }

    percorrerHash(hfHabitantes, callbackColetaMoradores, &cm);

    for (int i = 0; i < cm.contador; i++) {
        if (buscarHashFile(hfHabitantes, cm.cpfs[i], buffer) == 0) {
            Habitante hab = registroParaHabitanteBuffer(buffer);
            if (hab != NULL) {
                fprintf(txt, "%s %s\n", getCPFHabitante(hab), getNomeHabitante(hab));
                removerEnderecoHabitante(hab);
                habitanteBufferParaRegistro(hab, buffer);
                atualizarHashFile(hfHabitantes, buffer);
                eliminarHabitante(hab);
            }
        }
        free(cm.cpfs[i]);
    }
    free(cm.cpfs);

    if (svg) {
        desenharXVermelhoSVG(svg, x, y - h);
    }
}

void comandoPq(char* cep, HashFile hfQuadras, HashFile hfHabitantes, Arquivo svg, Arquivo txt) {
    char buffer[TAMANHO_MAX_BUFFER_REGISTRO];

    if (buscarHashFile(hfQuadras, cep, buffer) != 0) {
        return;
    }
    Quadra qdr = registroParaQuadraBuffer(buffer);
    if (qdr == NULL) {
        return;
    }

    double x = getXQuadra(qdr);
    double y = getYQuadra(qdr);
    double w = getWQuadra(qdr);
    double h = getHQuadra(qdr);
    eliminarQuadra(qdr);

    ContagemMoradores cm = { cep, 0, 0, 0 , 0};
    percorrerHash(hfHabitantes,callbackContagemMoradores, &cm);
    int total = cm.norte + cm.sul + cm.leste + cm.oeste;

    fprintf(txt, "N:%d S:%d L:%d O:%d Total:%d\n", cm.norte, cm.sul, cm.leste, cm.oeste, total);

    if (svg) {
        char stringNumero[16];
        double px;
        double py;

        char faces[] = {'N', 'S', 'L', 'O'};
        int contador[] = {cm.norte, cm.sul, cm.leste, cm.oeste};

        for (int i = 0; i < 4; i++) {
            posicaoFaceNaQuadra(x, y, w, h, faces[i], &px, &py);
            snprintf(stringNumero, sizeof(stringNumero), "%d", contador[i]);
            desenharTextoSVG(svg, px, py, stringNumero, "black", 5);
        }
        snprintf(stringNumero, sizeof(stringNumero), "%d", total);
        desenharTextoSVG(svg, x + w / 2.0, y - h / 2.0, stringNumero, "black", 7);
    }
}

void comandoCenso(HashFile hfHabitantes, Arquivo txt) {
    Censo c = {0};
    percorrerHash(hfHabitantes, callbackCenso, &c);

    escreverCenso(txt, c.totalHabitantes, c.totalMoradores, c.homens, c.mulheres, c.moradoresHomens, c.moradoresMulheres, c.semTeto, c.semTetoHomens, c.semTetoMulheres);
}

void comandoHinterrogacao(char* cpf, HashFile hfHabitante, Arquivo txt){
    char buffer[TAMANHO_MAX_BUFFER_REGISTRO];

    if (buscarHashFile(hfHabitante, cpf, buffer) != 0) {
        fprintf(txt, "Habitante nao encontrado\n");
        return;
    }

    Habitante hab = registroParaHabitanteBuffer(buffer);
    if (hab == NULL) {
        return;
    }

    escreverHabitante(txt, getCPFHabitante(hab), getNomeHabitante(hab), getSobrenomeHabitante(hab), getSexoHabitante(hab), getNascHabitante(hab), isMorador(hab), getCEPEnderecoHabitante(hab), getFaceEnderecoHabitante(hab), getNumeroEnderecoHabitante(hab), getComplementoHabitante(hab));

    eliminarHabitante(hab);

}

void comandoNasc(char* cpf, char* nome, char* sobrenome, char sexo, char* nascimento, HashFile hfHabitantes, Arquivo txt){
    Habitante hab = criarHabitante(cpf, nome, sobrenome, sexo, nascimento);
    if (hab == NULL) {
        return;
    }

    char buffer[TAMANHO_MAX_BUFFER_REGISTRO];
    habitanteBufferParaRegistro(hab,buffer);
    inserirHashFile(hfHabitantes, buffer);
    eliminarHabitante(hab);

    fprintf(txt, "Nascimento habitante: %s\n", cpf);
}

void comandoRip(char* cpf, HashFile hfQuadras, HashFile hfHabitantes, Arquivo svg, Arquivo txt) {
    char buffer[TAMANHO_MAX_BUFFER_REGISTRO];

    if (buscarHashFile(hfHabitantes, cpf, buffer) != 0) {
        fprintf(txt, "Habitante nao encontrado\n");
        return;
    }

    Habitante hab = registroParaHabitanteBuffer(buffer);
    if (hab == NULL) {
        return;
    }

    escreverHabitante(txt, getCPFHabitante(hab), getNomeHabitante(hab), getSobrenomeHabitante(hab), getSexoHabitante(hab), getNascHabitante(hab), isMorador(hab), getCEPEnderecoHabitante(hab), getFaceEnderecoHabitante(hab), getNumeroEnderecoHabitante(hab), getComplementoHabitante(hab));

    if (svg && isMorador(hab)) {
        char bufferQuadra[TAMANHO_MAX_BUFFER_REGISTRO];
        if (buscarHashFile(hfQuadras, getCEPEnderecoHabitante(hab), bufferQuadra) == 0) {
            Quadra q = registroParaQuadraBuffer(bufferQuadra);
            if (q != NULL) {
                double px;
                double py;
                posicaoFaceNaQuadra(getXQuadra(q), getYQuadra(q), getWQuadra(q), getHQuadra(q),getFaceEnderecoHabitante(hab), &px, &py);
                desenharCruzSVG(svg, px, py);
                eliminarQuadra(q);
            }
        }
    }

    removerHashFile(hfHabitantes, cpf, NULL);
    eliminarHabitante(hab);
}

void comandoMud(char* cpf, char* cep, char face, int numero, char* complemento, HashFile hfQuadras, HashFile hfHabitantes, Arquivo svg, Arquivo txt){
    char buffer[TAMANHO_MAX_BUFFER_REGISTRO];

    if (buscarHashFile(hfHabitantes, cpf, buffer) != 0) {
        return;
    }

    Habitante hab = registroParaHabitanteBuffer(buffer);
    if (hab == NULL) {
        return;
    }

    setEnderecoHabitante(hab, cep, face, numero, complemento);
    habitanteBufferParaRegistro(hab, buffer);
    atualizarHashFile(hfHabitantes, buffer);

    if (svg) {
        char bufferQuadra[TAMANHO_MAX_BUFFER_REGISTRO];
        if (buscarHashFile(hfQuadras, cep, bufferQuadra) == 0) {
            Quadra q = registroParaQuadraBuffer(bufferQuadra);
            if (q != NULL) {
                double px;
                double py;
                posicaoFaceNaQuadra(getXQuadra(q), getYQuadra(q), getWQuadra(q), getHQuadra(q), face, &px, &py);
                desenharQuadradoVermelhoSVG(svg, px, py, cpf);
                eliminarQuadra(q);
            }
        }
    }
    fprintf(txt, "Mudanca do Morador: %s\n", cpf);
    eliminarHabitante(hab);
}

void comandoDspj(char* cpf, HashFile hfQuadras, HashFile hfHabitantes, Arquivo svg, Arquivo txt) {
    char buffer[TAMANHO_MAX_BUFFER_REGISTRO];

    if (buscarHashFile(hfHabitantes, cpf, buffer) != 0) {
        fprintf(txt, "Habitante nao encontrado\n");
        return;
    }

    Habitante hab = registroParaHabitanteBuffer(buffer);
    if (hab == NULL) {
        return;
    }
    printf("DEBUB: Endereco: %s/%c/%d %s\n", getCEPEnderecoHabitante(hab), getFaceEnderecoHabitante(hab), getNumeroEnderecoHabitante(hab), getComplementoHabitante(hab));

    escreverHabitante(txt, getCPFHabitante(hab), getNomeHabitante(hab), getSobrenomeHabitante(hab), getSexoHabitante(hab), getNascHabitante(hab), isMorador(hab), getCEPEnderecoHabitante(hab), getFaceEnderecoHabitante(hab), getNumeroEnderecoHabitante(hab), getComplementoHabitante(hab));

    if (svg && isMorador(hab)) {
        char bufferQuadra[TAMANHO_MAX_BUFFER_REGISTRO];
        if (buscarHashFile(hfQuadras, getCEPEnderecoHabitante(hab), bufferQuadra) == 0) {
            Quadra q = registroParaQuadraBuffer(bufferQuadra);
            if (q != NULL) {
                double px;
                double py;
                posicaoFaceNaQuadra(getXQuadra(q), getYQuadra(q), getWQuadra(q), getHQuadra(q),getFaceEnderecoHabitante(hab), &px, &py);
                desenharCirculoSVG(svg, px, py);
                eliminarQuadra(q);
            }
        }
    }

    removerEnderecoHabitante(hab);
    habitanteBufferParaRegistro(hab,buffer);
    atualizarHashFile(hfHabitantes, buffer);
    eliminarHabitante(hab);

}

Arquivo abrirQry(Nome arquivo) {
    Arquivo qry = fopen(arquivo,"r");
    if (qry == NULL) {
        printf("Erro ao abrir o arquivo qry!\n");

        perror("Motivo do erro");
        exit(1);
    }
    return qry;
}

void LerComandosExecutar(Arquivo svg,Arquivo txt,Arquivo qry,HashFile hfQuadras,HashFile hfHabitantes) {
    char buffer[TAMANHO_MAX_BUFFER];
    int executados = 0;

    while(fgets(buffer,sizeof(buffer),qry) != NULL) {

        buffer[strcspn(buffer,"\n")] = 0;

        if (buffer[0] == '\n' || buffer[0] == '\0') {
            continue;
        }

        fprintf(txt,"\n[*] %s\n", buffer);

        char* comando = strtok(buffer," ");
        if (comando == NULL) {
            continue;
        }

        if (strcmp(comando, "rq") == 0) {
            char* cep = strtok(NULL," ");

            if (cep) {
                comandoRq(cep, hfQuadras, hfHabitantes, svg, txt);
                executados++;
            }
        }else if (strcmp(comando, "pq") == 0) {
            char* cep = strtok(NULL," ");

            if (cep) {
                comandoPq(cep, hfQuadras, hfHabitantes, svg, txt);
                executados++;
            }
        }else if (strcmp(comando, "censo") == 0) {
            comandoCenso(hfHabitantes, txt);
            executados++;
        }else if (strcmp(comando, "h?") == 0) {
            char* cpf = strtok(NULL," ");

            if (cpf) {
                comandoHinterrogacao(cpf, hfHabitantes, txt);
                executados++;
            }
        }else if (strcmp(comando, "nasc") == 0) {
            char* cpf = strtok(NULL," ");
            char* nome = strtok(NULL," ");
            char* sobrenome = strtok(NULL," ");
            char* sexo_temp = strtok(NULL," ");
            char* nascimento = strtok(NULL," ");

                if (cpf && nome && sobrenome && sexo_temp && nascimento) {
                    char sexo = sexo_temp[0];
                    comandoNasc(cpf, nome, sobrenome, sexo, nascimento, hfHabitantes, txt);
                    executados++;
                }
        }else if (strcmp(comando, "rip") == 0) {
            char* cpf = strtok(NULL," ");

            if (cpf) {
                comandoRip(cpf, hfQuadras, hfHabitantes, svg, txt);
                executados++;
            }
        }else if (strcmp(comando, "mud") == 0) {
            char* cpf = strtok(NULL," ");
            char* cep = strtok(NULL," ");
            char* face_temp = strtok(NULL," ");
            char* numero_temp = strtok(NULL," ");
            char* complemento = strtok(NULL," ");

            if (cpf && cep && face_temp && numero_temp && complemento) {
                char face = face_temp[0];
                int numero = atoi(numero_temp);

                while (*complemento == ' ') {
                    complemento++;
                }

                comandoMud(cpf, cep, face, numero, complemento, hfQuadras, hfHabitantes, svg, txt);
                executados++;
            }
        }else if (strcmp(comando, "dspj") == 0) {
            char* cpf = strtok(NULL," ");

            if (cpf) {
                comandoDspj(cpf, hfQuadras, hfHabitantes, svg, txt);
                executados++;
            }
        }else{
            printf("Comando desconhecido: '%s' \n", comando);
        }
    }
}