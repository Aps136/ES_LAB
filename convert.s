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
		LDR R1, =0X378
		AND R2, R1, #0X0F; extract digits
		MOV R3, #0
		MOV R4, #0
		MOV R6, #2
		MOV R8, #0XA
LOOP 
		ADD R2, R3
		LSR R1, #4; shift to next digit
		AND R3, R1, #0X0F; extract digit
		ADD R4, #1; counts shifts
		MOV R5, #0; inner loop counter
LOOP1 
		MUL R3, R8  ;multiply by 10
		ADD R5, #1; increment inner loop counter
		TEQ R4, R5;
		BNE LOOP1
		SUBS R6, #1
		BNE LOOP
		ADD R2, R3
		LDR R7, =DST
		STR R2, [R7]
		
STOP
    B STOP   
    AREA mydata, DATA, READWRITE
    ALIGN
DST DCD 0X0         
    END
