    AREA RESET, DATA, READONLY
    EXPORT __Vectors
__Vectors
    DCD 0x10001000           ; Initial stack pointer
    DCD Reset_Handler        ; Reset handler
    ALIGN

    AREA mycode, CODE, READONLY
    ENTRY
    EXPORT Reset_Handler

Reset_Handler
    LDR R0, =ARRAY           ; Load base address of array
    LDR R1, =RESULT          ; Load base address of result array
    MOV R2, #10              ; Number of elements
    MOV R3, #0               ; Index counter

COPY_ARRAY
    LDR R4, [R0, R3]         ; Load element from ARRAY
    STR R4, [R1, R3]         ; Store in RESULT
    ADD R3, #4               ; Move to next index
    SUBS R2, #1
    BHI COPY_ARRAY

    LDR R0, =RESULT          ; Start sorting RESULT array
    MOV R5, #10              ; Number of elements

OUTER_LOOP
    SUBS R5, #1              ; Outer loop runs (n-1) times
    BEQ STOP                 ; Exit if sorted
    MOV R6, R5               ; Inner loop counter
    MOV R7, R0               ; Pointer to current min element
    MOV R8, R7               ; Store index of min element

INNER_LOOP
    ADD R7, #4               ; Move to next element
    LDR R9, [R7]             ; Load next element
    LDR R10, [R8]            ; Load current min

    CMP R9, R10              ; Compare new element with current min
    MOVLO R8, R7             ; If R9 < R10, update min index

    SUBS R6, #1
    BNE INNER_LOOP           ; Continue inner loop

    ; Swap values
    LDR R9, [R0, R5, LSL #2] ; Load current position value
    LDR R10, [R8]            ; Load min element
    STR R10, [R0, R5, LSL #2]; Store min at current position
    STR R9, [R8]             ; Store old value at min index

    B OUTER_LOOP             ; Repeat for next position

STOP  
    B STOP       
	; Infinite loop
ARRAY DCD 0X10, 0X05, 0X33, 0X24, 0X56, 0X77, 0X21, 0X04, 0X87, 0X01

    AREA mydata, DATA, READWRITE
RESULT DCD 0,0,0,0,0,0,0,0,0,0       
    END
