//
// Created by natha on 16/03/2026.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "../include/hashfile.h"

#define MAX_CAMINHO_HF 512

#define SLOT_VAZIO 0
#define SLOT_CHEIO 1

#define PROFUNDIDADE_GLOBAL_INICIAL_HF 1
#define NUMERO_BUCKETS_INICIAIS 2

#define CAPACIDADE_INICIAL_EXPANSAO 8

typedef struct {
    size_t record_size;
    size_t key_offset;
    size_t key_size;
    size_t dir_offset;
    int capacidadeBucket;
} hashFileConfig;

typedef struct {
    int profundidadeGlobal;
    int contadorBucket;
    int contadorRegistro;

    hashFileConfig* config;
} hashFileCabecalho;

typedef struct {
    int indiceDiretorio;
    int antigaProfundidadeGlobal;
    int novaProfundidadeGlobal;
}ExpansaoDiretorio;

typedef struct {
    FILE *hfc;
    FILE *hf;

    hashFileConfig* config;
    int profundidadeGlobal;
    int contadorBucket;
    int contadorRegistro;
    long* diretorio;

    ExpansaoDiretorio* expansao;
    int contadorExpansao;
    int capacidadeExpansao;
} hashFile;

uint32_t funcaoHash(void* chave, size_t key_size) {
    uint32_t hash = 2166136261u;
    unsigned char* bytes = (unsigned char*) chave;
    for (size_t i = 0; i < key_size; i++) {
        hash ^= bytes[i];
        hash *= 16777619u;
    }
    return hash;
}

int funcaoComparacao(const void* a, const void* b, size_t size) {
    return memcmp(a,b,size);
}







size_t tamanhoBucketBytes(hashFileConfig* config) {
    return 2 * sizeof(int) + (size_t)config->capacidadeBucket * (1 + config->record_size);
}

char* ponteiroSlot(char* bufferSlots, int i, hashFileConfig* config) {
    return bufferSlots + (size_t)i * (1 + config->record_size);
}

void* getChave(void* registro, hashFileConfig* config) {
    return (char*)registro + config->key_offset;
}

int calculaIndiceDiretorio(void* chave, int profundidade, hashFileConfig* config) {
    int h = funcaoHash(chave, config->record_size);
    return h & ((1u << profundidade) - 1);
}










int leituraBucket(FILE* hf, long offset, int* profundidadeLocal, int* contador, char* bufferSlots, hashFileConfig* config) {
    if (fseek(hf, offset, SEEK_SET) != 0) {
        return -1;
    }

    if (fread(profundidadeLocal, sizeof(int), 1, hf) != 1) {
        return -1;
    }

    if (fread(contador, sizeof(int), 1, hf) != 1) {
        return -1;
    }

    size_t slotsSZ = (size_t)config->capacidadeBucket * (1 + config->record_size);

    if (fread(bufferSlots, 1, slotsSZ, hf) != slotsSZ) {
        return -1;
    }

    return 0;
}

int escritaBucket(FILE* hf, long offset, int profundidadeLocal, int contador, char* bufferSlots, hashFileConfig* config) {
    if (fseek(hf, offset, SEEK_SET) != 0) {
        return -1;
    }

    if (fwrite(&profundidadeLocal, sizeof(int), 1, hf) != 1) {
        return -1;
    }

    if (fwrite(&contador, sizeof(int), 1, hf) != 1) {
        return -1;
    }

    size_t slotsSZ = (size_t)config->capacidadeBucket * (1 + config->record_size);

    if (fwrite(bufferSlots, 1, slotsSZ, hf) != slotsSZ) {
        return -1;
    }

    fflush(hf);

    return 0;
}










int escritaHFC(hashFile* hf) {
    rewind(hf->hfc);

    hashFileCabecalho hfdir;
    hfdir.profundidadeGlobal = hf->profundidadeGlobal;
    hfdir.contadorBucket = hf->contadorBucket;
    hfdir.contadorRegistro = hf->contadorRegistro;
    hfdir.config = hf->config;

    if (fwrite(&hfdir, sizeof(hashFileCabecalho), 1, hf->hfc) != 1) {
        return -1;
    }

    int tamanhoDiretorio = 1 << hf->profundidadeGlobal;

    if (fwrite(hf->diretorio, sizeof(long), (size_t)tamanhoDiretorio, hf->hfc) != (size_t)tamanhoDiretorio) {
        return -1;
    }

    fflush(hf->hfc);
    return 0;
}

