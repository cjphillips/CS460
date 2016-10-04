        .global _syscall, _crt0, _exit, _getcs, auto_start

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
