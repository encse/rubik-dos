#include "src\kirak.h"
#include <string.h>
#include <conio.h>
#include <stdio.h>
//ofstream out("algoritmus.txt");
extern void forgat(int, int, int, int, int); //tengely,reteg,fok,rajzol,elmozgatas
extern void kocka_rajz();
extern void atad();
extern void Abort(int, char *);
extern int optimize(char *);
extern int optimalizalas_foka;
extern int sajat_kocka_kirakasa;
int oldkocka[2][6][9];

char gyorsforgatas(int);
char algoritmus[2000];
int hossz = 0;
void feljegyez(char *);

int (*Kocka)[6][9];

int kirakva_teteje()
{
  int i, k;
  int ret = 1;
  for (i = 0; i < 9; i++)
  {
    if ((*Kocka)[5][i] != (*Kocka)[5][4])
      ret = 0;
  }
  for (i = 0; i < 4; i++)
  {
    for (k = 2; k <= 8; k += 3)
      if ((*Kocka)[i][4] != (*Kocka)[i][k])
        ret = 0;
  }
  return ret;
}

int kirakva_kozepe()
{
  int i, k;
  for (i = 0; i < 4; i++)
  {
    for (k = 1; k <= 7; k += 6)
      if ((*Kocka)[i][4] != (*Kocka)[i][k])
        return 0;
  }
  return 1;
}

int kirakva_alja()
{
  int i, k;
  for (i = 0; i < 9; i++)
    if ((*Kocka)[4][i] != (*Kocka)[4][4])
      return 0;
  for (i = 0; i < 4; i++)
  {
    for (k = 0; k <= 6; k += 3)
      if ((*Kocka)[i][4] != (*Kocka)[i][k])
        return 0;
  }
  return 1;
}

int kirakva_kocka()
{
  if (!kirakva_teteje() || !kirakva_kozepe() || !kirakva_alja())
    return 0;

  return 1;
}

void feljegyez(char *p)
{
  int i = 0, tengely;
  strcpy(algoritmus + hossz, p);
  if (hossz < 1950)
  {
    hossz += strlen(p);
    do
    {
      switch (p[i])
      {
      case 'x':
        tengely = 0;
        break;
      case 'y':
        tengely = 2;
        break;
      case 'z':
        tengely = 1;
      }
      int reteg = (char)p[++i] - 48;
      forgat(tengely, reteg, p[++i] == '+' ? 1 : -1, 0, 0);
      i++;
    } while (p[i]);
  }
  else
  {
    if (!sajat_kocka_kirakasa)
    {
      FILE *fp;
      fp = fopen("error.dat", "wt");
      fwrite("RUBIK", 5, 1, fp);
      fwrite(oldkocka[0], 6 * 9 * sizeof(int), 1, fp);
      fclose(fp);
      Abort(10, "Hiba 10:\n\rVal�sz�n�leg egy v�gtelen ciklus miatt a program fut�sa megszakadt.\nK�rlek k�ldd el az error.dat f�jlt a k�vetkez� c�mre: ncs@valerie.inf.elte.hu");
    }
  }
}

char gyorsforgatas(char *buf)
{
  int i = 0, tengely;
  int hossz = strlen(buf);
  char a = 0;
  if (!(strlen(buf) % 3))
  {
    for (i = 0; i < hossz && !a; i++)
    {
      switch (buf[i])
      {
      case 'x':
        tengely = 0;
        break;
      case 'y':
        tengely = 2;
        break;
      case 'z':
        tengely = 1;
      }
      buf[++i] -= 48;
      int reteg;
      if ((char)buf[i] < 8)
        reteg = (char)buf[i];
      int irany;
      if (buf[++i] == '+')
        irany = 1;
      else if (buf[i] == '-')
        irany = -1;
      forgat(tengely, reteg, irany, 1, 0);
      if (kbhit())
      {
        a = getch();
        if (a != 27)
          a = 0;
      }
    }
  }
  return a;
}