int leituraHFC(hashFile* hf) {
    rewind(hf->hfc);

    hashFileCabecalho hfdir;
    if (fread(&hfdir, sizeof(hashFileCabecalho), 1, hf->hfc) != 1) {
        return -1;
    }

    hf->profundidadeGlobal = hfdir.profundidadeGlobal;
    hf->contadorBucket = hfdir.contadorBucket;
    hf->contadorRegistro = hfdir.contadorRegistro;

    int tamanhoDiretorio = 1 << hf->profundidadeGlobal;

    hf->diretorio = malloc((size_t)tamanhoDiretorio * sizeof(long));

    if (hf->diretorio == NULL) {
        return -1;
    }

    if (fread(hf->diretorio, sizeof(long), (size_t)tamanhoDiretorio, hf->hfc) != (size_t)tamanhoDiretorio) {
        free(hf->diretorio);
        hf->diretorio = NULL;
        return -1;
    }

    return 0;
}














int expansaoLog(hashFile* hf, int indiceDiretorio, int profundidadeAntiga, int profundidadeNova) {
    if (hf->contadorExpansao >= hf->capacidadeExpansao) {
        int novaCapacidade = hf->capacidadeExpansao * 2;
        ExpansaoDiretorio* temporario = realloc(hf->expansao, (size_t)novaCapacidade * sizeof(ExpansaoDiretorio));
        if (temporario == NULL) {
            return -1;
        }

        hf->expansao = temporario;
        hf->capacidadeExpansao = novaCapacidade;
    }

    hf->expansao[hf->contadorExpansao].indiceDiretorio = indiceDiretorio;
    hf->expansao[hf->contadorExpansao].antigaProfundidadeGlobal = profundidadeAntiga;
    hf->expansao[hf->contadorExpansao].novaProfundidadeGlobal = profundidadeNova;
    hf->contadorExpansao++;
    return 0;
}









int bucketSplit(hashFile* hf, int indiceDiretorio) {
    hashFileConfig* config = hf->config;
    size_t slotsSZ = (size_t)config->capacidadeBucket * (1 + config->record_size);

    char* slotsAntigo = malloc(slotsSZ);
    if (slotsAntigo == NULL) {
        return -1;
    }

    int profundidadeLocal;
    int contador;

    long antigoOffset = hf->diretorio[indiceDiretorio];

    if (leituraBucket(hf->hf, antigoOffset, &profundidadeLocal, &contador, slotsAntigo, config) != 0) {
        return -1;
    }

    if (profundidadeLocal == hf->profundidadeGlobal) {
        int tamanhoAntigoDiretorio = 1 << hf->profundidadeGlobal;
        int tamanhoNovoDiretorio = tamanhoAntigoDiretorio * 2;

        long* novoDiretorio = realloc(hf->diretorio, (size_t)tamanhoNovoDiretorio * sizeof(long));

        if (novoDiretorio == NULL) {
            free(slotsAntigo);
            return -1;
        }

        hf->diretorio = novoDiretorio;


        int profundidadeAntiga = hf->profundidadeGlobal;
        hf->profundidadeGlobal++;
        expansaoLog(hf, indiceDiretorio, profundidadeAntiga, hf->profundidadeGlobal);
    }

    if (fseek(hf->hf, 0, SEEK_END) != 0) {
        free(slotsAntigo);
        return -1;
    }
    long novoOffset = ftell(hf->hf);

    int novaProfundidadeLocal = profundidadeLocal + 1;

    char* vazio = calloc(slotsSZ, 1);

    if (vazio == NULL) {
        free(slotsAntigo);
        return -1;
    }

    escritaBucket(hf->hf, novoOffset, novaProfundidadeLocal, 0, vazio, config);
    free(vazio);
    hf->contadorBucket++;



    int tamanhoDiretorio = 1 << hf->profundidadeGlobal;
    int distingueBit = novaProfundidadeLocal - 1;
    for (int i = 0; i < tamanhoDiretorio; i++) {
        if (hf->diretorio[i] == antigoOffset && ((i >> distingueBit) & 1)) {
            hf->diretorio[i] = novoOffset;
        }
    }


    char* slotsA = calloc(slotsSZ, 1);
    char* slotsB = calloc(slotsSZ, 1);
    if (slotsA == NULL || slotsB == NULL) {
        free(slotsAntigo);
        free(slotsA);
        free(slotsB);
        return -1;
    }

    int contadorA = 0;
    int contadorB = 0;

    for (int i = 0; i < contador; i++) {
        char* sl = ponteiroSlot(slotsAntigo, i, config);

        if (sl[0] != SLOT_CHEIO) {
            continue;
        }

        void* registro = sl + 1;
        void* chave = getChave(registro, config);
        int indice = calculaIndiceDiretorio(chave, hf->profundidadeGlobal, config);

        if (hf->diretorio[indice] == antigoOffset) {
            char* dst = ponteiroSlot(slotsA, contadorA++, config);
            dst[0] = SLOT_CHEIO;
            memcpy(dst + 1, registro, config->record_size);
        } else {
            char* dst = ponteiroSlot(slotsB, contadorB++, config);
            dst[0] = SLOT_CHEIO;
            memcpy(dst + 1, registro, config->record_size);
        }
    }

    escritaBucket(hf->hf, antigoOffset, novaProfundidadeLocal, contadorA, slotsA, config);
    escritaBucket(hf->hf, novoOffset, novaProfundidadeLocal, contadorB, slotsB, config);

    free(slotsAntigo);
    free(slotsA);
    free(slotsB);

    return escritaHFC(hf);
}














