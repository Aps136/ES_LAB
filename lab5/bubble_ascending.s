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
	LDR R0, =ARRAY
	LDR R1, =RESULT
	MOV R2, #10
	MOV R3, #0
UP  LDR R4, [R0,R3]
	STR R4, [R1, R3]
	ADD R3, #4
	SUBS R2, #1
	CMP R2, #0
	BHI UP
	LDR R0, =RESULT
	MOV R5, #9
	MOV R6, #9
OUTERLOOP
	MOV R7, R0
	MOV R3, R5
INNERLOOP
	LDR R8, [R7], #4
	LDR R9, [R7]
	CMP R8, R9
	STRHI R8, [R7]
	STRHI R9, [R7, #-4]
	SUBS R3, #1
	BNE INNERLOOP
	SUBS R5, #1
	SUBS R6, #1
	BNE OUTERLOOP 
ARRAY DCD 0X10, 0X05, 0X33, 0X24, 0X56, 0X77, 0X21, 0X04, 0X87, 0X01
STOP  B STOP                  
    AREA mydata, DATA, READWRITE
RESULT DCD 0,0,0,0,0,0,0,0,0,0		
		END
