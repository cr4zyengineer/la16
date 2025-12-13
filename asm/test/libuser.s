;
; puts is used to print entire strings to console
;
; r0 = address
;
_puts:
    mov r1, r0
    mov r0, 0b0
_puts_loop:
    mldb r0, r1
    bl _putc
    inc r1
    cmp r0, '\0'
    jne _puts_loop
_puts_end:
    ret

_putc:
    mov r1, r0
    mov r0, 0b00
    int 0x80
    ret

_getc:
    mov r0, 0b01
    int 0x80
    ret
