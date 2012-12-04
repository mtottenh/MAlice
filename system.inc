; File descriptors
%define stdin  0
%define stdout 1
%define stderr 2


; name for each syscall
%define SYS_nosys   0
%define SYS_exit    1
%define SYS_fork    2
%define SYS_read    3
%define SYS_write   4

section .text
align 4
; for local use by syscall macros
access.the.kernel:
    int 80h
    ret


;
access.the.linux.kernel:
    push    rbx
    mov rbx, [rsp + 16]
    push    rcx
    mov rcx, [rsp+32]
    push rdx
    mov rdx, [rsp+48]

    int 80h
;    add rsp, 8 ; bodge to remove ret addr from stack?
    pop rdx
    pop rcx
    pop rbx

    or  rax, rax
    js  .errno
    clc
    ret

.errno:
    neg rax
    stc
    ret

;

; takes system call number as arg
%macro  system  1
    mov rax, %1
    call    access.the.linux.kernel
%endmacro

; Macro's for calling syscalls
%macro  sys.exit    0
    system  SYS_exit
%endmacro

%macro  sys.fork    0
    system  SYS_fork
%endmacro

%macro  sys.read    0
    system  SYS_read
%endmacro

%macro  sys.write   0
    system  SYS_write
%endmacro



