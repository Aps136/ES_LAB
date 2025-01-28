	AREA RESET, DATA, READONLY
	EXPORT __Vectors
__Vectors
	DCD 0X10001000
	DCD Reset_Handler 
	ALIGN 
	AREA mycode,CODE ,READONLY                                          
	ENTRY
	EXPORT Reset_Handler
Reset_Handler	
	LDR R0, =NUM1
	LDR R1, =NUM2
	LDR R2, [R0]
	LDR R3, [R1]
	MOV R4, #0;remainder
	MOV R5, #1;i
LCM_LOOP
	MUL R6, R5, R2
	UDIV R7, R6, R3
	MLS R4, R7, R3, R6   
    CMP R4, #0            
    BEQ FOUND            
    ADD R5, #1        
    B LCM_LOOP
FOUND
	LDR R9, =LCM
    STR R6, [R9]  	
STOP B STOP	
NUM1 DCD 0X00000008
NUM2 DCD 0X00000002
	AREA mydata, DATA, READWRITE
LCM  DCD 0X0
	END

lcm.s
Displaying lcm.s.
