#include "src\backtrac.h"
#include <fstream.h>
#include <string.h>
#include <conio.h>
#include <stdio.h>
#include "src\kirak.h"

#define FULL 54

ofstream out;
int oldKocka[6][9];

extern void forgat(int, int, int, int, int); //tengely,reteg,fok,rajzol,elmozgatas
extern int sajat_kocka_kirakasa;
extern void kocka_rajz();
void legrosszabb(int (*kocka)[6][9], int melyseg);

long pow(long s, int k)
{
  if (!k)
    return 1;

  if (k == 1)
    return s;

  long ret = s;

  k--;
  while (k)
  {
    ret *= s;
    k--;
  }
  return ret;
}

void keres(char *s, char *p)
{
  switch (*p)
  {
    case 0: strcpy(s, "x1+"); break;
    case 1: strcpy(s, "x2+"); break;
    case 2: strcpy(s, "x4+"); break;
    case 3: strcpy(s, "y1+"); break;
    case 4: strcpy(s, "y2+"); break;
    case 5: strcpy(s, "y4+"); break;
    case 6: strcpy(s, "z1+"); break;
    case 7: strcpy(s, "z2+"); break;
    case 8: strcpy(s, "z4+"); break;
    case 9: strcpy(s, "z4-"); break;
    case 10: strcpy(s, "z2-"); break;
    case 11: strcpy(s, "z1-"); break;
    case 12: strcpy(s, "y4-"); break;
    case 13: strcpy(s, "y2-"); break;
    case 14: strcpy(s, "y1-"); break;
    case 15: strcpy(s, "x4-"); break;
    case 16: strcpy(s, "x2-"); break;
    case 17: strcpy(s, "x1-"); break;
  }
}

void play(char *p, int hossz)
{
  char s[4] = "   ";
  p[hossz] = 255;

  while (*p != 255)
  {
    keres(s, p);
    gyorsforgatas(s);
    if (sajat_kocka_kirakasa)
    {
      kocka_rajz();
      getch();
    }
    p++;
  }
}

void expand(char *s, char *p)
{
  int i = 0;

  while (*p != 255 && i < 1996)
  {
    keres(s + i, p);
    i += 3;
    p++;
  }
  s[i] = 0;
}

int novel(char *szam, int pos)
{
  pos--;
  if (pos < 0)
    return 1;

  if (szam[pos] == 17)
  {
    szam[pos] = 0;
    return novel(szam, pos);
  }
  szam[pos]++;
  return 0;
}

int w(int (*kocka)[6][9])
{
  int ret = 0;
  for (int i = 0; i < 6; i++)
  {
    for (int k = 0; k < 9; k++)
    {
      if ((*kocka)[i][k] == (*kocka)[i][4])
        ret++;
    }
  }
  return ret;
}
int w2(int (*kocka)[6][9])
{
  int ret = 0;
  char szinhelye[] = {-1, 0, 1, -1, 2, -1, -1, -1, -1, 3, -1, -1, -1, -1, 4, 5};
  char szinszam[6];
  int max;

  for (int i = 0; i < 6; i++)
  {
    memset(szinszam, 0, 6);
    int l = 0;
  
    for (int k = 0; k < 9; k++)
    {
      int s = szinhelye[(*kocka)[i][k]];
      szinszam[s]++;
      if (!l)
      {
        l = 1, max = s;
      }
      else if (szinszam[max] < szinszam[s])
        max = s;
    }
  
    ret += szinszam[max];
  }
  
  return ret;
}

