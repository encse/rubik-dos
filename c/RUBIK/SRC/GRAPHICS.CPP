#include <dos.h>
#include <mem.h>
#include <alloc.h>
#include <stdlib.h>
#include <conio.h>
#include <values.h>
#include "src\graphics.h"
#include <stdio.h>

static int _COLOR = 0;
static int _BKCOLOR = 255;
static char huge *virt_ptr[5];
static char far *virt;
static char far *video;
static int dirty[5] = {0, 0, 0, 0, 0};  //a drawscreen sz�m�ra
static int dirty2[5] = {0, 0, 0, 0, 0}; //a cleardevice sz�m�ra;

int VESAgran, activepage, lap;
struct koordinata
{
  int x;
  int y;
  int z;
};

extern int line2d(int, int, int, int, struct scankoord *);

int setgraphmode()
{
  long i;
  unsigned int buf[256];
  unsigned far *bufptr;
  bufptr = &buf[0];
  virt = (char far *)farcalloc(0x50000L, 1);
  if (virt == NULL)
    return 1;

  for (i = 0; i < 5; i++)
    virt_ptr[i] = (char huge *)virt + i * 0x10000L;

  video = (char far *)MK_FP(0xa000, 0);
  asm {
    mov ax, 0x4f02
    mov bx, 0x101
    int 0x10
  }

  if (_AH) return 2;

  asm {
    les di, bufptr
    mov ax,0x4f01
    mov cx,bx
    int 0x10
  }

  VESAgran = 64 / buf[2];
  activepage = VESAgran;

  asm {
    mov ax,0x4f05
    mov bx,0
    mov dx,activepage
    int 0x10
  }
  lap = 1;
  return 0;
}

void drawscreen()
{
  for (int i = 0; i < 5; i++)
  {
    if (dirty[i])
    {
      lap = activepage = i;
      dirty[i] = 0;
      dirty2[i] = 1;
      asm {
      mov ax,0x4f05
      mov bx,0
      mov dx,activepage
      int 0x10
      }
      _fmemcpy(video, virt_ptr[lap], 0x8000);
      _fmemcpy(video + 0x8000, virt_ptr[lap] + 0x8000, 0x8000);
    }
  }
}

void closegraph()
{ 
  if (virt)
    farfree(virt);
  textmode(C80);
}

void setcolor(int color)
{
  _COLOR = color;
}
void paletteinit()
{
  unsigned int i;
  /*  i=16;
/  unsigned char R,G,B;
  R=G=B=10;
  asm {
  mov bx,i
  mov dh,R
  mov ch,G
  mov cl,B
  mov ax,0x1010
  int 0x10
  }
*/

  unsigned int r[16], g[16], b[16];
  unsigned char pal[255 * 3];
  unsigned char far *palptr;
  for (i = 0; i < 16; i++)
  {
    asm {
    mov bx,i
    mov ax,0x1015
    int 0x10
    }
    r[i] = _DH, g[i] = _CH, b[i] = _CL;
  }
  int s = 0;
  for (i = 0; i < 16; i++)
  { 
    // unsigned char R,G,B;
    int k;
    for (k = 0; k < 16; k++)
    {
      pal[s++] = (float)r[k] / 20 * (i + 4);
      pal[s++] = (float)g[k] / 20 * (i + 4);
      pal[s++] = (float)b[k] / 20 * (i + 4);
    }
  }
  pal[16 * 3] = pal[16 * 3 + 1] = pal[16 * 3 + 2] = 8;
  palptr = pal;
  asm {
  les dx,palptr
  mov ax,0x1012
  mov bx,0
  mov cx,256
  int 0x10
  }
  /* s=i*16+k;
      asm {
      mov bx,s
      mov dh,R
      mov ch,G
      mov cl,B
      mov ax,0x1010
      int 0x10
      }*/
}

char getpixel(int x, int y)
{
  return *((char huge *)virt_ptr[0] + (long)y * 640 + x);
}

