# Montador — Software Básico (UnB)

**Aluno:** João Pedro Fernandes Santos - 222025342
**SO:** Linux (Ubuntu 22.04)  
**Compilador:** GCC

Montador, simulador e ligador do assembly inventado da disciplina Software Básico.

---

## Compilar

```bash
make clean
make
```

## Rodar

O programa detecta o modo pela extensão do arquivo:

```bash
# 1. Pré-processar: resolve EQU/IF, remove comentários, reordena seções(DATA/TEXT)
./montador arquivo.asm

# 2. Montar: gera código de máquina
./montador arquivo.pre

# 3. Simular: executa o .obj 
./montador arquivo.obj
```

OBS: Adicione o arquivo que quiser traduzir na pasta `tests/`, e troque o termo "arquivo" nos comandos 
acima pelo nome do arquivo que você adicionou na pasta. Já existem teste padrões na pasta feitos para testar o montador, são eles:

teste1_soma:                INPUT, LOAD, ADD, STORE, OUTPUT — fluxo básico completo
teste2_copy:                COPY e CONST decimal — o bug que você corrigiu
teste3_equ_if:              EQU e IF — muda o FLAG de 1 para 0 e confere se a linha some
teste4_laco:                MPZ, JMP, SUB — laço com desvio condicional
teste5_data_primeiro:       DATA declarada antes de TEXT — reordenação no pré-processador
teste6_const_hexa:          CONST com 0x1E e valor negativo -5
teste7_rotulo_enter:        Rótulo sozinho na linha seguido de quebra de linha

## Limpar

```bash
make clean
```

---

## Estrutura

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

## Instruções

| Mnemônico | Código | Tamanho | Operação |
|-----------|--------|---------|----------|
| ADD | 1 | 2 | ACC ← ACC + MEM[OP] |
| SUB | 2 | 2 | ACC ← ACC - MEM[OP] |
| MULT | 3 | 2 | ACC ← ACC * MEM[OP] |
| DIV | 4 | 2 | ACC ← ACC / MEM[OP] |
| JMP | 5 | 2 | PC ← OP |
| JMPN | 6 | 2 | se ACC < 0: PC ← OP |
| JMPP | 7 | 2 | se ACC > 0: PC ← OP |
| JMPZ | 8 | 2 | se ACC = 0: PC ← OP |
| COPY | 9 | 3 | MEM[OP1] ← MEM[OP2] |
| LOAD | 10 | 2 | ACC ← MEM[OP] |
| STORE | 11 | 2 | MEM[OP] ← ACC |
| INPUT | 12 | 2 | MEM[OP] ← STDIN |
| OUTPUT | 13 | 2 | STDOUT ← MEM[OP] |
| STOP | 14 | 1 | encerra execução |

## Diretivas

| Diretiva | Descrição |
|----------|-----------|
| SECTION TEXT/DATA | marca início/retomada de seção |
| SPACE [n] | reserva n posições (padrão 1), inicializadas com 0 |
| CONST val | reserva posição com constante (decimal, negativo ou 0x hex) |
| EQU | alias textual, processado no pré |
| IF | inclui próxima linha só se operando = 1, processado no pré |
| BEGIN / END | início e fim de módulo |
| EXTERN / PUBLIC | rótulos externos e públicos |