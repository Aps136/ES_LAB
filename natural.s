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
	 MOV R0, #10          ; Load the value of n (e.g., sum of first 10 numbers)
    MOV R1, #1           ; Initialize the accumulator (starting number 1)
    MOV R2, #0           ; Initialize sum to 0 (R2 will hold the final result)

SumLoop
    CMP R1, R0           ; Compare current number (R1) with n (R0)
    BEQ EndLoop           ; Exit the loop when R1 equals n

    MLA R2, R1, R1, R2   ; Multiply R1 by R1 and add the result to R2 (sum += R1 * R1)
    ADD R1, R1, #1       ; Increment R1 to process the next number

    B SumLoop            ; Repeat the loop

EndLoop
    ; Store the result
    LDR R3, =RESULT      ; Load the address of the RESULT variable
    STR R2, [R3]
STOP
    B STOP   
    AREA mydata, DATA, READWRITE
    
RESULT DCD 0X0 
    END
