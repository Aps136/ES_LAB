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
		LDR R1, =TARGET
		MOV R2, #1
		MOV R3, #10
LOOP  LDR R4, [R0], #4
	  LDR R7, [R1]
	  CMP R4, R7
	  BEQ EXIT
	  SUBS R3, #1
	  ADD R2, #1
	  BNE LOOP
EXIT 
	LDR R5, =RESULT
	STR R2, [R5], #4
	STR R4, [R5]  
STOP  B STOP       
ARRAY DCD 0X10, 0X05, 0X33, 0X24, 0X56, 0X77, 0X21, 0X04, 0X87, 0X01
TARGET DCD 0X24
    AREA mydata, DATA, READWRITE
RESULT DCD 0,0  
    END