void fazerCaminho(char* buffer, char* base, char* ext) {
    snprintf(buffer, MAX_CAMINHO_HF, "%s%s", base, ext);
}











hashFile* alocarHashStruct(HashFileConfig config) {
    hashFile* hf = calloc(1,sizeof(hashFile));

    if (hf == NULL) {
        return NULL;
    }

    hf->config = (hashFileConfig*)config;

    hf->expansao = malloc(CAPACIDADE_INICIAL_EXPANSAO * sizeof(ExpansaoDiretorio));
    if (hf->expansao == NULL) {
        free(hf);
        return NULL;
    }

    hf->contadorExpansao = 0;
    hf->capacidadeExpansao = CAPACIDADE_INICIAL_EXPANSAO;
    return hf;
}







HashFileConfig criarHashFileConfig(size_t record_size,size_t key_offset,size_t key_size,size_t dir_offset,int capacidade_bucket) {
    hashFileConfig* config = (hashFileConfig*) malloc(sizeof(hashFileConfig));
    if (config == NULL) {
        printf("Erro ao alocar memória ao CriarHashFileConfig!\n");

        perror("Motivo do erro");
        exit(1);
    }

    config->record_size = record_size;
    config->key_offset = key_offset;
    config->key_size = key_size;
    config->dir_offset = dir_offset;
    config->capacidadeBucket = capacidade_bucket;

    return config;
}

void eliminarHashFileConfig(HashFileConfig config) {
    free(config);
}







HashFile criarHashFile(Nome arquivoPath, HashFileConfig config) {
    char caminhoHF[MAX_CAMINHO_HF];
    char caminhoHFC[MAX_CAMINHO_HF];

    fazerCaminho(caminhoHF, arquivoPath, ".hf");
    fazerCaminho(caminhoHFC, arquivoPath, ".hfc");

    hashFile* hf = alocarHashStruct(config);
    if (hf == NULL) {
        return NULL;
    }

    hf->hfc = fopen(caminhoHFC, "w+b");
    hf->hf = fopen(caminhoHF, "w+b");

    if (hf->hfc || hf->hf == NULL) {
        printf("Erro ao abrir o arquivo hf ou hfc!\n");
        if (hf->hfc) {
            fclose(hf->hfc);
        }
        if (hf->hf) {
            fclose(hf->hf);
        }
        free(hf->expansao);
        free(hf);

        perror("Motivo do erro");
        exit(1);
    }


    hashFileConfig* configuracao = hf->config;
    hf->profundidadeGlobal = 1;
    hf->contadorBucket = 2;
    hf->contadorRegistro = 0;

    hf->diretorio = malloc(NUMERO_BUCKETS_INICIAIS * sizeof(long));
    if (hf->diretorio == NULL) {
        fclose(hf->hfc);
        fclose(hf->hf);

        return NULL;
    }


    size_t slotsSZ = (size_t)configuracao->capacidadeBucket * (1 + configuracao->record_size);
    char* vazio = calloc(slotsSZ, 1);
    if (vazio == NULL) {
        free(hf->diretorio);
        fclose(hf->hfc);
        fclose(hf->hf);
        free(hf->expansao);
        free(hf);
        return NULL;
    }

    hf->diretorio[0] = 0;
    escritaBucket(hf->hf,hf->diretorio[0],PROFUNDIDADE_GLOBAL_INICIAL_HF, 0, vazio, configuracao);

    hf->diretorio[1] = (long)tamanhoBucketBytes(configuracao);
    escritaBucket(hf->hf, hf->diretorio[1], PROFUNDIDADE_GLOBAL_INICIAL_HF, 0, vazio, configuracao);

    free(vazio);
    escritaHFC(hf);

    return hf;
}