char gyorsforgatas(int opt)
{
  int i = 0, tengely;
  if (opt)
    hossz = optimize(algoritmus);
  if (hossz > 1950)
  {
    FILE *fp;
    fp = fopen("error.dat", "wt");
    fwrite("RUBIK", 5, 1, fp);
    fwrite(oldkocka[0], 6 * 9 * sizeof(int), 1, fp);
    fclose(fp);
    Abort(10, "Hiba 10:\n\rOptimaliz�l�si hiba miatt a program fut�sa megszakadt.\nK�rlek k�ldd el az error.dat f�jlt a k�vetkez� c�mre: ncs@valerie.inf.elte.hu");
  }
  char a = 0;
  for (i = 0; i < hossz && !a; i++)
  {
    switch (algoritmus[i])
    {
    case 'x':
      tengely = 0;
      break;
    case 'y':
      tengely = 2;
      break;
    case 'z':
      tengely = 1;
    }
    int reteg = (char)algoritmus[++i] - 48;
    forgat(tengely, reteg, algoritmus[++i] == '+' ? 1 : -1, 1, 0);
    if (sajat_kocka_kirakasa)
    {
      kocka_rajz();
      a = getch();
      if (a != 27)
        a = 0;
    }
    else if (kbhit())
    {
      a = getch();
      if (a != 27)
        a = 0;
    }
  }
  return a;
}

int e_kozeprol_keresztbe()
{
  int i = 0;
  if ((*Kocka)[2][1] == (*Kocka)[5][4])
  {
    feljegyez("y7-"), i = 7;
  }
  else if ((*Kocka)[2][7] == (*Kocka)[5][4])
  {
    feljegyez("y7+"), i = 1;
  }
  else if ((*Kocka)[0][1] == (*Kocka)[5][4])
  {
    feljegyez("y7-y7-"), i = 7;
  }
  else if ((*Kocka)[0][7] == (*Kocka)[5][4])
  {
    i = 1;
  }
  else if ((*Kocka)[1][1] == (*Kocka)[5][4])
  {
    i = 7;
  }
  else if ((*Kocka)[1][7] == (*Kocka)[5][4])
  {
    feljegyez("y7+y7+"), i = 1;
  }
  else if ((*Kocka)[3][1] == (*Kocka)[5][4])
  {
    feljegyez("y7+"), i = 7;
  }
  else if ((*Kocka)[3][7] == (*Kocka)[5][4])
  {
    feljegyez("y7-"), i = 1;
  }
  if (i == 1)
  {
    if ((*Kocka)[0][4] == (*Kocka)[2][1])
    {
      feljegyez("y4-z1+y4+");
    }
    else if ((*Kocka)[1][4] == (*Kocka)[2][1])
    {
      feljegyez("y4+z1+y4-");
    }
    else if ((*Kocka)[2][4] == (*Kocka)[2][1])
    {
      feljegyez("z1+");
    }
    else if ((*Kocka)[3][4] == (*Kocka)[2][1])
    {
      feljegyez("y4+y4+z1+y4-y4-");
    }
  }
  else if (i == 7)
  {
    if ((*Kocka)[0][4] == (*Kocka)[2][7])
    {
      feljegyez("y4-z1-y4+");
    }
    else if ((*Kocka)[1][4] == (*Kocka)[2][7])
    {
      feljegyez("y4+z1-y4-");
    }
    else if ((*Kocka)[2][4] == (*Kocka)[2][7])
    {
      feljegyez("z1-");
    }
    else if ((*Kocka)[3][4] == (*Kocka)[2][7])
    {
      feljegyez("y4+y4+z1-y4-y4-");
    }
  }
  return i;
}

int e_keresztbol_kozepre()
{
  int i = 0;
  if ((*Kocka)[2][5] == (*Kocka)[5][4])
  {
    feljegyez("z1-"), i = 1;
  }
  else if ((*Kocka)[1][5] == (*Kocka)[5][4])
  {
    feljegyez("y7+z1-"), i = 1;
  }
  else if ((*Kocka)[0][5] == (*Kocka)[5][4])
  {
    feljegyez("y7-z1-"), i = 1;
  }
  else if ((*Kocka)[3][5] == (*Kocka)[5][4])
  {
    feljegyez("y7+y7+z1-"), i = 1;
  }
  return i;
}