void putpixel(int x, int y, char c)
{
  if (x < 640 && y < 480 && x >= 0 && y >= 0)
  {
    long s = (long)y * 640 + x;
    *((char huge *)virt_ptr[0] + s) = c;
    dirty[s >> 16] = 1;
  }
}

void cleardevice()
{
  int i;
  for (i = 0; i < 5; i++)
  {
    if (dirty2[i])
    {
      _fmemset(virt_ptr[i], _BKCOLOR, 0x8000);
      _fmemset(virt_ptr[i] + 0x8000, _BKCOLOR, 0x8000);
      dirty2[i] = 0;
    }
  }
}

void setbkcolor(int color)
{
  int i;
  for (i = 0; i < 5; i++)
  {
    _fmemset(virt_ptr[i], color, 0x8000);
    _fmemset(virt_ptr[i] + 0x8000, color, 0x8000);
  }
  _BKCOLOR = color;
}

void perspect(int *x, int *y, int z)
{
  if (z)
  {
    *x = 1000 * (long int)*x / (1000 + z);
    *y = 1000 * (long int)(*y) / (1000 + z);
  }
}

int line2d(int x1, int y1, int x2, int y2, struct scankoord *f)
{
  int dx = abs(x2 - x1);
  int dy = abs(y2 - y1);
  int n, m;
  int i, l;
  int s = 0;

  char huge *ptr;

  (y2 > y1) ? (n = 1) : (n = -1);
  (x2 > x1) ? (m = 1) : (m = -1);

  if (!dx && !dy)
  {
    ptr = (char huge *)virt_ptr[0] + (long)y1 * 640 + x1;
    *ptr = _COLOR;
    f->x = x1, f->y = y1;
    return 1;
  }

  (!dx) ? (m = 0) : 1;
  (!dy) ? (n = 0) : 1;
  ptr = (char huge *)virt_ptr[0] + (long)y1 * 640 + x1;

  if (dx > dy)
  {
    l = dx >> 1;
    for (i = 0; i <= dx; i++)
    {
      *ptr = _COLOR;
      f->x = x1 + i * m, f->y = y1 - s, f++;
      l += dy;
      if (dx <= l)
      {
        s++;
        l -= dx;
        ptr += (n * 640);
      }
      ptr += m;
    }
    return dx + 1;
  }

  if (dy >= dx)
  {
    l = dy >> 1;
    for (i = 0; i <= dy; i++)
    {
      *ptr = _COLOR;
      f->x = x1 + s * m, f->y = y1 - i, f++;
      l += dx;
      if (dy <= l)
      {
        s++;
        l -= dy;
        ptr += m;
      }
      ptr += (n * 640);
    }
    return dy + 1;
  }
  return -1;
}

