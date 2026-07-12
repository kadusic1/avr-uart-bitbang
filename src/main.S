.include "m328Pdef.inc"
.include "pins.inc"

; TX/RX data register
.def temp   = r16
; Bit counter register
.def bitcnt = r18
; r24:r25 will be used as a 16 bit register for delay_cycles

.cseg
.org 0x0000
    rjmp main

main:
    ; TX output
    sbi   TX_DDR, TX_BIT
    ; TX idle = HIGH
    sbi   TX_PORT, TX_BIT
    ; RX input
    cbi   RX_DDR, RX_BIT

echo_loop:
    rcall uart_getc
    rcall uart_putc
    rjmp  echo_loop

.include "delay.inc"
.include "uart.inc"
