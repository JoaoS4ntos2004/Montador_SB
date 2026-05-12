#ifndef ASSEMBLER_H
#define ASSEMBLER_H

typedef struct {
    char nome[50];
    int endereco;
    int definido; // 1 = Endereço Real / 0 = primeiro da Lista de Pendências
} Simbolo;

int montar(const char *nome_arquivo_entrada);

#endif // 