void rectangle3d()
{
  int pixel, l, k, sor[4], y;
  int tomb[6] = {0, 1, 2, 3, 0, 1};
  int pos1 = 0, pos2, szin, eset;
  // long a;
  int seged;
  char huge *ptr;
  scankoord s[2000], p[4];
  koordinata extern boxkoord[4];
  for (l = 0; l < 4; l++)
  {
    p[l].x = boxkoord[l].x, p[l].y = boxkoord[l].y;
    perspect(&p[l].x, &p[l].y, boxkoord[l].z);
    p[l].x += 320, p[l].y = 240 - (p[l].y + 270);
  }

  sor[0] = 0, sor[3] = 3;
  for (l = 0; l < 4; l++)
  {
    if (p[sor[0]].y < p[l].y)
      sor[0] = l;
    if (p[sor[3]].y > p[l].y)
      sor[3] = l;
  }

  l = sor[0];
  if (sor[3] == tomb[l + 2])
    eset = 0;
  else
  {
    tomb[0] = sor[0];
    tomb[1] = sor[3];
    for (k = 0; k < 4; k++)
      if (k != sor[0] && k != sor[3])
        tomb[2] = k;
    for (k = 0; k < 4; k++)
      if (k != sor[0] && k != sor[3] && k != tomb[2])
        tomb[3] = k;

    if (p[tomb[2]].y < p[tomb[3]].y)
      sor[2] = tomb[2], sor[1] = tomb[3];
    else
      sor[1] = tomb[2], sor[2] = tomb[3];
    if (p[sor[1]].x > p[sor[0]].x)
      eset = 10;
    else
      eset = 11;
  }

  (l) ? 1 : (l = 4);

  szin = _COLOR,
#ifndef debug
  _COLOR = BLACK;
#else
  _COLOR = WHITE;
#endif
  switch (eset)
  {
  case 0:
    if (p[tomb[l + 1]].x > p[tomb[l - 1]].x)
      sor[2] = tomb[l + 1], sor[1] = tomb[l - 1];
    else
      sor[1] = tomb[l + 1], sor[2] = tomb[l - 1];
    pixel = line2d(p[sor[0]].x, p[sor[0]].y, p[sor[1]].x, p[sor[1]].y, &s[0]);
    pixel += line2d(p[sor[1]].x, p[sor[1]].y, p[sor[3]].x, p[sor[3]].y, &s[pixel]);
    pixel++;
    pos2 = pixel;
    pixel += line2d(p[sor[0]].x, p[sor[0]].y, p[sor[2]].x, p[sor[2]].y, &s[pixel]);
    pixel += line2d(p[sor[2]].x, p[sor[2]].y, p[sor[3]].x, p[sor[3]].y, &s[pixel]);
    break;
  case 10:
    pixel = line2d(p[sor[0]].x, p[sor[0]].y, p[sor[3]].x, p[sor[3]].y, &s[0]);
    pos2 = ++pixel;
    pixel += line2d(p[sor[0]].x, p[sor[0]].y, p[sor[1]].x, p[sor[1]].y, &s[pixel]);
    pixel += line2d(p[sor[1]].x, p[sor[1]].y, p[sor[2]].x, p[sor[2]].y, &s[pixel]);
    pixel += line2d(p[sor[2]].x, p[sor[2]].y, p[sor[3]].x, p[sor[3]].y, &s[pixel]);
    break;
  case 11:
    pixel = line2d(p[sor[0]].x, p[sor[0]].y, p[sor[1]].x, p[sor[1]].y, &s[0]);
    pixel += line2d(p[sor[1]].x, p[sor[1]].y, p[sor[2]].x, p[sor[2]].y, &s[pixel]);
    pixel += line2d(p[sor[2]].x, p[sor[2]].y, p[sor[3]].x, p[sor[3]].y, &s[pixel]);
    pos2 = ++pixel;
    pixel += line2d(p[sor[0]].x, p[sor[0]].y, p[sor[3]].x, p[sor[3]].y, &s[pixel]);
  }

  _COLOR = szin;
  y = s[pos1].y - 1;
  ptr = virt_ptr[0] + 640 * (long)y + 1;
  char huge *kezdeti_ptr = ptr;
  l = pos2 - 1;
  do
  {
    while (s[pos1].y != y && pos1 < l)
      pos1++;
    while (s[pos2].y != y && pos2 < pixel - 1)
      pos2++;
    if (s[pos1].x < s[pos2].x)
    {
      while ((s[pos1 + 1].y == y) && (s[pos1 + 1].x >= s[pos1].x))
        pos1++;
      while ((s[pos2 + 1].y == y) && (s[pos2 + 1].x <= s[pos2].x))
        pos2++;
    }
    else
    {
      while ((s[pos1 + 1].y == y) && (s[pos1 + 1].x <= s[pos1].x))
        pos1++;
      while ((s[pos2 + 1].y == y) && (s[pos2 + 1].x >= s[pos2].x))
        pos2++;
    }

    /*    while ((s[pos1+1].y==y)&&(s[pos1+1].x>=s[pos1].x)) pos1++;
    while ((s[pos2+1].y==y)&&(s[pos2+1].x<=s[pos2].x)) pos2++;
  */
#ifndef debug
    if ((k = s[pos2].x - s[pos1].x) > 0)
      _fmemset(ptr + s[pos1].x, _COLOR, k - 1);
    else if (k > 0)
      _fmemset(ptr + s[pos2].x, _COLOR, -(k + 1));
#endif
    ptr -= 640;
    y--;
  } while (y > s[pixel - 1].y);
  for (seged = 0; virt_ptr[0] < kezdeti_ptr; seged++)
  {
    kezdeti_ptr -= 65536L;
  }
  for (int i = (ptr - virt_ptr[0] - 640) >> 16; i <= seged; i++)
    dirty[i] = 1;
  //  for(int i=0;i<5;i++) dirty[i]=1;
}

