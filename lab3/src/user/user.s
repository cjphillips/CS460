        .global _main, _syscall, _exit, _getcs
        .global _getc, _putc, _color

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


_getc:
        xorb  ah, ah     ! Clear the ah register
        int   0x16       ! Call BIOS to get a character into ax
        andb  al, #0x7F  ! 7-bit ASCII
        ret


_putc:
        push  bp
        mov   bp, sp

        movb  al, 4[bp]  ! Get the character into aL
        movb  ah, #14    ! ah = 14
        mov   bx, _color ! Cyan
        int   0x10       ! Call BIOS to display the character

        pop bp
        ret
