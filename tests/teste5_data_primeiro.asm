; Teste 5: DATA declarada ANTES de TEXT
; O pre-processador deve reordenar: TEXT primeiro, DATA por ultimo
; Saida esperada: -7
 
SECTION DATA
X: CONST -7
 
SECTION TEXT
    OUTPUT X
    STOP