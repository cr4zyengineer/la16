section .data
    msg db "\nHello World!\n\n\0"

_start:
    mov r0, 0x80                ; Inthandler setup
    mov r1, _inthandler
    intset r0, r1
    mov r0, 0b0                 ; Mapping to MPP
    mov r1, 0b0
    mpageprot r1, 0b00000101    ; Marking kernel page(0) as r^x
    mpagemap r0, r1
    mov r0, 0x100
    mov r1, 0x50
    mpagemap r0, r1
    mpageprot r1, 0b00000011    ; Marking stack page(0x100) as rw^
    mov el, 0b0                 ; Switch to userspace
    bl _userspace_entry
    hlt

_userspace_entry:
    mov r0, msg
    bl _puts
    ret

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

_inthandler:
    cmp r0, 0b00
    je _inthandler_out
    cmp r0, 0b01
    je _inthandler_in
    jmp _inthandler_end
_inthandler_out:
    out 0x00, r1
    jmp _inthandler_end
_inthandler_in:
    in rr, 0x00
_inthandler_end:
    intret
