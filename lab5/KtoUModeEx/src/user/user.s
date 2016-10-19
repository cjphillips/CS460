        .globl _syscall, _crt0, _exit, _getcs, auto_start
        .globl _getc
auto_start:
        call  _crt0

! If main() ever returns, call exit(0)
        push  #0
        call  _exit


_syscall:
        int   80
        ret


_getcs:
        mov   ax, cs
        ret

_getc:
        xorb ah, ah
        int  0x16
        andb al, #0x7F
        ret
