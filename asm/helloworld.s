section .data
    msg db "\nHello World!\n\0"

section .bss
    buffer 100

_start:
    mov ra, 0xFD
    mov rb, buffer
    mov rc, 16
    bra _kern_imm_to_base_str
    mov ra, buffer
    bra _kern_puts
    mov ra, msg         ; Moving msg into ra
    bra _kern_puts      ; Calling _kern_puts to write msg
    hlt                 ; Halting

;
; Serial In/Output
;
_kern_puts:
    mov rb, ra          ; Moving address in ra to rb, because ra is used at call
_kern_puts_loop:
    mld8 ra, rb         ; Loading value contained in ra into rb
    cmp ra, '\0'        ; Checking if rb is a nulltermination
    jeq _kern_puts_end  ; If its nulltermination, jump back to loop start
    bra _kern_putc      ; Printing current character
    inc rb              ; Incrementing address offset
    jmp _kern_puts_loop ; Jump back to beginning
_kern_puts_end:
    ret

_kern_gets:
    mov rb, ra          ; Moving address in ra to rb, because ra is used at call
_kern_gets_loop:
    bra _kern_getc      ; Getting character
    mov ra, rr          ; Getc returned character to rr, so we move it from rr back to ra
    bra _kern_putc      ; Echoing ra with the character of getc out
    mst8 ra, rb         ; Storing Character of ra into current memory rb
    inc rb              ; Incrementing rb
    cmp ra, '\n'        ; Comparing rb against newline
    jnq _kern_gets_loop ; Looping if its not equal
_kern_gets_end:
    mst '\0', rb        ; Nullterminating string
    ret

_kern_putc:
    iow 0x0, ra         ; Writing character in ra to ioport 0x0
    ret

_kern_getc:
    ior 0x0, rr         ; Reading character from serial to rr
    ret

;
; Number to base string converter
;
; ra = number
; rb = buffer address
; rc = base
;
_kern_imm_to_base_str:
    cmp ra, 0
    jnq _kern_imm_to_base_str_go
    mst8 '0', rb
    inc rb
    mst8 '\0', rb
    ret
_kern_imm_to_base_str_go:
    mov rf, ra
    mov ea, rb
_kern_imm_to_base_str_loop:
    cmp rf, 0
    jeq _kern_imm_to_base_str_done
    mov rd, rf
    div rd, rc
    mov re, rd
    mov ra, re
    mul re, rc
    mov rd, rf
    sub rd, re
    cmp rd, 10
    jil _kern_imm_to_base_str_digit
    sub rd, 10
    mov re, 'A'
    add re, rd
    jmp _kern_imm_to_base_str_store
_kern_imm_to_base_str_digit:
    mov re, '0'
    add re, rd
_kern_imm_to_base_str_store:
    mst8 re, rb
    inc rb
    mov rf, ra
    jmp _kern_imm_to_base_str_loop
_kern_imm_to_base_str_done:
    mst8 '\0', rb
    dec rb
    mov ra, ea
_kern_imm_to_base_str_rev_loop:
    cmp ra, rb
    jig _kern_imm_to_base_str_end
    mld8 rd, ra
    mld8 re, rb
    mst8 re, ra
    mst8 rd, rb
    inc ra
    dec rb
    jmp _kern_imm_to_base_str_rev_loop
_kern_imm_to_base_str_end:
    ret
