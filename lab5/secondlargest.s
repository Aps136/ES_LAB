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
	LDR R0, =ARRAY          ; Load base address of ARRAY
    LDR R1, =RESULT         ; Load base address of RESULT
    MOV R2, #10             ; Array size counter
    MOV R3, #0              ; Offset counter

COPY_ARRAY
    LDR R4, [R0, R3]        ; Load element from ARRAY
    STR R4, [R1, R3]        ; Store in RESULT
    ADD R3, R3, #4          ; Move to next element
    SUBS R2, R2, #1         ; Decrement count
    BHI COPY_ARRAY          ; Repeat until done
    
	LDR R0, =RESULT         ; Load RESULT base address
    MOV R5, #9              ; Outer loop counter (n-1)

OUTERLOOP
    MOV R7, R0              ; Reset pointer to start
    MOV R3, R5              ; Inner loop counter

INNERLOOP
    LDR R8, [R7]            ; Load A[i]
    LDR R9, [R7, #4]        ; Load A[i+1]
    CMP R8, R9              ; Compare A[i] and A[i+1]
    BHS NO_SWAP             ; If A[i] >= A[i+1], no swap

    ; Swap A[i] and A[i+1] (Descending Order)
    STR R9, [R7]            ; Store A[i+1] at A[i]
    STR R8, [R7, #4]        ; Store A[i] at A[i+1]

NO_SWAP
    ADD R7, R7, #4          ; Move to next element
    SUBS R3, R3, #1         ; Decrement inner loop counter
    BNE INNERLOOP           ; Repeat inner loop

    SUBS R5, R5, #1         ; Decrement outer loop counter
    BNE OUTERLOOP           ; Repeat outer loop

    ; ----------------------
    ; Store 2nd Largest Element
    ; ----------------------
    LDR R10, =RESULT
    LDR R11, [R10, #4]      ; Second element (RESULT[1])

    LDR R12, =S             ; Store in S
    STR R11, [R12]          ; Save result
	
STOP  B STOP   
ARRAY DCD 0X10, 0X05, 0X33, 0X24, 0X56, 0X77, 0X21, 0X04, 0X87, 0X01               
    AREA mydata, DATA, READWRITE
RESULT DCD 0,0,0,0,0,0,0,0,0,0		
S DCD 0
		END
