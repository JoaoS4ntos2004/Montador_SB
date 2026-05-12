#include <stdio.h>
#include <string.h>
#include "preprocessador.h"
#include "assembler.h"
#include "simulador.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Uso correto: ./montador <arquivo>\n");
        return 1;
    }

    char *nome_arquivo = argv[1];
    
    char *extensao = strrchr(nome_arquivo, '.');

    if (extensao == NULL) {
        printf("Erro: Arquivo sem extensao.\n");
        return 1;
    }

    // Pré-processador .asm -> .pre
    if (strcmp(extensao, ".asm") == 0) {
        printf("Iniciando pre-processamento...\n");
        pre_processar(nome_arquivo);
    } 
    // Montador .pre -> .obj e .pen
    else if (strcmp(extensao, ".pre") == 0) {
        printf("Iniciando montagem...\n");
        montar(nome_arquivo);
    } 
    // Simulador .obj -> execucao
    else if (strcmp(extensao, ".obj") == 0) {
        simular(nome_arquivo);
    } 
    else {
        printf("Erro: Extensao %s nao reconhecida.\n", extensao);
    }

    return 0;
}