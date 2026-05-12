#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "preprocessador.h"

// armazenar as diretivas EQU
typedef struct {
    char nome[50];
    char valor[20];
} Equ;

// Variáveis globais 
static Equ tabela_equ[100];
static int qtd_equ = 0;

// remover espaço do lado das vírgulas(COPY)
void formatar_virgulas(char *linha) {
    char temp[256];
    int j = 0;
    for (int i = 0; linha[i] != '\0'; i++) {
        if (linha[i] == ' ' && i > 0 && linha[i-1] == ',') continue;
        if (linha[i] == ' ' && linha[i+1] == ',') continue;
        temp[j++] = linha[i];
    }
    temp[j] = '\0';
    strcpy(linha, temp);
}

const char* buscar_equ(const char *nome) {
    for (int i = 0; i < qtd_equ; i++) {
        if (strcmp(tabela_equ[i].nome, nome) == 0) {
            return tabela_equ[i].valor;
        }
    }
    return NULL; // 
}

int pre_processar(const char *nome_arquivo_entrada) {
    FILE *arquivo_asm = fopen(nome_arquivo_entrada, "r");
    if (!arquivo_asm) {
        printf("Erro: Nao foi possivel abrir o arquivo %s\n", nome_arquivo_entrada);
        return 0;
    }
    char nome_arquivo_saida[256];
    strcpy(nome_arquivo_saida, nome_arquivo_entrada);
    char *extensao = strrchr(nome_arquivo_saida, '.');
    if (extensao != NULL) {
        strcpy(extensao, ".pre");
    } else {
        strcat(nome_arquivo_saida, ".pre");
    }

    // reserva espaço para ordenar as seções
    char secao_text[10000] = "";
    char secao_data[10000] = "";
    int na_secao_text = 1; // 1 = TEXT, 0 = DATA

    char linha[256];
    int pular_proxima_linha = 0;
    char rotulo_pendente[100] = "";

    while (fgets(linha, sizeof(linha), arquivo_asm) != NULL) {
        // Remover comentários 
        char *comentario = strchr(linha, ';');
        if (comentario != NULL) {
            *comentario = '\0';
        }

        // Case Insensitive
        for (int i = 0; linha[i] != '\0'; i++) {
            linha[i] = toupper(linha[i]);
        }

        // separa linha em tokens
        char tokens[10][50];
        int num_tokens = 0;
        char *token = strtok(linha, " \t\r\n");

        while (token != NULL && num_tokens < 10) {
            strcpy(tokens[num_tokens], token);
            num_tokens++;
            token = strtok(NULL, " \t\r\n");
        }

        if (num_tokens == 0) continue; 

        // IF
        if (pular_proxima_linha) {
            pular_proxima_linha = 0;
            continue; 
        }

        // EQU e IF
        // EQU: ROTULO: EQU VALOR
        if (num_tokens >= 3 && strcmp(tokens[1], "EQU") == 0) {
            char nome_rotulo[50];
            strcpy(nome_rotulo, tokens[0]);
            if (nome_rotulo[strlen(nome_rotulo) - 1] == ':') {
                nome_rotulo[strlen(nome_rotulo) - 1] = '\0';
            }
            strcpy(tabela_equ[qtd_equ].nome, nome_rotulo);
            strcpy(tabela_equ[qtd_equ].valor, tokens[2]);
            qtd_equ++;
            continue; // EQU não vai para o arquivo .pre
        }

        // IF: IF VALOR_OU_ROTULO
        if (num_tokens == 2 && strcmp(tokens[0], "IF") == 0) {
            const char *valor_if = buscar_equ(tokens[1]);
            if (valor_if == NULL) valor_if = tokens[1]; 
            
            if (strcmp(valor_if, "1") != 0 && strcmp(valor_if, "0X01") != 0) {
                pular_proxima_linha = 1; // Se não for 1, pula a próxima linha
            }
            continue; // IF não vai para o arquivo .pre
        }

        // Seções data/text
        if (num_tokens >= 2 && strcmp(tokens[0], "SECTION") == 0) {
            if (strcmp(tokens[1], "TEXT") == 0) {
                na_secao_text = 1;
            } else if (strcmp(tokens[1], "DATA") == 0) {
                na_secao_text = 0;
            }
            continue; 
        }

        // rotulos sozinhos na msm linha
        if (num_tokens == 1 && tokens[0][strlen(tokens[0]) - 1] == ':') {
            strcpy(rotulo_pendente, tokens[0]);
            strcat(rotulo_pendente, " "); 
            continue; 
        }

        char linha_limpa[256] = "";
        
        // prioridade de rotulo
        if (strlen(rotulo_pendente) > 0) {
            strcpy(linha_limpa, rotulo_pendente);
            rotulo_pendente[0] = '\0'; 
        }

        for (int i = 0; i < num_tokens; i++) {
            if (tokens[i][strlen(tokens[i]) - 1] != ':') {
                const char *valor_equ = buscar_equ(tokens[i]);
                if (valor_equ != NULL) {
                    strcat(linha_limpa, valor_equ);
                } else {
                    strcat(linha_limpa, tokens[i]);
                }
            } else {
                strcat(linha_limpa, tokens[i]);
            }

            if (i < num_tokens - 1) strcat(linha_limpa, " ");
        }

        // argumentos com vírgula COPY A,B
        formatar_virgulas(linha_limpa);
        strcat(linha_limpa, "\n"); 

        if (na_secao_text) {
            strcat(secao_text, linha_limpa);
        } else {
            strcat(secao_data, linha_limpa);
        }
    }

    fclose(arquivo_asm);

    // DATA por último 
    FILE *arquivo_pre = fopen(nome_arquivo_saida, "w");
    if (!arquivo_pre) {
        printf("Erro: Nao foi possivel criar o arquivo %s\n", nome_arquivo_saida);
        return 0;
    }

    // seção TEXT
    if (strlen(secao_text) > 0) {
        fprintf(arquivo_pre, "SECTION TEXT\n");
        fprintf(arquivo_pre, "%s", secao_text);
    }

    // seção DATA
    if (strlen(secao_data) > 0) {
        fprintf(arquivo_pre, "SECTION DATA\n");
        fprintf(arquivo_pre, "%s", secao_data);
    }

    fclose(arquivo_pre);
    printf("Pre-processamento concluido! Arquivo gerado: %s\n", nome_arquivo_saida);

    return 1;
}