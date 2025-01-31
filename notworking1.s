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

    MOV R2, #0           ; i = 0 (outer loop)

Outer_Loop
    CMP R2, R1           ; If i >= size, sorting is done
    BGE End_Sort

    MOV R3, R2           ; min_index = i
    MOV R4, R2           ; j = i

Inner_Loop
    ADD R4, R4, #1       ; j++
    CMP R4, R1           ; If j >= size, end inner loop
    BGE Swap

    LDR R5, [R0, R3, LSL #2] ; Load arr[min_index]
    LDR R6, [R0, R4, LSL #2] ; Load arr[j]

    CMP R6, R5           ; Compare arr[j] < arr[min_index]
    BGE Inner_Loop       ; If not smaller, continue loop

    MOV R3, R4           ; min_index = j
    B Inner_Loop

Swap
    CMP R2, R3           ; If i == min_index, no swap needed
    BEQ Continue

    LDR R5, [R0, R2, LSL #2] ; Load arr[i]
    LDR R6, [R0, R3, LSL #2] ; Load arr[min_index]

    STR R6, [R0, R2, LSL #2] ; arr[i] = arr[min_index]
    STR R5, [R0, R3, LSL #2] ; arr[min_index] = arr[i]

Continue
    ADD R2, R2, #1       ; i++
    B Outer_Loop

End_Sort
    B End_Sort           ; Infinite loop to stop execution

    AREA mydata, DATA, READWRITE
SIZE    DCD 5             ; Number of elements in the array
ARRAY   DCD 45, 12, 88, 23, 7  ; Unsorted array

    END
