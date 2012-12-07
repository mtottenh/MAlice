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



access.the.linux.kernel:
    push    rbx
    mov rbx, [rsp+16]
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



%macro output.char 1
    push %1
    call output_char
    add rsp,8
%endmacro


%macro output.string 1

%%start:	
	push rcx
	mov rcx, [%1]
	cmp  rcx ,0
	je   %%end
	output.char [%1]
	pop rcx
	inc %1
	jmp %%start
%%end:
	pop rcx
%endmacro

%macro output.minus 0			; void output_minus()
	push rax
	mov rax, '-'
	output.char rax
	pop rax
%endmacro

%macro output.int 1
    push %1
    call output_int
    add rsp, 8
%endmacro

output_int:
    push rbp
    mov rbp, rsp
    
    push rax
    push rbx
    push rdx
    
    cmp qword [rbp+16], 0
    jge L88
    
    output.minus
    neg qword [rbp+16]
L88:
    mov rax, [rbp+16]
    mov rdx, 0
    mov rbx, 10
    idiv rbx

    cmp rax, 0
    je L99
    
    output.int rax

L99:
    add rdx, '0'
    output.char rdx

    pop rdx
    pop rbx
    pop rax

    mov rsp,rbp
    pop rbp
    ret

output_char:
    push rax
    push rbx
    push rcx
    push rdx
	push r8		; r8..r11 are altered by Linux kernel interrupt
	push r9
	push r10
	push r11
	push qword [octetbuffer] ; (just to make output_char() re-entrant...)

	mov  rax, SYS_write		; Linux system call 4; i.e. write ()
	mov  rbx, stdout	; File descriptor 1 i.e. standard output


	; first argument
	mov  rcx, [rsp+80]	; fetch char from non-I/O-accessible segment
	mov  [octetbuffer], rcx	; load into 1-octet buffer
	lea  rcx, [octetbuffer]	; Address of 1-octet buffer
	mov  rdx, 1		; Output 1 character only
	int  80h		; Interrupt Linux kernel

    pop qword [octetbuffer]
	pop  r11
	pop  r10
	pop  r9
	pop  r8
    pop  rdx
    pop  rcx
    pop  rbx
    pop  rax
    ret 

section .data
    octetbuffer DQ 0
