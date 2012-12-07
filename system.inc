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
	
	push  %1
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
	mov  rcx, [rsp+ 72]	; fetch char from non-I/O-accessible segment
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
	 add  rsp, 8
    
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

%macro output.int 1
%%start:
         ; rax=N then N/10, rdx=N%10, rbx=10

         push rax                ; save registers
         push rbx
         push rdx

         cmp  %1, 0 ; minus sign for negative numbers
         mov  rax, %1       ; rax = N

         jge  %%print

         output.minus
         neg  %1
%%print:

         mov  rdx, 0              ; rdx:rax = N (unsigned equivalent of "cqo")
         mov  rbx, 10
         idiv rbx                ; rax=N/10, rdx=N%10

         cmp  rax, 0              ; skip if N<10
         je   %%end
    ; THIS SEEMS TO BE BREAKING???
         add  rdx, '0'           ; output char for digit N % 10
    ; 
		 output.char rdx
         jmp %%print			  ; output.int (N / 10)

%%end:
         add  rdx, '0'           ; output char for digit N % 10
		 output.char rdx

         pop  rdx                ; restore registers
         pop  rbx
         pop  rax
%endmacro

%macro output.minus 0			; void output_minus()
	push rax
	mov rax, '-'
	output.char rax
	pop rax
%endmacro

section .data
    octetbuffer DQ 0
