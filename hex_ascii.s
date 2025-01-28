    AREA RESET, DATA, READONLY
    EXPORT __Vectors
__Vectors
    DCD 0x10001000
    DCD Reset_Handler
    ALIGN
    AREA mycode, CODE, READONLY
    ENTRY
    EXPORT Reset_Handler
Reset_Handler
    LDR R1, =0x2DEF
    LDR R4, =DST                
    MOV R5, #4                   ; Set loop counter (4 nibbles in a 16-bit number)
LOOP
    AND R2, R1, #0x0F            ; Extract the lower nibble
    CMP R2, #9                   ; Compare lower nibble with 9
    ADDLE R2, R2, #0x30          ; If less than 10, add ASCII offset for numbers
    ADDGE R2, R2, #0x37          ; If greater than or equal to 10, add ASCII offset for letters
    STRB R2, [R4], #1            ; Store the ASCII character and increment the destination address
    LSR R1, #4               ; Shift the hex number to process the next nibble
    SUBS R5, #1              ; Decrement the loop counter
    BNE LOOP           ; If not done, continue processing

STOP
    B STOP

    AREA mydata, DATA, READWRITE
DST DCB 0x00, 0x00, 0x00, 0x00    ; Reserve 4 bytes for ASCII characters
    END
