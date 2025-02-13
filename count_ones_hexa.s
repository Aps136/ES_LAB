    AREA RESET, DATA, READONLY
    EXPORT __Vectors
__Vectors
    DCD 0x10001000           ; Initial stack pointer
    DCD Reset_Handler        ; Reset handler
    ALIGN
    AREA mycode, CODE, READONLY
    ENTRY
    EXPORT Reset_Handler
Reset_Handler
		LDR R0, =NUM
		LDR R3, [R0]
		LDR R1, =RESULT
		MOV R2, #0		
		MOV R9,#0 ;LL
		MOV R10,#32 ;UL
		
LOOP 
	CMP R9,R10
	BEQ STOP
	ADD R9,#1	
	AND R4, R3, #0X1
	LSR R3, #1
	CMP R4, #1
	BEQ FNAME
	B LOOP
FNAME 
	ADD R2, #1
	B LOOP
STOP  B STOP       
NUM DCD 0X123
    AREA mydata, DATA, READWRITE
RESULT DCD 0
    END
