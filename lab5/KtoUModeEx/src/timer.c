#include "include/type.h"
#include "include/vid.h"

#define LATCH_COUNT     0x00	   /* cc00xxxx, c = channel, x = any */
#define SQUARE_WAVE     0x36	   /* ccaammmb, a = access, m = mode, b = BCD */

#define TIMER_FREQ   1193182L	   /* clock frequency for timer in PC and AT */
#define TIMER_COUNT  TIMER_FREQ/60 /* initial value for counter*/

#define TIMER0       0x40
#define TIMER_MODE   0x43
#define TIMER_IRQ       0

#define CLOCK_POS_ROW   0
#define CLOCK_POS_COL   70

u16 tick, second, minute, hour;

char clock[8];

extern u16 row, column;

int enable_irq(u16 irq_nr)
{
  lock();
    out_byte(0x21, in_byte(0x21) & ~(1 << irq_nr));
}

int timer_init()
{
  /* Initialize channel 0 of the 8253A timer to e.g. 60 Hz. */

  printf("Initializing timer ... ");
  tick = second = minute = hour = 0;
  out_byte(TIMER_MODE, SQUARE_WAVE);	// set timer to run continuously
  out_byte(TIMER0, TIMER_COUNT);	// timer count low byte
  out_byte(TIMER0, TIMER_COUNT >> 8);	// timer count high byte
  enable_irq(TIMER_IRQ);
  printf("done.\n");
}

void print_time()
{
  int i;
  u16 old_row, old_col;


  old_row = row;
  old_col = column;
  row = CLOCK_POS_ROW;
  column = CLOCK_POS_COL;
  for(i = 0; i < 8; i++)
  {
    putc(clock[i]);
  }
  row = old_row;
  column = old_col;

  move_cursor();
}

void get_time()
{
  second++;

  if (second % 60 == 0)
  {
    second = 0;
    minute++;

    if (minute % 60 == 0)
    {
      minute = 0;
      hour++;

      if (hour % 24 == 0)
      {
        hour = 0;
      }
    }
  }

  clock[0] = '0' + (hour / 10);
  clock[1] = '0' + (hour % 10);
  clock[2] = ':';
  clock[3] = '0' + (minute / 10);
  clock[4] = '0' + (minute % 10);
  clock[5] = ':';
  clock[6] = '0' + (second / 10);
  clock[7] = '0' + (second % 10);
}

/*===================================================================*
 *		    timer interrupt handler       		     *
 *===================================================================*/
int thandler()
{
  tick++;
  tick %= 60;
  if (tick == 0){                      // at each second
    //get_time();

    printf("1 second timer interrupt in ");
    running->inkmode > 1 ? putc('K') : putc('U');
    printf("mode\n");

  }
//  print_time();

  out_byte(0x20, 0x20);                // tell 8259 PIC EOI
}
