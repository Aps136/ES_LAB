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
	LDR R0, =0xF62562FA
	LDR R1, = 0xF412963B
	MOV R2, #0x35
	MOV R3, #0x21
	ADDS R4, R0, R1
	ADC R5, R2, R3
STOP B STOP
	AREA mydata, DATA, READWRITE
	END
