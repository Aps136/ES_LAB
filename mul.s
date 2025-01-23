    AREA RESET, DATA, READONLY
    EXPORT __Vectors
__Vectors
    DCD 0x10001000        ; Initial stack pointer value
    DCD Reset_Handler      ; Reset handler address
    ALIGN

    AREA mycode, CODE, READONLY
    ENTRY
    EXPORT Reset_Handler

Reset_Handler
	LDR R0, =VALUE1
	LDRH R1, [R0]
	LDR R0, =VALUE2
	LDRH R3, [R0]
	MUL R6, R1, R3
	LDR R2, =RESULT
	STR R6, [R2]
STOP
    B STOP   
VALUE1 DCD 0X1234;
VALUE2 DCD 0X5678;
	

    AREA mydata, DATA, READWRITE
    ALIGN
 
RESULT DCD 0               ; Space to store the result

    END