int melyiksarok(int oldal, int lap)
{
  int ret = 0;
  switch (oldal)
  {
    case 0:
      switch (lap)
      {
        case 0: ret = 0; break;
        case 2: ret = 3; break;
        case 6: ret = 1; break;
        case 8: ret = 2; break;
      }
      break;
    case 1:
      switch (lap)
      {
        case 0: ret = 5; break;
        case 2: ret = 6; break;
        case 6: ret = 4; break;
        case 8: ret = 7; break;
      }
      break;
    case 2:
      switch (lap)
      {
        case 0: ret = 1; break;
        case 2: ret = 2; break;
        case 6: ret = 5; break;
        case 8: ret = 6; break;
      }
      break;
    case 3:
      switch (lap)
      {
        case 0: ret = 4; break;
        case 2: ret = 7; break;
        case 6: ret = 0; break;
        case 8: ret = 3; break;
      }
      break;
    case 4:
      switch (lap)
      {
        case 0: ret = 0; break;
        case 2: ret = 1; break;
        case 6: ret = 4; break;
        case 8: ret = 5; break;
      }
      break;
    case 5:
      switch (lap)
      {
        case 0: ret = 2; break;
        case 2: ret = 3; break;
        case 6: ret = 6; break;
        case 8: ret = 7; break;
      }
      break;
  }
  return ret;
}

