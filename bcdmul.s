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
	LDR R0, =NUM1       ; Load address of first number
     LDR R0, =NUM1       ; Load address of NUM1 (BCD number)
    LDR R1, =NUM2       ; Load address of NUM2 (BCD number)
    LDR R0, [R0]        ; Dereference NUM1 into R0 (BCD format)
    LDR R1, [R1]        ; Dereference NUM2 into R1 (BCD format)

    ; Check if either NUM1 or NUM2 is zero
    CMP R0, #0
    BEQ EndLoop          ; If NUM1 is 0, result is 0
    CMP R1, #0
    BEQ EndLoop          ; If NUM2 is 0, result is 0

    MOV R2, #0          ; Initialize Sum (R2) to 0
    MOV R3, #0          ; Initialize Carry (R3) to 0

MultiplyLoop
    CMP R1, #0          ; Check if NUM2 is zero
    BEQ EndLoop          ; Exit loop if NUM2 == 0

    ; Process each digit of NUM2
    AND R4, R0, #0x0F   ; Extract the least significant digit of NUM1
    ADD R2, R2, R4      ; Add the extracted digit of NUM1 to the sum
    ADC R3, R3, #0      ; Add carry to R3 if overflow occurs in the addition

    ; Prepare NUM1 and NUM2 for the next iteration
    LSR R0, R0, #4      ; Logical shift NUM1 right by 4 bits (move to next digit)
    LSR R1, R1, #4      ; Logical shift NUM2 right by 4 bits (move to next digit)

    BNE MultiplyLoop    ; Repeat until NUM2 is zero

EndLoop
    ; Store the result in memory
    LDR R4, =RESULT     ; Load address of RESULT
    STR R2, [R4]        ; Store lower 32 bits of the result
    STR R3, [R4, #4] 	
STOP
    B STOP   
    AREA mydata, DATA, READWRITE
    
NUM1 DCD 0x0027
NUM2 DCD 0x0056	
RESULT DCD 0X0, 0X0    
    END
