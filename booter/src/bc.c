#include "type.h"

char mbr[512];
char ans[64];

int partition, bsector;

int main()
{
  short i;
  struct partition *p;

  // initialize the dap struct
  dp = &dap;
  dp->len  = 0x10;        // must be 16
  dp->zero = 0;           // must be 0
  dp->nsector = 1;        // load one sector
  dp->addr = 0;           // will set to addr
  dp->segment = BOOTSEG;  // load to which segment
  dp->s1 = 0;             // will set to LBA sector#
  dp->s2 = 0;             // high 4-byte address s2=0

  get_sector((u32)0, (u16)mbr); // get MBR

  color = YELLOW;

  printf("Partition Table:\n");
  p = (struct partition *)(&mbr[0x1be]);
  printf("#   Type    Start Sector    Nr. Sectors\n");
  printf("---------------------------------------\n");

  for(i = 1; i <= 4; i++)
  {
    printf("%d   %x       %l       %l\n",
	   i,
	   (short)p->type,
	   (long)p->start_sector,
	   (long)p->nr_sectors);

    if (i == 1)
    {
      partition = 1;
      bsector = p->start_sector;
    }
    p++;
  }
  printf("---------------------------------------\n");


  printf("Enter your name: ");
  gets(ans);

  if (strcmp(ans, "quit") == 0)
  {
    return;
  }

  printf("\n\nHello, %s!\n", ans);

  dp = &dap;
  dp->len = 0x10;
  dp->zero = 0;
  dp->nsector = 2;
  dp->segment = BOOTSEG;
  dp->s2 = 0;
  color = RED;

  printf("*****************  MTX booter : ***************** \n");
  printf("partition=%d bsector=%d\n", partition, bsector);
  getc();

  get_block((u32)1, buf);
  sp = (SUPER *)buf;
  printf("magic = %x\n", sp->s_magic);

  get_block((u32)2, buf);
  gp = (GD *)buf;
  printf("bmap=%l imap=%l inode_table=%l\n",
	 gp->bg_block_bitmap, gp->bg_inode_bitmap, gp->bg_inode_table);

   printf("bmap=%d imap=%d inode_table=%d\n",
  (u16)gp->bg_block_bitmap, (u16)gp->bg_inode_bitmap, (u16)gp->bg_inode_table);
}

// load a disk sector to (DS, addr), where addr is an offset in segment
int get_sector(u32 sector, char *addr)
{
  dp->addr    = addr;
  dp->s1      = (u32)sector;
  diskr();    // call int13-43 in assembly
}

int get_block(u32 blk, char *buf)
{
  dp->addr = (u16)buf;
  dp->s1   = (u32)(bsector + (u32)(2*blk));
  diskr();
}
