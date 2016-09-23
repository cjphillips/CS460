        .global _main, _syscall, _exit, _getcs

        call  _main

! If main() ever returns, call exit(0)
        push  #0
        call  _exit


_syscall:
        int   80
        ret


_getcs:
        mov   ax, cs
        ret