void putpicture(char *src, int x, int y, int szel, int mag)
{
  long ptr = (long)y * 640 + x;
  int seged = ptr / 65536L;
  long vidoffset = ptr & 0xffff;
  if (y + mag >= 481)
    mag = 480 - y;
  if (activepage != seged)
  {
    lap = activepage = seged;
    asm {
      mov ax,0x4f05
      mov bx,0
      mov dx,activepage
      int 0x10
    }
  }
  dirty[seged] = 1;
  for (int i = 0; i < mag; i++) //mag
  {
    for (int k = 0; k < szel; k++) //szel
    {
      if (src[k])
      {
        if (vidoffset + k > 65535L)
        {
          seged++;
          lap++;
          activepage++;
          dirty[seged] = 1;
          asm {
            mov ax,0x4f05
            mov bx,0
            mov dx,activepage
            int 0x10
          }

          vidoffset = vidoffset - 0x10000L;
        }
        video[vidoffset + k] = src[k];
      }
    }

    vidoffset += 640;
    src += szel;
  }
}

void getpicture(char *dest, int x, int y, int szel, int mag)
{
  char huge *ptr = virt_ptr[0] + (long)y * 640 + x;
  for (int i = 0; i < mag; i++)
  {
    _fmemcpy(dest, ptr, szel);
    dest += szel;
    ptr += 640;
  }
}

int test_screen_draw_speed(int lapok)
{
  for (int i = 0; i < lapok; i++)
    dirty[i] = 1;
  drawscreen();
  return 1;
}

void teszt_szamolasi_sebesseg(void)
{
  int pixel, k, y;
  int pos1 = 0, pos2;
  char huge *ptr;
  scankoord s[2000];
  y = 478;
  ptr = virt_ptr[0] + 640 * (long)y + 1;

  pixel = line2d(0, 479, 0, 0, &s[0]);
  pixel += line2d(0, 0, 639, 0, &s[pixel]);
  pixel++;
  pos2 = pixel;
  pixel += line2d(0, 479, 639, 479, &s[pixel]);
  pixel += line2d(639, 479, 639, 0, &s[pixel]);
  do
  {
    pos1++;
    pos2++;

    if ((k = s[pos2].x - s[pos1].x) > 0)
      _fmemset(ptr + s[pos1].x, _COLOR, k - 1);
    else if (k > 0)
      _fmemset(ptr + s[pos2].x, _COLOR, -(k + 1));
    ptr -= 640;
    y--;
  } while (y > s[pixel - 1].y);
}

void rectangle2D(int x, int y, int szel, int mag, int szin, int keret)
{
  char huge *ptr;
  ptr = virt_ptr[0] + 640 * (long)y + x;
  if (x < 0)
    x = 0;
  if (x > 639)
    x = 639;
  if (x + szel > 639)
    szel = 639 - x;
  if (y < 0)
    y = 0;
  if (y > 479)
    y = 479;
  if (y + mag > 479)
    mag = 479 - y;
  _fmemset(ptr, keret, szel);
  ptr += 640;
  for (int i = 0; i < mag - 2; i++)
  {
    *ptr = ptr[szel - 1] = (char)keret;
    _fmemset(ptr + 1, szin, szel - 2);
    ptr += 640;
  }
  _fmemset(ptr, keret, szel);
  dirty[0] = 1;
}