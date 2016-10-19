#ifndef VID_H
#define VID_H

/************************************************************************
                 HOW does PC's color display work:
 0. Before vid_init(), MUST set to mono mode, otherwise, no display at all.

 1. Screen = 25 rows X 80 cols = 2000 WORDs = [attribue byte|char byte]
                                 4000 bytes
 2. Video RAM at 0xB800, size=16 to 64 KB >> screen=4000 bytes;
    To display a screen: write 4000 bytes (2000 WORDs) to the video RAM
    at the ORG location; tell the display register where the ORG is.
    The display hardware will display the 4000 chars from ORG to the screen.

 3. Cursor shape and position can be changed by writing to the CRT register.

 4. Software must keep track of the Cursor position (row,col). For each char
    displayed, must advance the Curcor by one word, which may change(row,col):
               When col % 80  which may inc row,
               When row > 24, must scroll up one row.

5.  To Scroll up one row:
       inc org by one row (tell Hardware to display from the new org position)
       write a row of blanks to ORG + one screen  in video RAM
       if the last row exceeds VRAM size, must physically copy CURRENT screen
       to VRAM beginning and reset org to 0
*****************************************************************/


// vid_init() initializes the display org=0 (row,col)=(0,0)

int vid_init();

/* Move the cursor to the current position */
int move_cursor();

/*************************************************************************
 scroll(): scroll UP one line
**************************************************************************/
int scroll();

int putc(char c);

/*===========================================================================*
 *				set_6845				     *
 *===========================================================================*/
 /* Set a register pair inside the 6845.
 * Registers 10-11 control the format of the cursor (how high it is, etc).
 * Registers 12-13 tell the 6845 where in video ram to start (in WORDS)
 * Registers 14-15 tell the 6845 where to put the cursor (in WORDS)
 *
 * Note that registers 12-15 work in words, i.e. 0x0000 is the top left
 * character, but 0x0001 (not 0x0002) is the next character.  This addressing
 * is different from the way the 8088 addresses the video ram, where 0x0002
 * is the address of the next character.
 */
int set_VDC(u16 reg, u16 val);

// change color syscall from Umode, just for fun
int chcolor(u16 y);

#endif /* VID_H */
