    AREA RESET, DATA, READONLY
    EXPORT __Vectors
__Vectors
    DCD 0x40001000        ; Initial stack pointer value
    DCD Reset_Handler      ; Reset handler address
    ALIGN

    AREA mycode, CODE, READONLY
    ENTRY
    EXPORT Reset_Handler

Reset_Handler
    LDR R0, =NUM1
    LDR R0, [R0]        ; Load NUM1 into R0
    MOV R5, #0          ; R5 stores sum of digits

SUM_LOOP
    AND R2, R0, #0x0F   ; Extract last digit
    ADD R5, R5, R2      ; Add to sum
    LSR R0, R0, #4      ; Shift right to remove last digit
    CMP R0, #0          ; If number becomes 0, stop
    BNE SUM_LOOP

    ; Reload original NUM1 value for division
    LDR R0, =NUM1
    LDR R0, [R0]

    UDIV R4, R0, R5     ; Quotient (R4 = NUM1 / Sum of digits)
    MUL  R6, R4, R5     ; Multiply back (R6 = Quotient * Sum of digits)
    SUBS R7, R0, R6     ; Find remainder (R7 = NUM1 - R6)
    
    LDR R8, =RESULT
    STR R7, [R8]        ; Store remainder

STOP  B STOP 

NUM1 DCD 0X21  ; Example number
    AREA mydata, DATA, READWRITE
    ALIGN
RESULT DCD 0    
    END
