#include <stdio.h>
#include <string.h>
#include "preprocessador.h"

int main(int argc, char *argv[]) {
    // Verifica se o usuário passou o arquivo
    if (argc != 2) {
        printf("Uso correto: ./montador <arquivo>\n");
        return 1;
    }

    char *nome_arquivo = argv[1];
    
    // Pega a extensão do arquivo para saber em qual modo rodar
    char *extensao = strrchr(nome_arquivo, '.');

    if (extensao == NULL) {
        printf("Erro: Arquivo sem extensao.\n");
        return 1;
    }

    // Modo 1: Pré-processador (.asm -> .pre)
    if (strcmp(extensao, ".asm") == 0) {
        printf("Iniciando pre-processamento...\n");
        pre_processar(nome_arquivo);
    } 
    // Futuro Modo 2: Montador (.pre -> .obj e .pen)
    else if (strcmp(extensao, ".pre") == 0) {
        printf("Modo montador ainda nao implementado!\n");
    } 
    // Futuro Modo 3: Simulador (.obj -> execucao)
    else if (strcmp(extensao, ".obj") == 0) {
        printf("Modo simulador ainda nao implementado!\n");
    } 
    else {
        printf("Erro: Extensao %s nao reconhecida.\n", extensao);
    }

    return 0;
}