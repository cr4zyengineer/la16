;
; user space entry
;
; basically the entry of our user program
;

_userspace_entry:
    bl _getc                ; Getting keyboard input
    mov r0, rr              ; Moving return value back to rr
    bl _putc                ; Echoing keyboard input back out
    jmp _userspace_entry    ; Looping!
    ret                     ; We will never return lol

; Multiline comments work now aswell!!
/*
_userspace_entry:
    mov r0, msg
    bl _puts
    ret
*/