int e_lentrol_kozepre()
{
  int i = 0;
  if ((*Kocka)[2][3] == (*Kocka)[5][4])
    i = 1;
  else if ((*Kocka)[1][3] == (*Kocka)[5][4])
    feljegyez("y7+"), i = 1;
  else if ((*Kocka)[0][3] == (*Kocka)[5][4])
    feljegyez("y7-"), i = 1;
  else if ((*Kocka)[3][3] == (*Kocka)[5][4])
    feljegyez("y7+y7+"), i = 1;
  if (i)
  {
    if ((*Kocka)[2][4] != (*Kocka)[2][5] || (*Kocka)[5][4] != (*Kocka)[5][3])
      feljegyez("z1-");
    else if ((*Kocka)[1][4] != (*Kocka)[1][5] || (*Kocka)[5][4] != (*Kocka)[5][7])
      feljegyez("y4+z1-y4-");
    else if ((*Kocka)[0][4] != (*Kocka)[0][5] || (*Kocka)[5][4] != (*Kocka)[5][1])
      feljegyez("y4-z1-y4+");
    else if ((*Kocka)[3][4] != (*Kocka)[3][5] || (*Kocka)[5][4] != (*Kocka)[5][5])
      feljegyez("y4-y4-z1-y4-y4-");
  }
  return i;
}

int e_alulrol_kozepre()
{
  int i = 0;
  if ((*Kocka)[4][5] == (*Kocka)[5][4])
    i = 1;
  else if ((*Kocka)[4][3] == (*Kocka)[5][4])
    feljegyez("y7-y7-"), i = 1;
  else if ((*Kocka)[4][7] == (*Kocka)[5][4])
    feljegyez("y7+"), i = 1;
  else if ((*Kocka)[4][1] == (*Kocka)[5][4])
    feljegyez("y7-"), i = 1;
  if (i)
  {
    if ((*Kocka)[2][4] != (*Kocka)[2][5] || (*Kocka)[5][4] != (*Kocka)[5][3])
      feljegyez("z1-");
    else if ((*Kocka)[1][4] != (*Kocka)[1][5] || (*Kocka)[5][4] != (*Kocka)[5][7])
      feljegyez("y4+z1-y4-");
    else if ((*Kocka)[0][4] != (*Kocka)[0][5] || (*Kocka)[5][4] != (*Kocka)[5][1])
      feljegyez("y4-z1-y4+");
    else if ((*Kocka)[3][4] != (*Kocka)[3][5] || (*Kocka)[5][4] != (*Kocka)[5][5])
      feljegyez("y4-y4-z1-y4-y4-");
  }
  return i;
}

int e_fentrol_kozepre()
{
  int i = 0;
  if ((*Kocka)[5][3] == (*Kocka)[5][4] && (*Kocka)[2][5] != (*Kocka)[2][4])
    feljegyez("z1+"), i = 1;
  else if ((*Kocka)[5][1] == (*Kocka)[5][4] && (*Kocka)[0][5] != (*Kocka)[0][4])
    feljegyez("x1+"), i = 1;
  else if ((*Kocka)[5][7] == (*Kocka)[5][4] && (*Kocka)[1][5] != (*Kocka)[1][4])
    feljegyez("x4+"), i = 1;
  else if ((*Kocka)[5][5] == (*Kocka)[5][4] && (*Kocka)[3][5] != (*Kocka)[3][4])
    feljegyez("z4+"), i = 1;
  return i;
}

int e_lentrol_sarokba()
{
  int i = 0;
  if ((*Kocka)[1][0] == (*Kocka)[5][4])
  {
    feljegyez("y7+"), i = 1;
  }
  else if ((*Kocka)[1][6] == (*Kocka)[5][4])
  {
    feljegyez("y7+"), i = 6;
  }
  else if ((*Kocka)[0][0] == (*Kocka)[5][4])
  {
    feljegyez("y7-"), i = 1;
  }
  else if ((*Kocka)[0][6] == (*Kocka)[5][4])
  {
    feljegyez("y7-"), i = 6;
  }
  else if ((*Kocka)[2][0] == (*Kocka)[5][4])
  {
    i = 1;
  }
  else if ((*Kocka)[2][6] == (*Kocka)[5][4])
  {
    i = 6;
  }
  else if ((*Kocka)[3][0] == (*Kocka)[5][4])
  {
    feljegyez("y7+y7+"), i = 1;
  }
  else if ((*Kocka)[3][6] == (*Kocka)[5][4])
  {
    feljegyez("y7-y7-"), i = 6;
  }
  if (i == 1)
  {
    if ((*Kocka)[0][6] == (*Kocka)[2][4])
    {
      feljegyez("y1-x4+y1+x4-");
    }
    else if ((*Kocka)[0][6] == (*Kocka)[0][4])
    {
      feljegyez("y7-x4+y1+x4-y7+");
    }
    else if ((*Kocka)[0][6] == (*Kocka)[1][4])
    {
      feljegyez("y6+y1-x4+y1+x4-");
    }
    else if ((*Kocka)[0][6] == (*Kocka)[3][4])
    {
      feljegyez("y6+y6+y1-x4+y1+x4-");
    }
  }
  else if (i)
  {
    if ((*Kocka)[1][0] == (*Kocka)[2][4])
    {
      feljegyez("y1+x1+y1-x1-");
    }
    else if ((*Kocka)[1][0] == (*Kocka)[1][4])
    {
      feljegyez("y7+x1+y1-x1-");
    }
    else if ((*Kocka)[1][0] == (*Kocka)[0][4])
    {
      feljegyez("y6-y1+x1+y1-x1-");
    }
    else if ((*Kocka)[1][0] == (*Kocka)[3][4])
    {
      feljegyez("y6+y7+x1+y1-x1-");
    }
  }
  return i;
}