HashFile abrirHashFile(Nome arquivoPath, HashFileConfig config) {
    char caminhoHF[MAX_CAMINHO_HF];
    char caminhoHFC[MAX_CAMINHO_HF];

    fazerCaminho(caminhoHF, arquivoPath, ".hf");
    fazerCaminho(caminhoHFC, arquivoPath, ".hfc");

    hashFile* hf = alocarHashStruct(config);
    if (hf == NULL) {
        return NULL;
    }

    hf->hfc = fopen(caminhoHFC, "r+b");
    hf->hf = fopen(caminhoHF, "r+b");

    if (hf->hfc || hf->hf == NULL) {
        printf("Erro ao abrir o arquivo hf ou hfc!\n");
        if (hf->hfc) {
            fclose(hf->hfc);
        }
        if (hf->hf) {
            fclose(hf->hf);
        }
        free(hf->expansao);
        free(hf);

        perror("Motivo do erro");
        exit(1);
    }

    if (leituraHFC(hf) != 0) {
        fclose(hf->hfc);
        fclose(hf->hf);
        free(hf->expansao);
        free(hf);
        return NULL;
    }

    return hf;
}





void fecharHashFile(HashFile hf) {
    if (hf == NULL) {
        return;
    }

    hashFile* hash = (hashFile*)hf;
    escritaHFC(hf);
    fclose(hash->hfc);
    fclose(hash->hf);
    free(hash->diretorio);
    free(hash->expansao);
    free(hash);

}




int inserirHashFile(HashFile hf, void* registro) {
    if (hf == NULL || registro == NULL) {
        return -1;
    }

    hashFile* hash = (hashFile*)hf;
    hashFileConfig* config = hash->config;

    void* chave = getChave(registro, config);

    if (buscarHashFile(hf, chave, NULL) == HF_OK) {
        return 0;
    }

    int indice = calculaIndiceDiretorio(chave, hash->profundidadeGlobal, config);
    long offset = hash->diretorio[indice];

    size_t slotsSZ = (size_t)config->capacidadeBucket * (1 + config->record_size);
    char* slots = malloc(slotsSZ);
    if (slots == NULL) {
        return -1;
    }

    int profundidadeLocal;
    int contador;
    if (leituraBucket(hash->hf, offset, &profundidadeLocal, &contador, slots, config) != 0) {
        free(slots);
        return -1;
    }

    if (contador >= config->capacidadeBucket) {
        free(slots);
        if (bucketSplit(hash,indice) != 0) {
            return -1;
        }
        return inserirHashFile(hf, registro);
    }

    char* sl = ponteiroSlot(slots,contador,config);
    sl[0] = SLOT_CHEIO;
    memcpy(sl + 1, registro, config->record_size);
    contador++;

    if (escritaBucket(hash->hf, offset, profundidadeLocal, contador, slots,config) != 0) {
        free(slots);
        return -1;
    }

    free(slots);
    hash->contadorRegistro++;
    return escritaHFC(hash);
}







