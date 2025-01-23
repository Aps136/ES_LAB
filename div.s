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
	MOV R2, #00
	LDR R0, =VALUE1
	LDR R1, [R0]
	LDR R0, =VALUE2
	LDR R3, [R0]
up SUB R1, R3
	ADD R2, #01
	CMP R1, R3
	BCS up
	LDR R6, =RESULT
	STR R2, [R6, #4]
	STR R1, [R6]	
STOP
    B STOP   
VALUE1 DCD 0x10;
VALUE2 DCD 0x5;	
    AREA mydata, DATA, READWRITE
    ALIGN
RESULT DCD 0,0           
    END
