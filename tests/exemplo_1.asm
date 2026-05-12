TAM: EQU 5
FLAG: EQU 0
verdade: equ 1

section text

; teste de case insensitive e rotulo em linha isolada
Rotulo_Um:

   add DOIS

if verdade
copy VAR1 ,  VAR2 ; testando espacos loucos na virgula

IF FLAG
SUB DOIS ; Isso aqui TEM que sumir no .pre

stop

section data
DOIS: const -15 ; teste de constante negativa
TRES: cOnSt 0x1A ; teste de constante hexa

VAR1: space
VAR2: space TAM ; testando a substituicao do EQU pelo valor 5