int p(int (*kocka)[6][9])
{
  int ret = 0;
  int rosszsarok[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
  for (int i = 0; i < 6; i++)
  {
    for (int k = 0; k < 9; k++)
    {
      if ((*kocka)[i][k] != (*kocka)[i][4])
      { 
        int s = 0;
        if ((*kocka)[i][k] != (*kocka)[i + ((i % 2) ? -1 : 1)][4]) 
        {
          // ha a szemk�zti oldalon van
          s = 2;
        }
        else 
        {
          // ha szomsz�dos oldalon van
          s = 1;
        }
        ret += s;

        if (i == 0 || i == 2 || i == 6 || i == 8)
        {
          s--;
          int melyik = melyiksarok(i, k);
          rosszsarok[melyik] += (1 << s);
          // sarokban gond lehet, ekkor modos�tunk a ret-en
          if (rosszsarok[melyik] == 4)
            ret--;
        }
      }
    }
  }

  for (i = 0; i < 9; i++)
  {
    if (rosszsarok[i])
      ret--;
  }
  return -ret;
}

void playWithoutDrawing(char *p)
{
  int i = 0, tengely;

  do
  {
    switch (p[i])
    {
      case 'x': tengely = 0; break;
      case 'y': tengely = 2; break;
      case 'z': tengely = 1; break;
    }

    int reteg = (char)p[++i] - 48;
    forgat(tengely, reteg, p[++i] == '+' ? 1 : -1, 0, 0);
    i++;
  } while (p[i]);
}

int test(int (*kocka)[6][9], char *utvonal, int hossz)
{
  char buf[2000];
  utvonal[hossz] = 255;
  expand(buf, utvonal);
  playWithoutDrawing(buf);
  int ret = w(kocka);
  memcpy(kocka, oldKocka, sizeof(oldKocka));
  return ret;
}

void vissza(int (*kocka)[6][9], char *utvonal, int *akthossz, long *holJartunk)
{
  (*akthossz)--;
  if (*akthossz >= 0)
  {
    char *s = "   ";
    char p[] = {0, 0};
    p[0] = 17 - utvonal[(*akthossz)];
    keres(s, p);
    playWithoutDrawing(s);
    utvonal[*akthossz] = 255;
  }
}

int next(int (*kocka)[6][9], long *holJartunk, int hossz)
{
  int aktw = 0, maxw = 255;
  int max = 255;
  int i = 0, l = 0;
  while (i < 18)
  {
    if (!(holJartunk[hossz] & (1l << i)))
    {
      char *s = "   ";
      char a[] = {0, 0};
      a[0] = i;
      keres(s, a);
      playWithoutDrawing(s);

      if (!l)
      {
        max = i;
        maxw = w(kocka);
        l = 1;
      }
      else
      {
        aktw = w(kocka);
        if (maxw > aktw)
        {
          max = i;
          maxw = aktw;
        }
      }
      a[0] = 17 - i;
      keres(s, a);
      playWithoutDrawing(s);
    }
    i++;
  }
  //  out<<"max"<<max<<endl;
  return max;
}

int kovetkezo(int (*kocka)[6][9], char *utvonal, int *akthossz, long *holJartunk)
{
  if ((*akthossz) < 0)
    return 1;
  int n = next(kocka, holJartunk, *akthossz);

  // Ellentettj�t nem csin�ljuk
  if ((*akthossz) > 0 && n == 17 - utvonal[(*akthossz) - 1])
  {
    holJartunk[*akthossz] = holJartunk[*akthossz] + (1l << n);
    n = next(kocka, holJartunk, *akthossz);
  }

  if (n == 255)
  {
    vissza(kocka, utvonal, akthossz, holJartunk);
    return kovetkezo(kocka, utvonal, akthossz, holJartunk);
  }
  else
  {
    holJartunk[*akthossz] = holJartunk[*akthossz] + (1l << n);
    utvonal[*akthossz] = n;
    (*akthossz)++;
    holJartunk[*akthossz] = 0;
    utvonal[*akthossz] = 255;

    char *s = "   ";
    char p[] = {0, 0};
    p[0] = n;
    keres(s, p);
    playWithoutDrawing(s);
  }
  return 0;
}

char backTrack(int (*kocka)[6][9], int melyseg)
{ 
  melyseg++;
  char utvonal[20];
  long holJartunk[20];
  int akthossz = 0;
  int akt = w(kocka);
  int c = 0;
  out.open("x.x");
  memcpy(oldKocka, kocka, sizeof(oldKocka));
  memset(utvonal, 0, melyseg);
  utvonal[0] = 255;
  memset(holJartunk, 0, 20 * sizeof(long));
  int i = 0;
  while (!c && akt != FULL && !kbhit())
  {
    int sw = akt;
    int sw2 = w2(kocka);
    int sp = p(kocka);
    if (melyseg == akthossz || (melyseg == akthossz + 1 && (sw < 30 || sw2 < 42 || sp < -43)) || (melyseg == akthossz + 2 && (sw < 14 || sw2 < 30 || sp < -71)) || (melyseg == akthossz + 3 && (sw < 10 || sw2 < 23 || sp < -74)) || (melyseg == akthossz + 4 && (sw2 < 16)))
    {
      vissza(kocka, utvonal, &akthossz, holJartunk);
    }
    c = kovetkezo(kocka, utvonal, &akthossz, holJartunk);
    out << i++ << endl;
    akt = w(kocka);
  }
  memcpy(kocka, oldKocka, sizeof(oldKocka));
  if (akt == FULL)
    play(utvonal, akthossz);
  return akt == FULL;
}

void legrosszabb(int (*kocka)[6][9], int melyseg)
{
  out.open("x.x");
  textmode(C80);
  melyseg = 6;
  {
    char utvonal[20];
    char maxut[20];
    memset(utvonal, 0, melyseg);

    memcpy(oldKocka, kocka, sizeof(oldKocka));

    int c = 0;
    int min = 0;
    long i = 0;
    long oszto = pow(18l, melyseg);

    while (!c)
    {
      c = novel(utvonal, melyseg);
      int akt = test(kocka, utvonal, melyseg);
      if (akt > min)
      {
        min = akt;
        memcpy(maxut, utvonal, melyseg);
      }
      if (min > 96)
        c = 1;
      if (!(i % 5000))
      {
        gotoxy(1, 1), printf("%f%% %d > %d", 100. * i / oszto, min, akt);
      }
      if (akt == min)
      {
        gotoxy(1, 2), printf("%f%% %d > %d", 100. * i / oszto, min, akt);
      }
      i++;
    }
    gotoxy(1, 1);
    printf("%f%%", 100. * i / oszto);
    out << "A legrosszabb �rt�k " << melyseg << " m�lys�g mellett: ";
    out << min << endl;
    memcpy(kocka, oldKocka, sizeof(oldKocka));
  }
}