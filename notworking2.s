    AREA RESET, DATA, READONLY
    EXPORT __Vectors
__Vectors
    DCD 0x10001000       ; Initial stack pointer
    DCD Reset_Handler    ; Reset handler
    ALIGN

    AREA mycode, CODE, READONLY
    ENTRY
    EXPORT Reset_Handler

Reset_Handler
    LDR R0, =ARRAY       ; Load base address of array
    LDR R1, =SIZE        ; Load address of array size
    LDR R1, [R1]         ; Load actual size (N)
    LDR R2, =KEY         ; Load address of search key
    LDR R2, [R2]         ; Load the key value to R2

    MOV R3, #0           ; i = 0 (index counter)
    MOV R4, #-1          ; Not found case (default to -1)

Search_Loop
    CMP R3, R1           ; If i >= size, exit loop
    BGE Not_Found

    LDR R5, [R0, R3, LSL #2] ; Load arr[i]
    CMP R5, R2           ; Compare arr[i] with key
    BEQ Found            ; If found, jump to Found

    ADD R3, R3, #1       ; i++
    B Search_Loop        ; Repeat the loop

Not_Found
    STR R4, RESULT       ; Store -1 (not found case)
    B End_Search

Found
    STR R3, RESULT       ; Store found index

End_Search
    B End_Search         ; Infinite loop to stop execution

    AREA mydata, DATA, READWRITE
SIZE    DCD 10                  ; Number of elements in the array
ARRAY   DCD 5, 12, 8, 23, 45, 88, 32, 16, 27, 9  ; Array elements
KEY     DCD 23                  ; Element to search for
RESULT  DCD -1                  ; Store index of found element (-1 if not found)

    END
