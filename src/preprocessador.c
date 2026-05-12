#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "preprocessador.h"

// Estrutura para armazenar as diretivas EQU
typedef struct {
    char nome[50];
    char valor[20];
} Equ;

// Variáveis globais restritas a este arquivo
static Equ tabela_equ[100];
static int qtd_equ = 0;

// Função auxiliar para remover espaços ao redor das vírgulas (exigência do COPY)
void formatar_virgulas(char *linha) {
    char temp[256];
    int j = 0;
    for (int i = 0; linha[i] != '\0'; i++) {
        // Pula espaço se estiver imediatamente antes ou depois de uma vírgula
        if (linha[i] == ' ' && i > 0 && linha[i-1] == ',') continue;
        if (linha[i] == ' ' && linha[i+1] == ',') continue;
        temp[j++] = linha[i];
    }
    temp[j] = '\0';
    strcpy(linha, temp);
}

// Função auxiliar para buscar valor de um rótulo no EQU
const char* buscar_equ(const char *nome) {
    for (int i = 0; i < qtd_equ; i++) {
        if (strcmp(tabela_equ[i].nome, nome) == 0) {
            return tabela_equ[i].valor;
        }
    }
    return NULL; // Retorna NULL se não achar
}

int pre_processar(const char *nome_arquivo_entrada) {
    FILE *arquivo_asm = fopen(nome_arquivo_entrada, "r");
    if (!arquivo_asm) {
        printf("Erro: Nao foi possivel abrir o arquivo %s\n", nome_arquivo_entrada);
        return 0;
    }

    // Geração do nome do arquivo de saída (.pre) [cite: 15]
    char nome_arquivo_saida[256];
    strcpy(nome_arquivo_saida, nome_arquivo_entrada);
    char *extensao = strrchr(nome_arquivo_saida, '.');
    if (extensao != NULL) {
        strcpy(extensao, ".pre");
    } else {
        strcat(nome_arquivo_saida, ".pre");
    }

    // Buffers para reorganizar as seções [cite: 23]
    char secao_text[10000] = "";
    char secao_data[10000] = "";
    int na_secao_text = 1; // Flag padrão (1 = TEXT, 0 = DATA)

    char linha[256];
    int pular_proxima_linha = 0;
    char rotulo_pendente[100] = "";

    while (fgets(linha, sizeof(linha), arquivo_asm) != NULL) {
        // 1. Remover comentários (tudo após ';') [cite: 30, 31]
        char *comentario = strchr(linha, ';');
        if (comentario != NULL) {
            *comentario = '\0';
        }

        // 2. Converter para maiúsculas (Case Insensitive) [cite: 25]
        for (int i = 0; linha[i] != '\0'; i++) {
            linha[i] = toupper(linha[i]);
        }

        // 3. Tokenização da linha para remover espaços/tabs extras [cite: 28]
        char tokens[10][50];
        int num_tokens = 0;
        char *token = strtok(linha, " \t\r\n");

        while (token != NULL && num_tokens < 10) {
            strcpy(tokens[num_tokens], token);
            num_tokens++;
            token = strtok(NULL, " \t\r\n");
        }

        if (num_tokens == 0) continue; // Linha vazia, ignora

        // Verifica flag do IF [cite: 31]
        if (pular_proxima_linha) {
            pular_proxima_linha = 0;
            continue; 
        }

        // 4. Tratamento de Diretivas de Pré-processamento (EQU e IF)
        // Cenário EQU: ROTULO: EQU VALOR
        if (num_tokens >= 3 && strcmp(tokens[1], "EQU") == 0) {
            char nome_rotulo[50];
            strcpy(nome_rotulo, tokens[0]);
            // Remove o ':' do final do nome do rótulo para salvar na tabela
            if (nome_rotulo[strlen(nome_rotulo) - 1] == ':') {
                nome_rotulo[strlen(nome_rotulo) - 1] = '\0';
            }
            strcpy(tabela_equ[qtd_equ].nome, nome_rotulo);
            strcpy(tabela_equ[qtd_equ].valor, tokens[2]);
            qtd_equ++;
            continue; // EQU não vai para o arquivo .pre
        }

        // Cenário IF: IF VALOR_OU_ROTULO
        if (num_tokens == 2 && strcmp(tokens[0], "IF") == 0) {
            const char *valor_if = buscar_equ(tokens[1]);
            // Se o valor não existir no EQU, assumimos o próprio token (pode ser "1" ou "0" direto)
            if (valor_if == NULL) valor_if = tokens[1]; 
            
            if (strcmp(valor_if, "1") != 0 && strcmp(valor_if, "0X01") != 0) {
                pular_proxima_linha = 1; // Se não for 1, pula a próxima linha
            }
            continue; // IF não vai para o arquivo .pre
        }

        // 5. Identificação das Seções
        if (num_tokens >= 2 && strcmp(tokens[0], "SECTION") == 0) {
            if (strcmp(tokens[1], "TEXT") == 0) {
                na_secao_text = 1;
            } else if (strcmp(tokens[1], "DATA") == 0) {
                na_secao_text = 0;
            }
            continue; // Não vamos salvar as linhas "SECTION", vamos recriá-las na escrita
        }

        // 6. Tratamento de Rótulos Sozinhos na Linha [cite: 29]
        if (num_tokens == 1 && tokens[0][strlen(tokens[0]) - 1] == ':') {
            strcpy(rotulo_pendente, tokens[0]);
            strcat(rotulo_pendente, " "); // Adiciona um espaço para concatenar com a próxima instrução
            continue; 
        }

        // 7. Montagem da Linha Limpa
        char linha_limpa[256] = "";
        
        // Se tinha um rótulo na linha anterior, coloca ele na frente
        if (strlen(rotulo_pendente) > 0) {
            strcpy(linha_limpa, rotulo_pendente);
            rotulo_pendente[0] = '\0'; // Limpa a pendência
        }

        for (int i = 0; i < num_tokens; i++) {
            // Substituição de EQU no código (exceto se for definição de rótulo terminada em ':')
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

        // Aplica a regra de juntar argumentos com vírgula (ex: COPY A,B) [cite: 20, 27]
        formatar_virgulas(linha_limpa);
        strcat(linha_limpa, "\n"); // Adiciona quebra de linha no final

        // 8. Separação de TEXT e DATA
        if (na_secao_text) {
            strcat(secao_text, linha_limpa);
        } else {
            strcat(secao_data, linha_limpa);
        }
    }

    fclose(arquivo_asm);

    // 9. Escrita no arquivo .pre garantindo DATA por último 
    FILE *arquivo_pre = fopen(nome_arquivo_saida, "w");
    if (!arquivo_pre) {
        printf("Erro: Nao foi possivel criar o arquivo %s\n", nome_arquivo_saida);
        return 0;
    }

    // Escreve a seção TEXT
    if (strlen(secao_text) > 0) {
        fprintf(arquivo_pre, "SECTION TEXT\n");
        fprintf(arquivo_pre, "%s", secao_text);
    }

    // Escreve a seção DATA
    if (strlen(secao_data) > 0) {
        fprintf(arquivo_pre, "SECTION DATA\n");
        fprintf(arquivo_pre, "%s", secao_data);
    }

    fclose(arquivo_pre);
    printf("Pre-processamento concluido! Arquivo gerado: %s\n", nome_arquivo_saida);

    return 1;
}