int e_alulrol_lentre()
{
  int i = 0;
  if ((*Kocka)[4][2] == (*Kocka)[5][4])
    i = 1;
  else if ((*Kocka)[4][8] == (*Kocka)[5][4])
    feljegyez("y7+"), i = 1;
  else if ((*Kocka)[4][0] == (*Kocka)[5][4])
    feljegyez("y7-"), i = 1;
  else if ((*Kocka)[4][6] == (*Kocka)[5][4])
    feljegyez("y7+y7+"), i = 1;
  if (i)
  {
    if ((*Kocka)[2][4] != (*Kocka)[2][2] || (*Kocka)[5][4] != (*Kocka)[5][0] || (*Kocka)[0][4] != (*Kocka)[0][8])
      feljegyez("x1+y1-y1-x1-");
    else if ((*Kocka)[2][4] != (*Kocka)[2][8] || (*Kocka)[5][4] != (*Kocka)[5][6] || (*Kocka)[1][4] != (*Kocka)[1][2])
      feljegyez("x4+y1-y1-x4-");
    else if ((*Kocka)[3][4] != (*Kocka)[3][2] || (*Kocka)[5][4] != (*Kocka)[5][8] || (*Kocka)[1][4] != (*Kocka)[1][8])
      feljegyez("y4-y4-x1+y1-y1-x1-y4+y4+");
    else if ((*Kocka)[3][4] != (*Kocka)[3][8] || (*Kocka)[5][4] != (*Kocka)[5][2] || (*Kocka)[0][4] != (*Kocka)[0][2])
      feljegyez("y4-x1+y1-y1-x1-y4+");
  }
  return i;
}

int e_sarokbol_lentre()
{
  int i = 0;
  if ((*Kocka)[2][2] == (*Kocka)[5][4])
    feljegyez("y7-x4+y1+x4-"), i = 1;
  else if ((*Kocka)[2][8] == (*Kocka)[5][4])
    feljegyez("y7+x1+y1-x1-"), i = 1;
  else if ((*Kocka)[1][2] == (*Kocka)[5][4])
    feljegyez("x4+y1+x4-"), i = 1;
  else if ((*Kocka)[1][8] == (*Kocka)[5][4])
    feljegyez("x4-y1-x4+"), i = 1;
  else if ((*Kocka)[0][2] == (*Kocka)[5][4])
    feljegyez("y4-y4-x4+y1+x4-y4+y4+"), i = 1;
  else if ((*Kocka)[0][8] == (*Kocka)[5][4])
    feljegyez("x1+y1-x1-"), i = 1;
  else if ((*Kocka)[3][2] == (*Kocka)[5][4])
    feljegyez("y7+x4+y1+x4-"), i = 1;
  else if ((*Kocka)[3][8] == (*Kocka)[5][4])
    feljegyez("y7-x1+y1-x1-"), i = 1;
  return i;
}

