; Teste 6: CONST com valor hexadecimal e negativo
; 0x1E = 30 em decimal
; Saida esperada: 30 e depois -5
 
SECTION TEXT
    OUTPUT HEX
    OUTPUT NEG
    STOP
 
SECTION DATA
HEX: CONST 0x1E
NEG: CONST -5