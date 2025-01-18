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
	LDR R0, =0XF412963B
	LDR R1, =0XF62562FA
	MOV R2, #035
	MOV R3, #0X21
	SUBS R4, R0, R1
	SBC R5, R2, R3
STOP B STOP
	AREA mydata, DATA, READWRITE
DST  DCD 0
	END