int e_fentrol_lentre()
{
  int i = 0;
  if ((*Kocka)[5][0] == (*Kocka)[5][4] && ((*Kocka)[2][2] != (*Kocka)[2][4] || (*Kocka)[0][8] != (*Kocka)[0][4]))
    feljegyez("x1+y1-x1-"), i = 1;
  else if ((*Kocka)[5][2] == (*Kocka)[5][4] && ((*Kocka)[0][2] != (*Kocka)[0][4] || (*Kocka)[3][8] != (*Kocka)[3][4]))
    feljegyez("y7-x1+y1-x1-"), i = 1;
  else if ((*Kocka)[5][6] == (*Kocka)[5][4] && ((*Kocka)[2][8] != (*Kocka)[2][4] || (*Kocka)[1][2] != (*Kocka)[1][4]))
    feljegyez("x4+y1+x4-"), i = 1;
  else if ((*Kocka)[5][8] == (*Kocka)[5][4] && ((*Kocka)[1][8] != (*Kocka)[1][4] || (*Kocka)[3][2] != (*Kocka)[3][4]))
    feljegyez("y7+x4+y1+x4-"), i = 1;
  return i;
}

int m_fentrol_kozepre()
{
  int i = 0;
  if ((*Kocka)[2][5] != (*Kocka)[5][4] && (*Kocka)[5][3] != (*Kocka)[5][4])
    i = 1;
  else if ((*Kocka)[0][5] != (*Kocka)[5][4] && (*Kocka)[5][1] != (*Kocka)[5][4])
    feljegyez("y7-"), i = 1;
  else if ((*Kocka)[1][5] != (*Kocka)[5][4] && (*Kocka)[5][7] != (*Kocka)[5][4])
    feljegyez("y7+"), i = 1;
  else if ((*Kocka)[3][5] != (*Kocka)[5][4] && (*Kocka)[5][5] != (*Kocka)[5][4])
    feljegyez("y7-y7-"), i = 1;
  if (i)
  {
    if ((*Kocka)[2][5] == (*Kocka)[0][4])
      feljegyez("y3-");
    else if ((*Kocka)[2][5] == (*Kocka)[1][4])
      feljegyez("y3+");
    else if ((*Kocka)[2][5] == (*Kocka)[3][4])
      feljegyez("y3+y3+");

    if ((*Kocka)[5][3] == (*Kocka)[0][4])
      feljegyez("x4+z1+z1+y4-z1-y4+z1-z1-x4-");
    else
      feljegyez("x1+z1+z1+y4+z1+y4-z1-z1-x1-");
  }
  return i;
}

int m_kozeprol_fentre()
{
  int i = 0;
  if ((*Kocka)[2][1] != (*Kocka)[2][4] || (*Kocka)[0][7] != (*Kocka)[0][4])
    feljegyez("x4+z1+z1+y4-z1-y4+z1-z1-x4-"), i = 1;
  else if ((*Kocka)[2][7] != (*Kocka)[2][4] || (*Kocka)[1][1] != (*Kocka)[1][4])
    feljegyez("x1+z1+z1+y4+z1+y4-z1-z1-x1-"), i = 1;
  else if ((*Kocka)[1][7] != (*Kocka)[1][4] || (*Kocka)[3][1] != (*Kocka)[3][4])
    feljegyez("y7+x1+z1+z1+y4+z1+y4-z1-z1-x1-"), i = 1;
  else if ((*Kocka)[0][1] != (*Kocka)[0][4] || (*Kocka)[3][7] != (*Kocka)[3][4])
    feljegyez("y7-x4+z1+z1+y4-z1-y4+z1-z1-x4-"), i = 1;
  return i;
}

