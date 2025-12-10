section .data
    str_hw db "LA16 ShellOS v0.0\n\0"
    str_prompt db "# \0"
    str_exit db "exit\0"
    str_clear db "clear\0"
    str_ver db "ver\0"
    str_echo db "echo\0"
    str_unknown_command db "Unknown command: \0"
    str_newline db '\n'
    str_null db '\0'
    str_del db "\b \b\0"
    str_clear_screen db 0b00011011,0b01011011,0b00110010,0b01001010,0b00011011,0b01011011,0b01001000,0b00000000

section .bss
    buffer 100

_start:
    bra _switch_userspace
    mov ra, str_hw
    bra _puts
_start_loop:
    mov ra, str_prompt
    bra _puts
    mov ra, buffer
    bra _readline
    mov rb, str_exit    ; exit command
    bra _strcmp
    jeq _start_exit
    mov rb, str_clear   ; clear command
    bra _strcmp
    beq _clear_command
    mov rb, str_ver     ; ver command
    bra _strcmp
    beq _ver_command
    mov rb, str_echo    ; echo command
    bra _strcmp
    beq _echo_command
    bra _unknown_command
    jmp _start_loop
_start_exit:
    hlt

_unknown_command:
    mov ra, str_null
    mov rb, buffer
    bra _strcmp
    reteq
    mov ra, str_unknown_command
    bra _puts
    mov ra, buffer
    bra _puts
    mov ra, str_newline
    bra _puts
    ret

_echo_command:
    mov ra, buffer
    bra _readline
    bra _puts
    mov ra, '\n'
    bra _putc
    mov ra, _start_loop
    bra _return_to_addr

_clear_command:
    mov ra, str_clear_screen
    bra _puts
    mov ra, _start_loop
    bra _return_to_addr

_ver_command:
    mov ra, str_hw
    bra _puts
    mov ra, _start_loop
    bra _return_to_addr

_return_to_addr:
    mld fp, fp
    mld fp, fp
    bra _reroute_ret_to_addr
    ret

_reroute_ret_to_addr:
    mov sp, fp        ; Restoring callers stack pointer in callee(us)
    add sp, 18          ; Accessing PC address
    pop rb              ; Pop return address to ra
    sub ra, 4           ; Subtract from ra so the return address is alligned correctly
    psh ra              ; Push return address back onto stack
    ret

_puts:
    mov rb, ra
    mov ra, 0b0
_puts_loop:
    mld8 ra, rb
    bra _putc
    inc rb
    cmp ra, '\0'
    jnq _puts_loop
_puts_end:
    ret

_gets:
    mov rb, ra      ; Moving ra into rb
    mov rr, 0b0     ; Nullifying rr
_gets_loop:
    bra _getc       ; Getting character which gets witten to rr
    mov ra, rr      ; Moving rr to ra because putc takes characters by ra
    bra _putc       ; Calling putc to echo user input
    cmp ra, '\n'    ; Comparing the finding with a newline
    jeq _gets_end   ; Jumping if ra contains a newline chatacter to end the loop
    mst8 ra, rb     ; Storing ra at the address of rb
    inc rb          ; Increment rb
    jmp _gets_loop  ; Jumping to loop beginning
_gets_end:
    mst8 '\0', rb   ; Storing nullterminator into address contained in rb
    ret

_readline:
    mov rb, ra      ; Moving ra into rb
    mov rc, ra      ; Moving ra into rc to keep a copy of ra
    mov rr, 0b0     ; Nullifying rr
_readline_loop:
    bra _getc       ; Getting character which gets witten to rr
    mov ra, rr      ; Moving rr to ra because putc takes characters by ra
    bra _putc       ; Calling putc to echo user input
    cmp ra, '\n'    ; Comparing the finding with a newline
    jeq _readline_end   ; Jumping if ra contains a newline chatacter to end the loop
    cmp ra, 0x7F
    jeq _readline_back
    mst8 ra, rb     ; Storing ra at the address of rb
    inc rb          ; Increment rb
    jmp _readline_loop  ; Jumping to loop beginning
_readline_back:
    cmp rb, rc      ; We only delete if rb is offset to rc, rc is the copy of the start address
    jeq _readline_loop
    dec rb          ; Decrementing to effectively go back
    mov ra, str_del ; Moving the address of the deletion string to delete one character to ra
    bra _puts       ; Branch to puts to to clear the character
    jmp _readline_loop  ; Reentering the loop
_readline_end:
    mst8 '\0', rb   ; Storing nullterminator into address contained in rb
    ret

_putc:
    mov rb, ra      ; The character
    mov ra, 0x01    ; Write Syscall
    int 0x80
    ret

_getc:
    mov rb, 0x00    ; Nullify rb
    mov ra, 0x00    ; Read Syscall
    int 0x80
    ret

;
; String Compare
;
; ra = address of first string
; rb = address of second string
;
;
_strcmp:
    mld8 ea, ra ; Loading both into memory
    mld8 eb, rb
    cmp ea, eb  ; Comparing both
    psh cf
    cmp ea, '\0'
    jeq _strcmp_end
    pop cf
    inc ra
    inc rb
    jeq _strcmp
_strcmp_end:
    pop cf
    ret

;
; Kernel land test
;
_switch_userspace:
    mov ra, 0x80
    mov rb, _int_handler
    intst ra, rb
    mov el, 0x0     ; Now actually switch to userspace
    ret;

_int_handler:
    cmp ra, 0x0     ; Is it Read?
    jeq _int_handler_syscall_read
    cmp ra, 0x1     ; Is it Write?
    jeq _int_handler_syscall_write
    jmp _int_handler_end
_int_handler_syscall_read:
    ior 0x00, rr
    jmp _int_handler_end
_int_handler_syscall_write:
    iow 0x00, rb
_int_handler_end:
    iret
