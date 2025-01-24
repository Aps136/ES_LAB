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
    LDR R1, =NUM2       ; Load address of second number
    LDR R0, [R0]        ; Dereference NUM1 into R0
    LDR R1, [R1]        ; Dereference NUM2 into R1

    MOV R2, #0          ; Initialize Sum to 0
    MOV R3, #0          ; Initialize Carry to 0

MultiplyLoop
    CMP R1, #0          ; Check if R1 (multiplier) is zero
    BEQ EndLoop         ; Exit loop if R1 == 0

    ADDS R2, R2, R0     ; Add R0 to R2 (Sum), set flags
    ADC R3, R3, #0      ; Add carry to R3 if overflow occurred
    SUBS R1, R1, #1     ; Decrement R1 (multiplier), set flags
    BNE MultiplyLoop    ; Repeat if R1 != 0

EndLoop
    ; Store the result
    LDR R4, =RESULT     ; Load address of RESULT
    STR R2, [R4]        ; Store lower 32 bits of the result in RESULT
    STR R3, [R4, #4] 	
STOP
    B STOP   
    AREA mydata, DATA, READWRITE
    
NUM1 DCD 0X0000000F
NUM2 DCD 0X00000005	
RESULT DCD 0X0, 0X0    
    END
