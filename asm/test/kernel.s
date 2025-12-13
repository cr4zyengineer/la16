;
; data section which contains strings such as this hello world string
;
section .data
    msg db "\nHello World!\n\n\0"

;
; start symbol of the kernel
;
; sets up everything and switches after setup to userspace
;
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

;
; Interruption handler
;
; handles interruptions by the userspace program
;
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
