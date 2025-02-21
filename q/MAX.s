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
	  LDR R0, =ARRAY
	  LDR R3, [R0]
	  MOV R2, #4
LOOP 
	ADD R0, R0, #4	;MOVE TO NEXT ELEMENT
	LDR R4, [R0]    ;LOAD NEXT ELE
	CMP R3, R4
	MOVLO R3, R4; if r4>r3 update max
	SUBS R2, #1
	BNE LOOP
	LDR R5, =RESULT
	STR R3, [R5]		
STOP B STOP	
ARRAY DCD 10,5,33,24,56
    AREA mydata, DATA, READWRITE
RESULT DCD 0
		END
	
