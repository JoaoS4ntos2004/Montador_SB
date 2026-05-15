; Teste 4: laco que conta de N ate 0 e imprime cada valor
; Sem entrada
; Saida esperada (N=3): 3, 2, 1
; (quando chegar a 0 o JMPZ desvia para FIM e nao imprime o 0)
 
SECTION TEXT
LOOP:   OUTPUT CONT
        LOAD CONT
        SUB UM
        STORE CONT
        JMPZ FIM
        JMP LOOP
FIM:    STOP
 
SECTION DATA
CONT: CONST 3
UM:   CONST 1
 