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
	MOV R1, #0
	MOV R9, #10
LOOP 
	CMP R0, #0
	BEQ CHECK
	
	UDIV R3, R0, R9
	MLS R2, R3, R9, R0
	MUL R4, R2, R2
	MUL R4, R4, R2
	ADD R1, R4
	MOV R0, R3
	B LOOP
CHECK

	LDR R0, =NUM1
	LDR R0, [R0]
	CMP R0, R1
	LDR R7, =RESULT
	MOVEQ R8, #1
	MOVNE R8, #0
	STR R8, [R7]
	
STOP  B STOP 
NUM1 DCD 153
    AREA mydata, DATA, READWRITE
	ALIGN
RESULT DCD 0	
    END
