; Teste 3: diretivas EQU e IF no pre-processamento
; FLAG = 1 entao a linha seguinte ao IF e incluida
; FLAG = 0 entao e ignorada
;
; Com FLAG EQU 1: saida esperada = 10
; Com FLAG EQU 0: saida esperada = 99 (linha do ADD e ignorada)
 
FLAG EQU 1
 
SECTION TEXT
    LOAD BASE
IF FLAG
    ADD EXTRA
    STORE RES
    OUTPUT RES
    STOP
 
SECTION DATA
BASE:  CONST 10
EXTRA: CONST 5
RES:   SPACE
 