int sarokkocka_helyen(int x)
{
  switch (x)
  {
  case 0:
    if ((*Kocka)[5][0] == (*Kocka)[5][4] && (*Kocka)[2][2] == (*Kocka)[2][4])
      return 1;
    if ((*Kocka)[2][2] == (*Kocka)[5][4] && (*Kocka)[5][0] == (*Kocka)[0][4])
      return 1;
    if ((*Kocka)[0][8] == (*Kocka)[5][4] && (*Kocka)[2][2] == (*Kocka)[0][4])
      return 1;
    break;
  case 2:
    if ((*Kocka)[5][2] == (*Kocka)[5][4] && (*Kocka)[0][2] == (*Kocka)[0][4])
      return 1;
    if ((*Kocka)[3][8] == (*Kocka)[5][4] && (*Kocka)[5][2] == (*Kocka)[0][4])
      return 1;
    if ((*Kocka)[0][2] == (*Kocka)[5][4] && (*Kocka)[3][8] == (*Kocka)[0][4])
      return 1;
    break;
  case 6:
    if ((*Kocka)[5][6] == (*Kocka)[5][4] && (*Kocka)[2][8] == (*Kocka)[2][4])
      return 1;
    if ((*Kocka)[2][8] == (*Kocka)[5][4] && (*Kocka)[5][6] == (*Kocka)[1][4])
      return 1;
    if ((*Kocka)[1][2] == (*Kocka)[5][4] && (*Kocka)[2][8] == (*Kocka)[1][4])
      return 1;
    break;
  case 8:
    if ((*Kocka)[5][8] == (*Kocka)[5][4] && (*Kocka)[1][8] == (*Kocka)[1][4])
      return 1;
    if ((*Kocka)[1][8] == (*Kocka)[5][4] && (*Kocka)[5][8] == (*Kocka)[3][4])
      return 1;
    if ((*Kocka)[3][2] == (*Kocka)[5][4] && (*Kocka)[1][8] == (*Kocka)[3][4])
      return 1;
    break;
  }
  return 0;
}

int sarokkockak_helyre()
{
  int i = 0;
  int s = 0;
  do
  {
    if (sarokkocka_helyen(0) && sarokkocka_helyen(2) && sarokkocka_helyen(6) && sarokkocka_helyen(8))
      i = 1;
    else if (sarokkocka_helyen(0) && sarokkocka_helyen(2))
      feljegyez("y7+y7+x4+y4+x1+y4-x4-y4+x1-y4-y4-"), i = 1;
    else if (sarokkocka_helyen(0) && sarokkocka_helyen(6))
      feljegyez("y7-x4+y4+x1+y4-x4-y4+x1-y4-y4-"), i = 1;
    else if (sarokkocka_helyen(6) && sarokkocka_helyen(8))
      feljegyez("x4+y4+x1+y4-x4-y4+x1-y4-y4-"), i = 1;
    else if (sarokkocka_helyen(8) && sarokkocka_helyen(2))
      feljegyez("y7+x4+y4+x1+y4-x4-y4+x1-y4-y4-"), i = 1;
    else if (sarokkocka_helyen(0) && sarokkocka_helyen(8))
      feljegyez("z1+x4-y4+x4+y4-z1-y4+"), i = 1;
    else if (sarokkocka_helyen(2) && sarokkocka_helyen(6))
      feljegyez("y7+z1+x4-y4+x4+y4-z1-y4+"), i = 1;
    else
      feljegyez("y4+"), s++;
    if (s == 4)
      i = 1;
  } while (!i);
  return 1;
}

int sarokkockak_szinre()
{
  int i = 0;
  if ((*Kocka)[2][8] == (*Kocka)[5][4])
    i = 1;
  else if ((*Kocka)[0][8] == (*Kocka)[5][4])
    feljegyez("y7-"), i = 1;
  else if ((*Kocka)[1][8] == (*Kocka)[5][4])
    feljegyez("y7+"), i = 1;
  else if ((*Kocka)[3][8] == (*Kocka)[5][4])
    feljegyez("y7+y7+"), i = 1;
  if (i) //*//
  {
    if ((*Kocka)[3][2] == (*Kocka)[5][4])
      feljegyez("x4+y1-x4-y7+x1+y1-x1-y4+x1+y1+x1-y7-x4+y1+x4-y4-");
    else if ((*Kocka)[2][2] == (*Kocka)[5][4])
      feljegyez("x4+y1-x4-y7+x1+y1-x1-y4-x1+y1+x1-y7-x4+y1+x4-y4+");
    else if ((*Kocka)[0][2] == (*Kocka)[5][4])
      feljegyez("x4+y1-x4-y7+x1+y1-x1-y4+y4+x1+y1+x1-y7-x4+y1+x4-y4-y4-");

    else if ((*Kocka)[0][8] == (*Kocka)[5][4] && (*Kocka)[3][8] != (*Kocka)[5][4])
      feljegyez("y7-x4+y1-x4-y7+x1+y1-x1-y4+x1+y1+x1-y7-x4+y1+x4-y4-");
    else if ((*Kocka)[0][8] == (*Kocka)[5][4] && (*Kocka)[3][8] == (*Kocka)[5][4])
      feljegyez("y7-y7-x4+y1-x4-y7+x1+y1-x1-y4+x1+y1+x1-y7-x4+y1+x4-y4-");
    else if ((*Kocka)[1][8] == (*Kocka)[5][4])
      feljegyez("x4+y1-x4-y7+x1+y1-x1-y4+x1+y1+x1-y7-x4+y1+x4-y4-");
  }
  else
  {
    if ((*Kocka)[2][2] == (*Kocka)[5][4])
    {
      if ((*Kocka)[5][6] == (*Kocka)[5][4])
        feljegyez("y7+y7+");
      else if ((*Kocka)[5][8] == (*Kocka)[5][4])
        feljegyez("y7-");
      i = 1;
    }
    else if ((*Kocka)[1][2] == (*Kocka)[5][4])
      feljegyez("y7+"), i = 1;
    if (i)
      feljegyez("x4+y1-x4-y7+x1+y1-x1-y4-x1+y1+x1-y7-x4+y1+x4-y4+");
  }
  return i;
}

