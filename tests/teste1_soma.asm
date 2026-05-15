; Teste 1: soma dois numeros e imprime o resultado
; Entrada: dois numeros inteiros
; Saida esperada: a soma dos dois
 
SECTION TEXT
    INPUT A
    INPUT B
    LOAD A
    ADD B
    STORE RES
    OUTPUT RES
    STOP
 
SECTION DATA
A:      SPACE
B:      SPACE
RES:    SPACE
 