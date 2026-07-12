; 4*412+8 = 1656 from the math in the paper
.equ BIT_DELAY_N   = 412
; ; 4*623+8 = 2500 also see the paper
.equ START_ALIGN_N = 623

.macro WAIT_BIT
    ldi   r24, low(BIT_DELAY_N)
    ldi   r25, high(BIT_DELAY_N)
    rcall delay_cycles
.endmacro

.macro WAIT_START_ALIGN
    ldi   r24, low(START_ALIGN_N)
    ldi   r25, high(START_ALIGN_N)
    rcall delay_cycles
.endmacro

delay_cycles:
    sbiw  r24, 1
    brne  delay_cycles
    ret
