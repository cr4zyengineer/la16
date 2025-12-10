section .data
    c db 'a'
    c2 db 'b'
    rk db "Hi\0"
    meow db "mrppp\0"

section .bss
    test 10

_start:
    mov ra, test
    mov rb, 0x1
    mov rc, 0
    mov rd, 0x5
    jmp _start_end
    mov rd, ra
_start_end:
    hlt
    mov rb, rc

