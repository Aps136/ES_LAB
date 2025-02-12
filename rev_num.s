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
	LDR R0, =0X12345678      ; Load address of the hex number
    LDR R1, [R0]          ; Load the actual hex value into R1
    MOV R2, #0            ; R2 will hold the reversed number
    MOV R3, #4            ; Counter for 4 bytes (Loop runs 4 times)
    MOV R4, #0            ; Shift amount (starts at 0, increases by 8)
Loop_Reverse
    AND R5, R1, #0xFF     ; Extract the least significant byte
    LSL R5, R4        ; Shift it to the correct position
    ORR R2, R5        ; Merge with the reversed number
    LSR R1, #8        ; Move to the next byte
    ADD R4, #8        ; Increase shift amount by 8 bits
    SUB R3, #1        ; Decrement loop counter
    CMP R3, #0
    BNE Loop_Reverse      ; Repeat until all 4 bytes are processed
    LDR R0, =DST          ; Load address of result storage
    STR R2, [R0]           ; Store reversed number back in memory
	
STOP B STOP	
	AREA mydata, DATA, READWRITE
DST DCD 0
	END
