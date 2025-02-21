    AREA RESET, DATA, READONLY
    EXPORT __Vectors
__Vectors
    DCD 0x10001000          ; Initial stack pointer
    DCD Reset_Handler       ; Reset handler
    ALIGN

    AREA mycode, CODE, READONLY
    ENTRY
    EXPORT Reset_Handler

Reset_Handler
    ; Initialize array in registers (R0-R9)
    LDR R0, =0x11111111
    LDR R1, =0x22222222
    LDR R2, =0x33333333
    LDR R3, =0x44444444
    LDR R4, =0x55555555
    LDR R5, =0x66666666
    LDR R6, =0x77777777
    LDR R7, =0x88888888
    LDR R8, =0x99999999
    LDR R9, =0xAAAAAAAA

    ; Store registers onto the stack in descending order (STMDB)
    STMDB SP!, {R0-R9}  ; Push R0-R9 onto the stack

    ; Load them in reverse order (LDMDB)
    LDMDB SP!, {R9, R8, R7, R6, R5, R4, R3, R2, R1, R0}  ; Reverse load

    ; Apply bitwise NOT (MVN) to demonstrate additional computation
    MVN R0, R0
    MVN R1, R1
    MVN R2, R2
    MVN R3, R3
    MVN R4, R4
    MVN R5, R5
    MVN R6, R6
    MVN R7, R7
    MVN R8, R8
    MVN R9, R9

    B .  ; Infinite loop to prevent execution errors

    AREA mydata, DATA, READWRITE
    END
