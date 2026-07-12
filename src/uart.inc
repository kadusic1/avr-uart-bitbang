; padding macros for aligning cycles
.macro PAD8
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
.endmacro

.macro PAD3
    nop
    nop
    nop
.endmacro

; uart_putc subroutine for transmitting characters via bit-banged uart protocol
uart_putc:
    ; load 8 as the number of bits we transfer
    ; this is done before the start bit to not mess up the timing
    ; because ldi takes 1 cycle
    ldi   bitcnt, 8

    ; start bit starts with low
    ; cbi takes 2 cycles
    cbi   TX_PORT, TX_BIT
    ; padding with delay to make it take exactly 1666 cycles
    PAD8
    WAIT_BIT

    ; loop for data bits
    txbit_loop:
        ; skip if bit 0 in register temp is clear
        ; if bit 0 in register temp is 1 it lasts 1 cycle and goes to txbit_high
        ; if bit 0 in register temp is 0 it lasts 2 cycles and goes to txbit_low
        sbrc  temp, 0
        rjmp  txbit_high
    txbit_low:
        ; Clear the TX bit in TX port to transmit a 0
        ; Takes 2 cycles
        cbi   TX_PORT, TX_BIT
        ; Jump to done transmit takes 2 cycles
        rjmp  txbit_done
    txbit_high:
        ; Set the TX bit to high in TX port to transmit 1
        ; Takes 2 cycles
        sbi   TX_PORT, TX_BIT
        ; nop to take up 1 cycle so that txbit_low and txbit_high take same
        ; amaount of time
        nop
    ; 6 cycles until now
    txbit_done:
        ; wait bit consumes 1656 cycles + 6 = 1662
        ; 4 more cycles to go
        WAIT_BIT
        ; shifts right next bit to position 0 to transmit
        ; takes up 1 cycle
        lsr   temp
        ; decrement the bit counter by 1
        ; 1 cycle
        dec   bitcnt
        ; If after decrement bitcnt is not 0 we continue with the transmit bit
        ; loop and take up 2 cycles so in total we have 1666 cycles for a bit
        ; If it is 0 we continue to stop bit and take 1 cycle so in total
        ; 1665 cycles which is a small error of 1 cycle uart can easily handle
        brne  txbit_loop

    ; stop bit with padding logic to make it take exactly 1666 cycles
    sbi   TX_PORT, TX_BIT
    PAD8
    WAIT_BIT

    ret

; uart_getc subroutine for receiving characters via bit-banged uart protocol
uart_getc:
    wait_start:
        ; if rx_bit in rx_pinreg is cleared skip to wait start align
        ; otherwise loop
        sbic  RX_PINREG, RX_BIT
        rjmp  wait_start

        ; align to read from the middle of first bit 2500 cycles
        WAIT_START_ALIGN

        ; load bit count to bitcnt register and clean up temp register
        ; takes 2 cycles in total which is a minor acceptable delay and not
        ; a problem for the uart protocol
        ldi   bitcnt, 8
        clr   temp

    rxbit_loop:
        ; clear carry flag
        ; takes 1 cycle
        clc
        ; if rx_bit in rx_pinreg is cleared skip setting the carry flag
        ; takes 1 cycle if bit is 0 (skips) or 2 cycles if bit is 1
        sbic  RX_PINREG, RX_BIT
        ; set carry flag to 1 because the rx pin was high
        ; takes 1 cycle and balances execution time with the skip path
        sec
        ; rotate temp register right through carry to shift the bit into position 7
        ; takes 1 cycle and shifts previously received bits towards LSB
        ror   temp
        ; padding to equalize RX block (4 cycles) with TX block (6 cycles)
        ; so total fixed overhead becomes 10, matching TX exactly
        PAD3
        ; wait for a full bit period to position the next read at the center of the next bit
        ; consumes the majority of the bit timing cycles
        WAIT_BIT
        ; decrement the bit counter by 1
        ; takes 1 cycle
        dec   bitcnt
        ; if after decrement bitcnt is not 0 we continue with the receive bit loop
        ; takes 2 cycles when jumping back to loop for a total of 1666 cycles per bit
        ; takes 1 cycle when bitcnt reaches 0 and leaves the loop
        brne  rxbit_loop

        ; wait for the stop bit period to complete before returning
        ; ensures the line stabilizes high and does not corrupt subsequent reads
        WAIT_BIT

        ret
