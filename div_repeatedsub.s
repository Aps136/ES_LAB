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
	LDR R0, [R0]
	LDR R1, =NUM2
	LDR R1, [R1]
	MOV R2, #0	
LOOP
	CMP R0, R1
	BLT DONE
	SUBS R0, R0, R1	
	ADD R2, #1
	BNE LOOP
DONE
	LDR R5, =RESULT1
	LDR R6, =RESULT2
	STR R2, [R5]
	STR R0, [R6]	
	
	
STOP  B STOP 
NUM1 DCD 0X5
NUM2 DCD 0X2
    AREA mydata, DATA, READWRITE
	ALIGN
RESULT1 DCD 0
RESULT2 DCD 0
	
    END
