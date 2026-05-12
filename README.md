# Montador — Software Básico
Implementação de um montador, ligador e simulador para o assembly hipotético apresentado na disciplina de Software Básico.

### Nome: João Pedro Fernandes Santos

### Sistema operacional: Linux (Ubuntu 22.04)

### Compilador: GCC

## Visão geral
O montador processa arquivos em três estágios independentes, acionados pela extensão do arquivo de entrada:

Entrada	Saída	O que faz
.asm	.pre	Pré-processamento
.pre	.obj + .pen	Montagem
.obj	(stdout)	Simulação

## Como compilar
Na raiz do projeto, execute:

```
make clean 
make
```

O executável gerado se chama montador.

## Fluxo de execução
### 1. Pré-processamento — .asm → .pre
Resolve diretivas EQU e IF, remove comentários, normaliza maiúsculas/minúsculas e reordena as seções (TEXT sempre antes de DATA).

```
./montador tests/nome_do_arquivo.asm
```

### 2. Montagem — .pre → .obj e .pen
Gera o código de máquina ( .obj) e o arquivo de passagem única com pendências não resolvidas ( .pen).

```
./montador tests/nome_do_arquivo.pre
```

### 3. Simulação — .obj → execução
Carrega o código na memória e executa as instruções. Saídas do comando OUTPUT são impressas no terminal; entradas do comando INPUT são lidas do teclado.

```
./montador tests/nome_do_arquivo.obj
```

## Limpeza
Remove o executável e todos os arquivos temporários gerados durante os testes:

```
make clean
```

## Instruções suportadas
Mnemônico	Operandos	Código	Tamanho	Descrição
ADD	1	1	2	ACC ← ACC + MEM[OP]
SUB	1	2	2	ACC ← ACC - MEM[OP]
MULT	1	3	2	ACC ← ACC * MEM[OP]
DIV	1	4	2	ACC ← ACC / MEM[OP]
JMP	1	5	2	PC ← OP
JMPN	1	6	2	Se ACC < 0, PC ← OP
JMPP	1	7	2	Se ACC > 0, PC ← OP
JMPZ	1	8	2	Se ACC == 0, PC ← OP
COPY	2	9	3	MEM[OP1] ← MEM[OP2]
LOAD	1	10	2	ACC ← MEM[OP]
STORE	1	11	2	MEM[OP] ← ACC
INPUT	1	12	2	MEM[OP] ← STDIN
OUTPUT	1	13	2	STDOUT ← MEM[OP]
STOP	0	14	1	Encerra a execução

## Diretivas suportadas
SECTION TEXT/DATA: Marca início de seção.

SPACE [n]: Reserva 1 ou n posições de memória (inicializadas com 0).

CONST valor: Reserva memória com constante (decimal, negativo ou hex 0x).

EQU: Cria sinônimo textual para um símbolo (processado no pré).

IF: Inclui a próxima linha somente se o operando for 1 (processado no pré).

BEGIN / END: Marcam início e fim de módulo.

EXTERN: Indica rótulo definido em módulo externo.

PUBLIC: Indica rótulo público para outros módulos.

## Estrutura do projeto
```
montador/
├── src/
│   ├── main.c
│   ├── preprocessador.c / .h
│   ├── assembler.c / .h
│   └── simulador.c / .h
├── tests/
│   └── *.asm
├── Makefile
└── README.md
```