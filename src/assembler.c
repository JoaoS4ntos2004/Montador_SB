#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "assembler.h"

// tabela de simbolos global
static Simbolo tabela_simbolos[200];
static int qtd_simbolos = 0;

// Tabelas de instruções
const char *instrucoes[] = {"ADD", "SUB", "MULT", "DIV", "JMP", "JMPN", "JMPP", "JMPZ", "COPY", "LOAD", "STORE", "INPUT", "OUTPUT", "STOP"};
const int opcodes[]      = {1,     2,     3,      4,     5,     6,      7,      8,      9,      10,     11,      12,      13,       14};
const int tamanhos[]     = {2,     2,     2,      2,     2,     2,      2,      2,      3,      2,      2,       2,       2,        1};

int memoria_obj[1000]; // pendências resolvidas p/ o .obj
int memoria_pen[1000]; // ponteiros da lista p/ o .pen


int buscar_opcode(const char *nome) {
    for (int i = 0; i < 14; i++) {
        if (strcmp(instrucoes[i], nome) == 0) return opcodes[i];
    }
    return -1;
}

int buscar_tamanho(const char *nome) {
    for (int i = 0; i < 14; i++) {
        if (strcmp(instrucoes[i], nome) == 0) return tamanhos[i];
    }
    return 0;
}

// Busca o índice de um símbolo na tabela. Retorna -1 se não achar.
int buscar_simbolo(const char *nome) {
    for (int i = 0; i < qtd_simbolos; i++) {
        if (strcmp(tabela_simbolos[i].nome, nome) == 0) return i;
    }
    return -1;
}

// resolve a lista encadeada substituindo os ponteiros pelo endereço real do rotulo
void resolver_pendencias(int cabeca_lista, int endereco_real) {
    int atual = cabeca_lista;
    while (atual != -1) {
        int proximo = memoria_obj[atual]; 
        memoria_obj[atual] = endereco_real; 
        atual = proximo; 
    }
}


