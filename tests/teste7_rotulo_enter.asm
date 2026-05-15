; Teste 7: rotulo sozinho na linha (seguido de Enter)
; o rotulo deve ser associado a instrucao da linha seguinte
; Saida esperada: 77
 
SECTION TEXT
    JMP INICIO
 
LIXO:
    OUTPUT ERRADO  ; nunca deve executar
    STOP
 
INICIO:
    OUTPUT CERTO
    STOP
 
SECTION DATA
ERRADO: CONST 0
CERTO:  CONST 77