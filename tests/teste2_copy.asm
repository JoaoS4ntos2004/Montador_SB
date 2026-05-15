; Teste 2: copia um valor constante para outra posicao e imprime
; Sem entrada do teclado
; Saida esperada: 30
 
SECTION TEXT
    COPY DESTINO,ORIGEM
    OUTPUT DESTINO
    STOP
 
SECTION DATA
ORIGEM:  CONST 30
DESTINO: SPACE
 