int montar(const char *nome_arquivo_entrada) {
    FILE *arquivo_pre = fopen(nome_arquivo_entrada, "r");
    if (!arquivo_pre) return 0;

    int PC = 0;
    char linha[256];

    while (fgets(linha, sizeof(linha), arquivo_pre) != NULL) {
        if (strstr(linha, "SECTION") != NULL) continue; // Ignora ultimas seções

        char tokens[10][50];
        int num_tokens = 0;
        char *token = strtok(linha, " \t\r\n,"); // Quebra por espaços 

        while (token != NULL) {
            strcpy(tokens[num_tokens++], token);
            token = strtok(NULL, " \t\r\n,");
        }
        if (num_tokens == 0) continue;

        int indice_atual = 0;

        // verifica rotulo
        if (tokens[0][strlen(tokens[0]) - 1] == ':') {
            char nome_rotulo[50];
            strcpy(nome_rotulo, tokens[0]);
            nome_rotulo[strlen(nome_rotulo) - 1] = '\0'; 

            int idx_simbolo = buscar_simbolo(nome_rotulo);

            if (idx_simbolo == -1) {
                // Rótulo novo-->DEFINIDO na tabela                      
                strcpy(tabela_simbolos[qtd_simbolos].nome, nome_rotulo);
                tabela_simbolos[qtd_simbolos].endereco = PC;
                tabela_simbolos[qtd_simbolos].definido = 1;
                qtd_simbolos++;
            } else if (tabela_simbolos[idx_simbolo].definido == 0) {
                // rotulo existia como pendencia
                resolver_pendencias(tabela_simbolos[idx_simbolo].endereco, PC);
                // atualiza a tabela
                tabela_simbolos[idx_simbolo].endereco = PC;
                tabela_simbolos[idx_simbolo].definido = 1;
            }
            indice_atual++; 
        }

        if (indice_atual >= num_tokens) continue;

        // verifica instrução ou diretiva
        int opcode = buscar_opcode(tokens[indice_atual]);

        if (opcode != -1) {
            // instrução
            memoria_obj[PC]= opcode;
            memoria_pen[PC] = opcode;
            int tamanho = buscar_tamanho(tokens[indice_atual]);
            PC++;

            // tratamento de Operandos 
            for (int i = 1; i < tamanho; i++) {
                if (indice_atual + i >= num_tokens) break;
                
                char *operando = tokens[indice_atual + i];
                
                char op_limpo[50];
                int k = 0;
                for(int j = 0; operando[j] != '\0'; j++) {
                    if(operando[j] != ',') op_limpo[k++] = operando[j];
                }
                op_limpo[k] = '\0';

                int idx_op = buscar_simbolo(op_limpo);

                if (idx_op == -1) {
                    // cria pendência p/ opp desconecido
                    strcpy(tabela_simbolos[qtd_simbolos].nome, op_limpo);
                    tabela_simbolos[qtd_simbolos].endereco = -1; 
                    tabela_simbolos[qtd_simbolos].definido = 0;
                    
                    memoria_obj[PC] = -1;
                    memoria_pen[PC] = -1;
                    
                    tabela_simbolos[qtd_simbolos].endereco = PC; 
                    qtd_simbolos++;
                } else if (tabela_simbolos[idx_op].definido == 1) {
                    // operando conhecido
                    memoria_obj[PC] = tabela_simbolos[idx_op].endereco;
                    memoria_pen[PC] = tabela_simbolos[idx_op].endereco;
                } else {
                    // pendência conhecida: atualiza a lista
                    memoria_obj[PC] = tabela_simbolos[idx_op].endereco;  
                    memoria_pen[PC] = tabela_simbolos[idx_op].endereco;
                    tabela_simbolos[idx_op].endereco = PC; 
                }
                PC++;
            }
        } 
        else {
            // é derivativa CONST ou SPACE
            if (strcmp(tokens[indice_atual], "CONST") == 0) {
                // CONST aceita negativo e hexa
                int valor = (int)strtol(tokens[indice_atual + 1], NULL, 0); 
                memoria_obj[PC] = valor;
                memoria_pen[PC] = valor;
                PC++;
            } 
            else if (strcmp(tokens[indice_atual], "SPACE") == 0) {
                int qtd_espacos = 1;
                if (indice_atual + 1 < num_tokens) {
                    qtd_espacos = atoi(tokens[indice_atual + 1]);
                }
                for (int i = 0; i < qtd_espacos; i++) {
                    memoria_obj[PC] = 0; 
                    memoria_pen[PC] = 0;
                    PC++;
                }
            }
        }
    }

    fclose(arquivo_pre);

    // GERAÇÃO DOS ARQUIVOS DE SAÍDA .obj E .pen
    char nome_obj[256], nome_pen[256];
    strcpy(nome_obj, nome_arquivo_entrada);
    strcpy(nome_pen, nome_arquivo_entrada);
    
    // Troca a extensão .pre por .obj e .pen
    char *p_ext = strrchr(nome_obj, '.');
    if (p_ext) strcpy(p_ext, ".obj");
    
    p_ext = strrchr(nome_pen, '.');
    if (p_ext) strcpy(p_ext, ".pen");

    FILE *arquivo_obj = fopen(nome_obj, "w");
    FILE *arquivo_pen = fopen(nome_pen, "w");

    // Grava os códigos separados por espaço em uma única linha
    for (int i = 0; i < PC; i++) {
        fprintf(arquivo_obj, "%d%c", memoria_obj[i], (i == PC - 1) ? '\0' : ' ');
        fprintf(arquivo_pen, "%d%c", memoria_pen[i], (i == PC - 1) ? '\0' : ' '); 
    }

    fclose(arquivo_obj);
    fclose(arquivo_pen);
    
    printf("Montagem concluida! Arquivos gerados: %s e %s\n", nome_obj, nome_pen);
    return PC; 
}