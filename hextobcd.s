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
            LDR R1, =0XF7EAB
			MOV R2, #0X0A
			MOV R5, #0X0
			MOV R6, #0
LOOP UDIV R0, R1, R2
	 MUL R3, R0, R2
	 SUB R4, R1, R3
	 LSL R4, R6
	 ADD R5, R4
	 ADD R6, #4
	 MOV R1, R0
	 CMP R0, R2
	 BHS LOOP
	 LSL R0, R6
	 ADD R5, R0
	 LDR R7, =BCD
	 STR R5, [R7]
	 
STOP  B STOP                   

    AREA mydata, DATA, READWRITE
BCD DCD 0               
    END