int elkocka_helyen(int x)
{
  switch (x)
  {
  case 1:
    if ((*Kocka)[5][1] == (*Kocka)[5][4] && (*Kocka)[0][5] == (*Kocka)[0][4])
      return 1;
    if ((*Kocka)[5][1] == (*Kocka)[0][4] && (*Kocka)[0][5] == (*Kocka)[5][4])
      return 1;
    break;
  case 3:
    if ((*Kocka)[5][3] == (*Kocka)[5][4] && (*Kocka)[2][5] == (*Kocka)[2][4])
      return 1;
    if ((*Kocka)[5][3] == (*Kocka)[2][4] && (*Kocka)[2][5] == (*Kocka)[5][4])
      return 1;
    break;
  case 5:
    if ((*Kocka)[5][5] == (*Kocka)[5][4] && (*Kocka)[3][5] == (*Kocka)[3][4])
      return 1;
    if ((*Kocka)[5][5] == (*Kocka)[3][4] && (*Kocka)[3][5] == (*Kocka)[5][4])
      return 1;
    break;
  case 7:
    if ((*Kocka)[5][7] == (*Kocka)[5][4] && (*Kocka)[1][5] == (*Kocka)[1][4])
      return 1;
    if ((*Kocka)[5][7] == (*Kocka)[1][4] && (*Kocka)[1][5] == (*Kocka)[5][4])
      return 1;
    break;
  }
  return 0;
}

int elkockak_helyre()
{
  int i = 0;
  if (elkocka_helyen(1) && elkocka_helyen(3) && elkocka_helyen(5) && elkocka_helyen(7))
    return 0;
  if (elkocka_helyen(3))
    feljegyez("y7-");
  else if (elkocka_helyen(1))
    feljegyez("y7-y7-");
  else if (elkocka_helyen(5))
    feljegyez("y7+");
  if (((*Kocka)[5][3] == (*Kocka)[5][4] && (*Kocka)[2][5] == (*Kocka)[3][4]) || ((*Kocka)[5][3] == (*Kocka)[3][4] && (*Kocka)[2][5] == (*Kocka)[5][4]))
    feljegyez("x1-x1-y4-z3+z1-y4-y4-z3-z1+y4-x1+x1+"), i = 1;
  else
    feljegyez("x1-x1-y4+z3+z1-y4+y4+z3-z1+y4+x1+x1+"), i = 1;
  return i;
}

int elkockak_szinre()
{
  int i = 0;
  if ((*Kocka)[2][5] == (*Kocka)[5][4])
    i = 1;
  else if ((*Kocka)[0][5] == (*Kocka)[5][4])
    feljegyez("y7-"), i = 1;
  else if ((*Kocka)[1][5] == (*Kocka)[5][4])
    feljegyez("y7+"), i = 1;
  else if ((*Kocka)[3][5] == (*Kocka)[5][4])
    feljegyez("y7+y7+"), i = 1;
  if (i)
  {
    if ((*Kocka)[0][5] == (*Kocka)[5][4])
      feljegyez("y4+x1-y4+y7-y7-x4-y7+x4-y7+x4-y7+x4-y7+y4-y7+y7+x1+y4-x1-y7-x1-y7-x1-y7-x1-");
    else if ((*Kocka)[1][5] == (*Kocka)[5][4])
      feljegyez("y7+y4+x1-y4+y7-y7-x4-y7+x4-y7+x4-y7+x4-y7+y4-y7+y7+x1+y4-x1-y7-x1-y7-x1-y7-x1-");
    else
      feljegyez("x1+x4+z1-y4+x1-z1+y4-x1-x4-z4-y4-x1+z4+y4+");
  }
  return i;
}

