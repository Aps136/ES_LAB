  AREA RESET, DATA, READONLY
  EXPORT __Vectors
__Vectors
  ALIGN
  DCD 0x10001000        ; Initial stack pointer value
  DCD Reset_Handler      ; Reset handler address

  AREA mycode, CODE, READONLY
  ENTRY
  EXPORT Reset_Handler

Reset_Handler
  MOV R3, #10            ; Load N (10) into R3
  LDR R0, =SRC           ; Load address of SRC into R0
  LDR R1, =DST           ; Load address of DST into R1

LOOP
  LDR R2, [R0], #4       ; Load word from SRC into R2 and increment R0
  STR R2, [R1], #4       ; Store word from R2 to DST and increment R1
  SUBS R3, R3, #1        ; Decrement R3
  BNE LOOP               ; Repeat if R3 != 0

  MOV R3, #10            ; Reload N (10)
  LDR R0, =DST           ; Load address of DST into R0
  LDR R1, =0x10000040    ; Load destination address into R1

LOOP1
  LDR R2, [R0], #4       ; Load word from DST into R2 and increment R0
  STR R2, [R1], #4       ; Store word from R2 to memory and increment R1
  SUBS R3, R3, #1        ; Decrement R3
  BNE LOOP1              ; Repeat if R3 != 0

STOP
  B STOP                 ; Infinite loop

  AREA mydata, DATA, READWRITE
  ALIGN
SRC DCD 0x23, 0x45, 0xA, 0xB, 0xC, 0x87, 0x88
DST DCD 0, 0, 0, 0, 0, 0, 0, 0, 0, 0

  END
