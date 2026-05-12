#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simulador.h"

int simular(const char *nome_arquivo_entrada) {
    FILE *arquivo_obj = fopen(nome_arquivo_entrada, "r");
    if (!arquivo_obj) {
        printf("Erro: Nao foi possivel abrir o arquivo %s\n", nome_arquivo_entrada);
        return 0;
    }

    // "Memória RAM" do simulador: armazena programa e dados
    int memoria[1000] = {0}; 
    int tamanho_memoria = 0;
    int valor;

    // lendo .obj e colocando na memória
    while (fscanf(arquivo_obj, "%d", &valor) == 1) {
        memoria[tamanho_memoria] = valor;
        tamanho_memoria++;
    }
    fclose(arquivo_obj);

    printf("Iniciando execucao do simulador...\n");
    printf("----------------------------------\n");

    // registradores 
    int PC = 0;  // Program Counter
    int ACC = 0; // Acumulador

    // execução por instrução
    int rodando = 1;
    while (rodando && PC < tamanho_memoria) {
        int opcode = memoria[PC]; // Busca a instrução

        switch (opcode) {
            case 1: // ADD
                ACC = ACC + memoria[memoria[PC + 1]];
                PC += 2;
                break;
            case 2: // SUB
                ACC = ACC - memoria[memoria[PC + 1]];
                PC += 2;
                break;
            case 3: // MULT
                ACC = ACC * memoria[memoria[PC + 1]];
                PC += 2;
                break;
            case 4: // DIV
                ACC = ACC / memoria[memoria[PC + 1]];
                PC += 2;
                break;
            case 5: // JMP
                PC = memoria[PC + 1];
                break;
            case 6: // JMPN
                if (ACC < 0) PC = memoria[PC + 1];
                else PC += 2;
                break;
            case 7: // JMPP
                if (ACC > 0) PC = memoria[PC + 1];
                else PC += 2;
                break;
            case 8: // JMPZ
                if (ACC == 0) PC = memoria[PC + 1];
                else PC += 2;
                break;
            case 9: // COPY 
                memoria[memoria[PC + 2]] = memoria[memoria[PC + 1]];
                PC += 3;
                break;
            case 10: // LOAD
                ACC = memoria[memoria[PC + 1]];
                PC += 2;
                break;
            case 11: // STORE
                memoria[memoria[PC + 1]] = ACC;
                PC += 2;
                break;
            case 12: // INPUT
                printf("INPUT: ");
                scanf("%d", &memoria[memoria[PC + 1]]);
                PC += 2;
                break;
            case 13: // OUTPUT
                printf("OUTPUT: %d\n", memoria[memoria[PC + 1]]);
                PC += 2;
                break;
            case 14: // STOP
                rodando = 0;
                PC += 1;
                break;
            default:
                printf("ERRO DE EXECUCAO: Opcode desconhecido (%d) na posicao %d\n", opcode, PC);
                rodando = 0;
                break;
        }
    }

    printf("----------------------------------\n");
    printf("Execucao finalizada.\n");
    return 1;
}