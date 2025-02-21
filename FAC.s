    AREA RESET, DATA, READONLY
    EXPORT __Vectors
__Vectors
    DCD 0x10001000           ; Initial stack pointer
    DCD Reset_Handler         ; Reset handler
    ALIGN

    AREA mycode, CODE, READONLY
    ENTRY
    EXPORT Reset_Handler

Reset_Handler
	LDR R0, =NUM
	LDR R1, [R0]
	MOV R2, #1
	CMP R1, #0
	BEQ STORE_RESULT
FAC_LOOP
	MUL R2,R2,  R1
	SUBS R1, R1, #1
	BNE FAC_LOOP
STORE_RESULT
	LDR R3, =RESULT
	STR R2, [R3]	
STOP  B STOP   
NUM DCD 5        
    AREA mydata, DATA, READWRITE
RESULT DCD 0	

		END