void kirak_teteje()
{
  int szin = (*Kocka)[5][4];
  int s;
  int i, k;
  int oldal[8] = {-1, 0, -1, 2, -1, 3, -1, 1};
  //Van-e a tetej�n j� kocka
  for (i = 1; i <= 7; i += 2)
    if ((*Kocka)[5][i] == szin)
      s = i, i = 8;
  //Ha van szembeforgatjuk
  switch (s)
  {
  case 1:
    feljegyez("y7-");
    break;
  case 5:
    feljegyez("y7-y7-");
    break;
  case 7:
    feljegyez("y7+");
    break;
  }
  // al�ford�tjuk a k�z�ps� sort
  if ((*Kocka)[2][5] == (*Kocka)[0][4])
    feljegyez("y3-");
  else if ((*Kocka)[2][5] == (*Kocka)[1][4])
    feljegyez("y3+");
  else if ((*Kocka)[2][5] == (*Kocka)[3][4])
    feljegyez("y3-y3-");
  // �lkock�k helyrerak�sa
  i = 0;
  do
  {
    while (e_kozeprol_keresztbe() && i < 10)
      i++;
    i++;
  } while ((e_keresztbol_kozepre() || e_lentrol_kozepre() || e_alulrol_kozepre() || e_fentrol_kozepre()) && i < 10);
  //Sarokkock�k helyrerak�sa
  i = 0;
  do
  {
    while (e_lentrol_sarokba() && i < 10)
      i++;
    i++;
  } while ((e_alulrol_lentre() || e_sarokbol_lentre() || e_fentrol_lentre()) && i < 10);
}

void kirak_kozepe()
{
  int i = 0;
  do
  {
    while (m_fentrol_kozepre() && i < 10)
      i++;
    i++;
  } while (m_kozeprol_fentre() && i < 10);
}

void kirak_alja()
{ //Sarokkock�k helyreford�t�sa
  sarokkockak_helyre();
  //Sarokkockak sz�nreford�t�sa
  int i = 0;
  while (sarokkockak_szinre() && i < 2)
    i++;
  //�lkock�k helyreforgat�sa
  i = 0;
  while (elkockak_helyre() && i < 2)
    i++;
  //�lkock�k sz�nreforgat�sa
  i = 0;
  while (elkockak_szinre() && i < 2)
    i++;
}

char kirak_kocka(int (*x)[6][9])
{
  Kocka = x;
  char a;
  hossz = 0;
  memcpy(oldkocka[0], Kocka, sizeof(oldkocka[0]));

  if (!kirakva_teteje())
    kirak_teteje();
  if (kirakva_teteje())
  {
    memcpy(Kocka, oldkocka[0], sizeof(oldkocka[0]));
    if (gyorsforgatas(optimalizalas_foka))
      return 1; // gyorsforgatas parameter hatarozza meg, hogy ki legyen-e optimaliz�lva a forgatas sorozat. (True / False)
  }
  if (!kirakva_kocka() && kirakva_teteje())
  {
    forgat(0, 7, 1, 1, 0);
    forgat(0, 7, 1, 1, 0);
    if (sajat_kocka_kirakasa)
      kocka_rajz(), getch();
    hossz = 0;
    memcpy(oldkocka[1], Kocka, sizeof(oldkocka[1]));
    if (!kirakva_kozepe())
      kirak_kozepe(); //ezek mind while-ban vannak (a felt�tel ugyanez)
    if (!kirakva_kocka() && kirakva_kozepe())
      kirak_alja();
    if (kirakva_kocka())
    {
      memcpy(Kocka, oldkocka[1], sizeof(oldkocka[1]));
      if (gyorsforgatas(optimalizalas_foka))
        return 1;
    }
    else
      memcpy(Kocka, oldkocka[1], sizeof(oldkocka[1]));
  }
  //  else memcpy(Kocka,oldkocka[0],sizeof(oldkocka[0]));
  return 0;
}