int buscarHashFile(HashFile hf, void* chave, void* registro_clone) {
    if (hf == NULL || chave == NULL) {
        return -1;
    }

    hashFile* hash = (hashFile*)hf;
    hashFileConfig* config = hash->config;

    int indice = calculaIndiceDiretorio(chave, hash->profundidadeGlobal, config);
    long offset = hash->diretorio[indice];

    size_t slotsSZ = (size_t)config->capacidadeBucket * (1 + config->record_size);
    char* slots = malloc(slotsSZ);
    if (slots == NULL) {
        return -1;
    }

    int profundidadeLocal;
    int contador;
    if (leituraBucket(hash->hf, offset, &profundidadeLocal, &contador, slots, config) != 0) {
        free(slots);
        return -1;
    }

    for (int i = 0; i < contador; i++) {
        char* sl = ponteiroSlot(slots, i, config);
        void* chaveRegistro = getChave(sl + 1, config);

        if (sl[0] == SLOT_CHEIO && funcaoComparacao(chave, chaveRegistro, config->key_size) == 0) {
            if (registro_clone != NULL) {
                memcpy(registro_clone, sl + 1, config->record_size);
            }
            free(slots);
            return 0;
        }
    }

    free(slots);
    return -1;
}









int removerHashFile(HashFile hf, void* chave, void* registro_clone) {
    if (hf == NULL || chave == NULL) {
        return -1;
    }

    hashFile* hash = (hashFile*)hf;
    hashFileConfig* config = hash->config;

    int indice = calculaIndiceDiretorio(chave, hash->profundidadeGlobal, config);
    long offset = hash->diretorio[indice];

    size_t slotsSZ = (size_t)config->capacidadeBucket * (1 + config->record_size);
    char* slots = malloc(slotsSZ);
    if (slots == NULL) {
        return -1;
    }

    int profundidadeLocal;
    int contador;
    if (leituraBucket(hash->hf, offset, &profundidadeLocal, &contador, slots, config) != 0) {
        free(slots);
        return -1;
    }

    for (int i = 0; i < contador; i++) {
        char* sl = ponteiroSlot(slots, i, config);
        void* chaveRegistro = getChave(sl + 1, config);

        if (sl[0] == SLOT_CHEIO && funcaoComparacao(chave, chaveRegistro, config->key_size) == 0) {
            if (registro_clone != NULL) {
                memcpy(registro_clone, sl + 1, config->record_size);
            }

            if (i < contador - 1) {
                char* ultimo = ponteiroSlot(slots, contador - 1, config);
                memcpy(sl, ultimo, 1 + config->record_size);
                ultimo[0] = SLOT_VAZIO;
            } else {
                sl[0] = SLOT_VAZIO;
            }
            contador--;

            escritaBucket(hash->hf, offset, profundidadeLocal, contador, slots, config);
            free(slots);
            hash->contadorRegistro--;

            return escritaHFC(hash);
        }
    }

    free(slots);
    return 0;
}







int atualizarHashFile(HashFile hf, void* registro_novo) {
    if (hf == NULL || registro_novo == NULL) {
        return -1;
    }

    hashFile* hash = (hashFile*)hf;
    hashFileConfig* config = hash->config;

    void* chave = getChave(registro_novo,config);
    int indice = calculaIndiceDiretorio(chave, hash->profundidadeGlobal, config);
    long offset = hash->diretorio[indice];

    size_t slotsSZ = (size_t)config->capacidadeBucket * (1 + config->record_size);
    char* slots = malloc(slotsSZ);
    if (slots == NULL) {
        return -1;
    }

    int profundidadeLocal;
    int contador;

    if (leituraBucket(hash->hf, offset, &profundidadeLocal, &contador, slots, config) != 0) {
        free(slots);
        return -1;
    }

    for (int i = 0; i < contador; i++) {
        char* sl = ponteiroSlot(slots, i, config);
        void* chaveRegistro = getChave(sl + 1, config);

        if (sl[0] == SLOT_CHEIO && funcaoComparacao(chave, chaveRegistro, config->key_size) == 0) {
            memcpy(sl + 1,registro_novo, config->record_size);
            escritaBucket(hash->hf, offset, profundidadeLocal, contador, slots, config);
            free(slots);
            return 0;
        }
    }

    free(slots);
    return 0;
}






int getProfundidadeGlobalHF(HashFile hf) {
    if (hf == NULL) {
        return -1;
    }

    return ((hashFile*)hf)->profundidadeGlobal;

}

int getNumeroBucketsHF(HashFile hf) {
    if (hf == NULL) {
        return -1;
    }

    return ((hashFile*)hf)->contadorBucket;

}

int getNumeroRegistrosHF(HashFile hf) {
    if (hf == NULL) {
        return -1;
    }

    return ((hashFile*)hf)->contadorRegistro;
}
