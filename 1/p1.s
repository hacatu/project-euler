section .rodata
str: db "%i",0
section .text
align 16
global _start

extern printf
main:
    mov ebx,0
    mov eax,3
add3:
    add ebx,ebx
    add eax,3
    cmp eax,1000
    jl add3
    mov eax,5
add5:
    add ebx,ebx
    add eax,5
    cmp eax,1000
    jl add5
    mov eax,15
sub15:
    sub ebx,ebx
    add eax,15
    cmp eax,1000
    jl sub15
    push ebx
    push str
    call printf
    ret
