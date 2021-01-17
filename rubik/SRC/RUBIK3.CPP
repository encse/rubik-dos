#include <fstream.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "src\graphics.h"
#include "src\grmouse.h"
#include "src\kirak.h"
#include "src\backtrac.h"
#define MERET 41
#define SELECT 127
int speed = 20;
//#define debug

//A visszal�p�ses keres�s maxim�lis m�lys�ge
char MAXMELYSEG = 6;
#define KEVERESHOSSZ 6
int LIGHTX = 0;
int LIGHTY = 100;
int LIGHTZ = -400;

struct koordinata
{
  int x;
  int y;
  int z;
} koord[32], boxkoord[4], kocka_helye[2], kocka_sarkai[2][8], lap_kozepe[2][8], perspekt_koord[2][8];

struct floatkoord
{
  float x;
  float y;
  float z;
};

struct floatkoord2d
{
  float x;
  float y;
};

struct koord2d
{
  int x;
  int y;
};

int lathato_lapok[2][6];
int box[6][4] = {{3, 7, 0, 4}, {1, 5, 2, 6}, {0, 4, 1, 5}, {2, 6, 3, 7}, {3, 0, 2, 1}, {4, 7, 5, 6}};
int kocka[2][6][9] = {{{1, 1, 1, 1, 1, 1, 1, 1, 1},           //k�k        0
                       {2, 2, 2, 2, 2, 2, 2, 2, 2},           //z�ld       1
                       {4, 4, 4, 4, 4, 4, 4, 4, 4},           //piros      2
                       {9, 9, 9, 9, 9, 9, 9, 9, 9},           //vk�k       3
                       {15, 15, 15, 15, 15, 15, 15, 15, 15},  //feh�r 4
                       {14, 14, 14, 14, 14, 14, 14, 14, 14}}, //s�rga 5

                      {{15, 15, 15, 15, 15, 15, 15, 15, 15}, //feh�r 0
                       {14, 14, 14, 14, 14, 14, 14, 14, 14}, //s�rga 1
                       {1, 1, 1, 1, 1, 1, 1, 1, 1},          //k�k        2
                       {2, 2, 2, 2, 2, 2, 2, 2, 2},          //z�ld       3
                       {4, 4, 4, 4, 4, 4, 4, 4, 4},          //piros      4
                       {9, 9, 9, 9, 9, 9, 9, 9, 9}}};        //vk�k       5

float egesz_kocka_elforditasa = 18; //ennyi fokkal (rad) lesz az eg�sz kocka elforgatva
int oldalak[6][9];
int szinek[6] = {WHITE, GREEN, BLUE, RED, YELLOW, CYAN};
int maxy, maxx;
float vektor[6][3];
float Sin[900];
extern int optimalizalas_foka = 1;
int sajat_kocka_kirakasa = 0;
int aktiv_kocka = 0;    //Ha a forgat f�ggv�ny elmozdul param�ter�nek nem 0 �rt�ket adunk meg, akkor ez a kocka fog elmozdulni
int elhelyezes_mod = 0; // Igaz, ha a vezerlest az atad fuggveny vegzi
int light = 0;

int sign(float);
void set(koordinata *, int, int, int);
floatkoord sum(koordinata, floatkoord);
float skal(floatkoord, koordinata);
float hossz(koordinata);
float hossz(floatkoord);
float hossz(floatkoord2d);
int line3d(int, int, int, int, int, int, struct scankoord *);
int line2d(int, int, int, int, scankoord *);
void box3d(int, int, int, int);
void forgat_rajz(int, int, int, int, int, int, int);
void forgat_atszinez(int, int, int);
extern void forgat(int, int, int, int, int); //tengely,reteg�,fok,rajzol,elmozdulas merteke
extern char kirak_kocka(int (*Kocka)[6][9]);
extern int atad();
extern void Abort(int, char *);
void kocka_allapot(int); // a megadott kocka �llapot�nak (l�that� oldalak, sarkok helye...) kisz�m�t�sa
void hol_all_az_eger(int &Kocka, int &lap, int &negyzet, int &szin, int mx, int my);
int kocka_beolvasas(void);

void kocka_rajz();
int ment();
int mentkocka();
char kockaAdatFajl[20];
int betoltkocka();
int kever();
unsigned randominit;

void main(int argv, char *argc[])
{
  int i, k;
  unsigned char a;
  char filename[13];
  int kulso_vezerles = 0;
  randominit = (unsigned)time(NULL);
  strcpy(kockaAdatFajl, "rubik.dat");
  if (argv)
    for (i = 1; i < argv; i++)
    {
      if (!strcmpi(argc[i], "-ol"))
        optimalizalas_foka = 0;
      if (!strcmpi(argc[i], "-mi"))
        MAXMELYSEG = atoi(argc[i + 1]);
      if (!strcmpi(argc[i], "-lon"))
        light = 1;
      if (!strcmpi(argc[i], "-h") || !strcmpi(argc[i], "-?"))
      {
        printf("\n\r  -ol:   nincs optimaliz�l�s");
        printf("\n\r  -mi szint:  a visszal�p�ses keres�s m�lys�ge\n\r");
        printf("\n\r  -lon:  megvil�g�t�s bekapcsol�sa\n\r");
        printf("\n\r  -play filename:  mintaf�jl lej�tsz�sa\n\r");
        printf("\n\r  -load filename:  el�z�leg elmentett f�jl bet�lt�se\n\r");
        printf("\n\r  -my:  Rakjuk ki a Rubik-kock�d\n\r");
        return;
      }
      if (!strcmpi(argc[i], "-play"))
        strcpy(filename, argc[i + 1]), kulso_vezerles = 1;
      if (!strcmpi(argc[i], "-load"))
      {
        strcpy(kockaAdatFajl, argc[i + 1]);
        betoltkocka();
      }
      if (!strcmpi(argc[i], "-my"))
        sajat_kocka_kirakasa = 1;
      if (!strcmpi(argc[i], "-debug"))
      {
        FILE *fp;
        fp = fopen("error.dat", "rb");
        char id[6] = {0, 0, 0, 0, 0, 0};
        fread(id, 5, 1, fp);
        if (!strcmp(id, "RUBIK"))
          fread(kocka[0], 6 * 9 * sizeof(int), 1, fp);
        fclose(fp);
      }
    }
  i = setgraphmode();
  if (i == 1)
    Abort(1, "Hiba 1:\n\rNincs el�g mem�ria a k�perny� kezel�s�hez.");
  if (i == 2)
    Abort(2, "Hiba 2:\n\rVESA inicializ�l�s sikertelen.");

  paletteinit();
  for (i = -450; i < 450; i++)
    Sin[i + 450] = sin(i * 3.141592654 / 180);
  for (i = 0; i < 6; i++)
    vektor[i][0] = vektor[i][1] = vektor[i][2] = 0;
  vektor[0][0] = vektor[4][1] = vektor[2][2] = -1;
  vektor[1][0] = vektor[5][1] = vektor[3][2] = 1;
  for (i = 0; i < 6; i++)
    for (k = 0; k < 9; k++)
      oldalak[i][k] = szinek[i];
  setbkcolor(BLACK);
  setcolor(BLACK);
  kocka_helye[0].x = -150;
  kocka_helye[0].y = -250;
  kocka_helye[0].z = 70;
  kocka_helye[1].x = 150;
  kocka_helye[1].y = -250;
  kocka_helye[1].z = 70;

  //sebess�g teszt
  // clock_t start=clock();
  // i=0;
  // rajzol�si sebess�g
  // while((clock()-start)/CLK_TCK<=1) test_screen_draw_speed(3),i++;
  // sz�mol�si sebess�g
  /*  start=clock();
  for(k=0;k<4;k++) teszt_szamolasi_sebesseg();
  clock_t stop=clock();
  ofstream out("out.txt");
  out << stop-start;          */
  //  speed=((float)1/i)/.0149*9;
  //  speed=(stop-start)*speed/21;

  //  if(speed<1) speed=1;
  //speed = 1;
  //  speed=7;

  srand(randominit);
  kocka_rajz();
  mouseinit();
  if (sajat_kocka_kirakasa)
    a = kocka_beolvasas();
  if (a == 27)
  {
    closegraph();
    return;
  }

  if (kulso_vezerles)
  {
    ifstream in(filename);
    char buf[1024];
    int ki = 0;
    in >> buf;
    if (!in.fail())
      while (ki != 2 && a != 27)
      {
        a = gyorsforgatas(buf);
        if (!ki)
          in >> buf;
        if (ki)
          ki++;
        if (in.eof() && !ki)
          ki = 1;
      }
  }

  if (!atad())
    do
    {
      mouse_status(BUSY); //homok�ra
      i = 0;
      for (k = 0; k < MAXMELYSEG && !i; k++)
      {
        i = backTrack(&kocka[0], k);
      }
      mouse_status(ARROW); //ny�l
      if (!i)
        i = kirak_kocka(&kocka[0]);
      i = atad();
    } while (!i);
  closegraph();
}

void Abort(int errno, char *s)
{
  closegraph();
  printf(s);
  if (errno == 10)
    printf(" %d", randominit);
  exit(1);
}

int atad()
{
  int ki = 0;
  //  elhelyezes_mod=1;
  kocka_rajz();
  kocka_allapot(0);
  kocka_allapot(1);
  int gomb_benyomva = 0;
  int Kocka[3] = {-1, -1, -1};
  int lap[3];
  int negyzet[3];
  int szin[3];
  while (!ki)
  {
    word mx, my, mb;
    do
    {
      mousepos(&mx, &my, &mb);
      if (!(mb & 1) && gomb_benyomva)
      {
        hol_all_az_eger(Kocka[1], lap[1], negyzet[1], szin[1], mx, my);
        kocka[Kocka[0]][lap[0]][negyzet[0]] = szin[0], kocka_rajz();
        if (Kocka[2] != -1)
          kocka[Kocka[2]][lap[2]][negyzet[2]] = szin[2], kocka_rajz(), Kocka[2] = -1;
        if (Kocka[1] == Kocka[0] && lap[1] != lap[0])
        {
          int ki = 0;
          {
            int par[2][3][4][2] = {{{{5, 2}, {2, 4}, {4, 3}, {3, 5}},
                                    {{1, 4}, {4, 0}, {0, 5}, {5, 1}},
                                    {{1, 2}, {3, 1}, {0, 3}, {2, 0}}},
                                   {{{1, 4}, {4, 0}, {0, 5}, {5, 1}},
                                    {{2, 1}, {1, 3}, {3, 0}, {0, 2}},
                                    {{2, 5}, {4, 2}, {3, 4}, {5, 3}}}};

            for (int tengely = 0; tengely < 3 && !ki; tengely++)
            {
              for (int i = 0; i < 4 && !ki; i++)
              {
                if (lap[0] == par[Kocka[0]][tengely][i][0] && lap[1] == par[Kocka[0]][tengely][i][1])
                  forgat(tengely, 7, 1, 1, 0), kocka_rajz(), ki = 1;
                if (lap[1] == par[Kocka[0]][tengely][i][0] && lap[0] == par[Kocka[0]][tengely][i][1])
                  forgat(tengely, 7, -1, 1, 0), kocka_rajz(), ki = 1;
              }
            }
          }
        }
        if (Kocka[1] == Kocka[0] && lap[1] == lap[0])
        {
          int ki = 0;
          {
            int par[2][3][4][2] = {{{{2, -1}, {4, -1}, {3, 1}, {5, -1}},
                                    {{0, 1}, {5, 3}, {1, -1}, {4, -3}},
                                    {{0, -3}, {1, -3}, {2, -3}, {3, -3}}},
                                   {{{0, 3}, {4, -1}, {3, 1}, {5, -1}},
                                    {{0, -3}, {1, -3}, {2, -3}, {3, -3}},
                                    {{0, 1}, {5, 3}, {1, -1}, {4, -3}}}};

            for (int tengely = 0; tengely < 3 && !ki; tengely++)
            {
              for (int i = 0; i < 4 && !ki; i++)
              {
                if (!Kocka[0] && par[0][tengely][i][0] == lap[0])
                {
                  if (negyzet[0] + par[0][tengely][i][1] == negyzet[1] || (negyzet[0] + 2 * par[0][tengely][i][1] == negyzet[1] && negyzet[0] != 4 && !negyzet[0] % 2))
                    switch (tengely)
                    {
                    case 0:
                      if (lap[0] == 2 || lap[0] == 5)
                        forgat(tengely, 1 << (negyzet[0] / 3), 1, 1, 0), kocka_rajz(), ki = 1;
                      if (lap[0] == 0)
                        forgat(tengely, 1 << (2 - negyzet[0] / 3), 1, 1, 0), kocka_rajz(), ki = 1;
                      if (lap[0] == 4)
                        forgat(tengely, 1 << (2 - negyzet[0] % 3), 1, 1, 0), kocka_rajz(), ki = 1;
                      break;
                    case 1:
                      if (lap[0] == 3)
                        forgat(tengely, 1 << negyzet[0] / 3, -1, 1, 0), kocka_rajz(), ki = 1;
                      else if (lap[0] == 5)
                        forgat(tengely, 1 << (negyzet[0] % 3), 1, 1, 0), kocka_rajz(), ki = 1;
                      else
                        forgat(tengely, 1 << (2 - negyzet[0] / 3), 1, 1, 0), kocka_rajz(), ki = 1;
                      break;
                    case 2:
                      forgat(tengely, 1 << negyzet[0] % 3, 1, 1, 0), kocka_rajz(), ki = 1;
                      break;
                    }
                  if (negyzet[0] - par[0][tengely][i][1] == negyzet[1] || (negyzet[0] - 2 * par[0][tengely][i][1] == negyzet[1] && negyzet[0] != 4 && !negyzet[0] % 2))
                    switch (tengely)
                    {
                    case 0:
                      if (lap[0] == 2 || lap[0] == 5)
                        forgat(tengely, 1 << (negyzet[0] / 3), -1, 1, 0), kocka_rajz(), ki = 1;
                      if (lap[0] == 0)
                        forgat(tengely, 1 << (2 - negyzet[0] / 3), -1, 1, 0), kocka_rajz(), ki = 1;
                      if (lap[0] == 4)
                        forgat(tengely, 1 << (2 - negyzet[0] % 3), -1, 1, 0), kocka_rajz(), ki = 1;
                      break;
                    case 1:
                      if (lap[0] == 3)
                        forgat(tengely, 1 << negyzet[0] / 3, 1, 1, 0), kocka_rajz(), ki = 1;
                      else if (lap[0] == 5)
                        forgat(tengely, 1 << (negyzet[0] % 3), -1, 1, 0), kocka_rajz(), ki = 1;
                      else
                        forgat(tengely, 1 << (2 - negyzet[0] / 3), -1, 1, 0), kocka_rajz(), ki = 1;
                      break;
                    case 2:
                      forgat(tengely, 1 << negyzet[0] % 3, -1, 1, 0), kocka_rajz(), ki = 1;
                      break;
                    }
                }
                if (Kocka[0] && par[1][tengely][i][0] == lap[0])
                {
                  if (negyzet[0] + par[1][tengely][i][1] == negyzet[1] || (negyzet[0] + 2 * par[1][tengely][i][1] == negyzet[1] && negyzet[0] != 4 && !negyzet[0] % 2))
                    switch (tengely)
                    {
                    case 0:
                      if (lap[0] == 5 || lap[0] == 1 || lap[0] == 4)
                        forgat(2, 1 << negyzet[0] / 3, -1, 1, 0), kocka_rajz(), ki = 1;
                      if (lap[0] == 3)
                        forgat(2, 1 << (2 - negyzet[0] / 3), -1, 1, 0), kocka_rajz(), ki = 1;
                      break;
                    case 1:
                      forgat(1, 1 << negyzet[0] % 3, -1, 1, 0), kocka_rajz(), ki = 1;
                      break;
                    case 2:
                      if (lap[0] == 0)
                        forgat(0, 1 << (2 - negyzet[0] / 3), 1, 1, 0), kocka_rajz(), ki = 1;
                      else
                        forgat(0, 1 << negyzet[0] % 3, 1, 1, 0), kocka_rajz(), ki = 1;
                      break;
                    }
                  if (negyzet[0] - par[1][tengely][i][1] == negyzet[1] || (negyzet[0] - 2 * par[1][tengely][i][1] == negyzet[1] && negyzet[0] != 4 && !negyzet[0] % 2))
                    switch (tengely)
                    {
                    case 0:
                      if (lap[0] == 5 || lap[0] == 1 || lap[0] == 4)
                        forgat(2, 1 << negyzet[0] / 3, 1, 1, 0), kocka_rajz(), ki = 1;
                      if (lap[0] == 3)
                        forgat(2, 1 << (2 - negyzet[0] / 3), 1, 1, 0), kocka_rajz(), ki = 1;
                      break;
                    case 1:
                      forgat(1, 1 << negyzet[0] % 3, 1, 1, 0), kocka_rajz(), ki = 1;
                      break;
                    case 2:
                      if (lap[0] == 0)
                        forgat(0, 1 << (2 - negyzet[0] / 3), -1, 1, 0), kocka_rajz(), ki = 1;
                      else
                        forgat(0, 1 << negyzet[0] % 3, -1, 1, 0), kocka_rajz(), ki = 1;
                      break;
                    }
                }
              }
            }
          }
        }
        Kocka[0] = -1;
        gomb_benyomva = 0;
      }
      else if (mb & 1 && !gomb_benyomva)
      {
        hol_all_az_eger(Kocka[0], lap[0], negyzet[0], szin[0], mx, my);
        if (Kocka[0] != -1)
        {
          if (szin[0] % 16 == 15)
            kocka[Kocka[0]][lap[0]][negyzet[0]] = SELECT;
          else
            kocka[Kocka[0]][lap[0]][negyzet[0]] = 127;
          kocka_rajz();
          gomb_benyomva = 1;
        }
      }
      else if (mb & 1 && gomb_benyomva)
      {
        int sk, sl, sn, ssz;
        hol_all_az_eger(sk, sl, sn, ssz, mx, my);
        //itt
        if ((sk != Kocka[2] || sl != lap[2] || sn != negyzet[2]) && (sk != Kocka[0] || sl != lap[0] || sn != negyzet[0]))
        {
          if (Kocka[2] != -1)
            kocka[Kocka[2]][lap[2]][negyzet[2]] = szin[2], kocka_rajz();
          Kocka[2] = sk, negyzet[2] = sn, lap[2] = sl, szin[2] = ssz;
          if (Kocka[2] != -1)
            if (szin[2] % 16 == 15)
              kocka[Kocka[2]][lap[2]][negyzet[2]] = SELECT;
            else
              kocka[Kocka[2]][lap[2]][negyzet[2]] = 127;
          kocka_rajz();
        }
      }
    } while (!kbhit());
    if (!gomb_benyomva)
    {
      switch (getch())
      {
      case 9:
        aktiv_kocka = 1 - aktiv_kocka;
        kocka_rajz();
        kocka_allapot(aktiv_kocka);
        break;
      case 72:
        forgat(1, 7, 1, 1, 0);
        kocka_rajz();
        break;
      case 80:
        forgat(1, 7, -1, 1, 0);
        kocka_rajz();
        break;
      case 75:
        forgat(2, 7, 1, 1, 0);
        kocka_rajz();
        break;
      case 77:
        forgat(2, 7, -1, 1, 0);
        kocka_rajz();
        break;
      case 115:
        forgat(0, 7, -1, 1, 0);
        kocka_rajz();
        break;
      case 116:
        forgat(0, 7, 1, 1, 0);
        kocka_rajz();
        break;
      case '+':
        if (kocka_helye[aktiv_kocka].z > -100)
        {
          forgat(2 - aktiv_kocka, 7, -1, 1, -100);
          kocka_rajz();
          kocka_allapot(aktiv_kocka);
        }
        break;
      case '-':
        if (kocka_helye[aktiv_kocka].z < 400)
        {
          forgat(2 - aktiv_kocka, 7, 1, 1, -100); //az elmozgat�s ir�ny�t a forgat�s ir�ny�nak el�jele is szab�lyozza
          kocka_rajz();
          kocka_allapot(aktiv_kocka);
        }
        break;
      case 'p':
        ment();
        break;
      case 'm':
        mentkocka();
        break;
      case 'l':
        betoltkocka();
        break;
      case 'k':
        if (kever() == 27)
          ki = 2;
        break;
      case 27:
        ki = 2;
        break;
      case 13:
        ki = 1;
      }
    }
  }
  //  elhelyezes_mod=0;
  return ki - 1;
}

int kever()
{
  int oldk = -1, k = -1;
  for (int i = 0; i < KEVERESHOSSZ; i++)
  {
    int j = random(2);
    (!j) ? (j--) : 0;
    while (oldk == k)
      k = random(3);
    oldk = k;
    int elek = random(7) + 1;
    while (elek == 7)
      elek = random(7) + 1;
    forgat(k, elek, j, 1, 0);
    if (kbhit())
    {
      byte a = getch();
      if (a == 27)
      {
        kocka_rajz();
        return 27;
      }
      if (a == 'p')
        ment();
      if (a == 'm')
        mentkocka();
      if (a == 'l')
        betoltkocka();
    }
  }
  kocka_rajz();
  return 0;
}

void kocka_rajz()
{
  forgat_rajz(0, 2, 0, 0, kocka_helye[0].x, kocka_helye[0].y, kocka_helye[0].z);
  forgat_rajz(0, 2, 1, 0, kocka_helye[0].x, kocka_helye[0].y, kocka_helye[0].z);
  forgat_rajz(0, 2, 2, 0, kocka_helye[0].x, kocka_helye[0].y, kocka_helye[0].z);
  forgat_rajz(1, 2, 0, 0, kocka_helye[1].x, kocka_helye[1].y, kocka_helye[1].z);
  forgat_rajz(1, 2, 1, 0, kocka_helye[1].x, kocka_helye[1].y, kocka_helye[1].z);
  forgat_rajz(1, 2, 2, 0, kocka_helye[1].x, kocka_helye[1].y, kocka_helye[1].z);
  drawscreen();
  if (kbhit())
  {
    int a;
    a = getch();
    if (a == 'p')
      ment();
    else if (a == ('m'))
      mentkocka();
    else if (a == ('l'))
      betoltkocka();

    else
      ungetch(a);
  }
  //  cleardevice();
}

void forgat_rajz(int melyikkocka, int tengely, int reteg, int fok, int x, int y, int z)
{
  int mod;
  int i, k, j;
  int xvis, yvis, zvis;
  float xs, ys, zs;
  // float szin,r;
  //	0	1	2	3	4	5
  int lapok[3][3][6][3] = {{{{0, 0, 0}, {0, 0, 0}, {0, 1, 2}, {6, 7, 8}, {0, 1, 2}, {0, 1, 2}},
                            {{0, 0, 0}, {0, 0, 0}, {3, 4, 5}, {3, 4, 5}, {3, 4, 5}, {3, 4, 5}},
                            {{0, 0, 0}, {0, 0, 0}, {6, 7, 8}, {0, 1, 2}, {6, 7, 8}, {6, 7, 8}}},
                           {{{6, 7, 8}, {0, 1, 2}, {0, 0, 0}, {0, 0, 0}, {2, 5, 8}, {0, 3, 6}},
                            {{3, 4, 5}, {3, 4, 5}, {0, 0, 0}, {0, 0, 0}, {1, 4, 7}, {1, 4, 7}},
                            {{0, 1, 2}, {6, 7, 8}, {0, 0, 0}, {0, 0, 0}, {0, 3, 6}, {2, 5, 8}}},
                           {{{0, 3, 6}, {0, 3, 6}, {0, 3, 6}, {0, 3, 6}, {0, 0, 0}, {0, 0, 0}},
                            {{1, 4, 7}, {1, 4, 7}, {1, 4, 7}, {1, 4, 7}, {0, 0, 0}, {0, 0, 0}},
                            {{2, 5, 8}, {2, 5, 8}, {2, 5, 8}, {2, 5, 8}, {0, 0, 0}, {0, 0, 0}}}};
  switch (reteg)
  {
  case 0:
    mod = -MERET;
    break;
  case 1:
    mod = 0;
    break;
  case 2:
    mod = MERET;
    break;
  }
  float iranyx, iranyz;
  //  float iranyy;
  if (melyikkocka)
  {
    iranyx = Sin[450 - egesz_kocka_elforditasa];
    //    iranyx=sin(-0.279); // -16*pi/180
    //    iranyy=0;
    iranyz = Sin[540 - egesz_kocka_elforditasa];
    //  iranyz=cos(-0.279);
  }
  else
  {
    iranyx = Sin[450 + egesz_kocka_elforditasa];
    //iranyx=sin(0.279); // 16*pi/180
    //    iranyy=0;
    iranyz = Sin[540 + egesz_kocka_elforditasa];
    //  iranyz=cos(0.279);
  }

  float S[3] = {0, 0, 0};
  switch (tengely)
  {
    case 0:
      S[0] = mod;
      ys = zs = MERET * 1.5, xs = .5 * MERET;
      vektor[3][1] = vektor[4][2] = Sin[450 + fok % 360];
      vektor[3][2] = vektor[5][1] = Sin[540 + fok % 360];
      vektor[2][1] = vektor[5][2] = -vektor[3][1];
      vektor[2][2] = vektor[4][1] = -vektor[3][2];
      break;
    case 1:
      S[2] = mod;
      xs = ys = MERET * 1.5, zs = .5 * MERET;
      vektor[1][0] = vektor[5][1] = Sin[540 + fok % 360];
      vektor[0][1] = vektor[5][0] = Sin[450 + fok % 360];
      vektor[0][0] = vektor[4][1] = -vektor[1][0];
      vektor[1][1] = vektor[4][0] = -vektor[0][1];
      break;
    case 2:
      S[1] = mod;
      xs = zs = MERET * 1.5, ys = .5 * MERET;
      vektor[1][0] = vektor[3][2] = Sin[540 + fok % 360];
      vektor[0][2] = vektor[3][0] = Sin[450 + fok % 360];
      vektor[0][0] = vektor[2][2] = -vektor[1][0];
      vektor[1][2] = vektor[2][0] = -vektor[0][2];
  }
  float s;
  s = iranyx * vektor[0][0] - iranyz * vektor[0][2];
  vektor[0][2] = iranyx * vektor[0][2] + iranyz * vektor[0][0];
  vektor[0][0] = s;
  s = iranyx * vektor[1][0] - iranyz * vektor[1][2];
  vektor[1][2] = iranyx * vektor[1][2] + iranyz * vektor[1][0];
  vektor[1][0] = s;
  s = iranyx * vektor[2][0] - iranyz * vektor[2][2];
  vektor[2][2] = iranyx * vektor[2][2] + iranyz * vektor[2][0];
  vektor[2][0] = s;
  s = iranyx * vektor[3][0] - iranyz * vektor[3][2];
  vektor[3][2] = iranyx * vektor[3][2] + iranyz * vektor[3][0];
  vektor[3][0] = s;
  s = iranyx * vektor[4][0] - iranyz * vektor[4][2];
  vektor[4][2] = iranyx * vektor[4][2] + iranyz * vektor[4][0];
  vektor[4][0] = s;
  s = iranyx * vektor[5][0] - iranyz * vektor[5][2];
  vektor[5][2] = iranyx * vektor[5][2] + iranyz * vektor[5][0];
  vektor[5][0] = s;

  s = iranyx * S[0] - iranyz * S[2];
  S[2] = iranyx * S[2] + iranyz * S[0];
  S[0] = s;
  x += S[0], y += S[1], z += S[2];

  koord[0].x = (float)x + xs * vektor[0][0] + ys * vektor[4][0] + zs * vektor[2][0];
  koord[0].y = (float)y + xs * vektor[0][1] + ys * vektor[4][1] + zs * vektor[2][1];
  koord[0].z = (float)z + xs * vektor[0][2] + ys * vektor[4][2] + zs * vektor[2][2];

  koord[1].x = (float)x + xs * vektor[1][0] + ys * vektor[4][0] + zs * vektor[2][0];
  koord[1].y = (float)y + xs * vektor[1][1] + ys * vektor[4][1] + zs * vektor[2][1];
  koord[1].z = (float)z + xs * vektor[1][2] + ys * vektor[4][2] + zs * vektor[2][2];

  koord[2].x = (float)x + xs * vektor[1][0] + ys * vektor[4][0] + zs * vektor[3][0];
  koord[2].y = (float)y + xs * vektor[1][1] + ys * vektor[4][1] + zs * vektor[3][1];
  koord[2].z = (float)z + xs * vektor[1][2] + ys * vektor[4][2] + zs * vektor[3][2];

  koord[3].x = (float)x + xs * vektor[0][0] + ys * vektor[4][0] + zs * vektor[3][0];
  koord[3].y = (float)y + xs * vektor[0][1] + ys * vektor[4][1] + zs * vektor[3][1];
  koord[3].z = (float)z + xs * vektor[0][2] + ys * vektor[4][2] + zs * vektor[3][2];

  koord[4].x = (float)x + xs * vektor[0][0] + ys * vektor[5][0] + zs * vektor[2][0];
  koord[4].y = (float)y + xs * vektor[0][1] + ys * vektor[5][1] + zs * vektor[2][1];
  koord[4].z = (float)z + xs * vektor[0][2] + ys * vektor[5][2] + zs * vektor[2][2];

  koord[5].x = (float)x + xs * vektor[1][0] + ys * vektor[5][0] + zs * vektor[2][0];
  koord[5].y = (float)y + xs * vektor[1][1] + ys * vektor[5][1] + zs * vektor[2][1];
  koord[5].z = (float)z + xs * vektor[1][2] + ys * vektor[5][2] + zs * vektor[2][2];

  koord[6].x = (float)x + xs * vektor[1][0] + ys * vektor[5][0] + zs * vektor[3][0];
  koord[6].y = (float)y + xs * vektor[1][1] + ys * vektor[5][1] + zs * vektor[3][1];
  koord[6].z = (float)z + xs * vektor[1][2] + ys * vektor[5][2] + zs * vektor[3][2];

  koord[7].x = (float)x + xs * vektor[0][0] + ys * vektor[5][0] + zs * vektor[3][0];
  koord[7].y = (float)y + xs * vektor[0][1] + ys * vektor[5][1] + zs * vektor[3][1];
  koord[7].z = (float)z + xs * vektor[0][2] + ys * vektor[5][2] + zs * vektor[3][2];

  for (i = 0; i < 6; i++)
  {
    int sor[4];
    int arnyalat;
    xvis = (koord[box[i][0]].x + koord[box[i][3]].x) / 2;
    yvis = (koord[box[i][0]].y + koord[box[i][3]].y) / 2; 
    zvis = (koord[box[i][0]].z + koord[box[i][3]].z) / 2 + 1000;

    if (xvis * vektor[i][0] + yvis * vektor[i][1] + zvis * vektor[i][2] < 0)
    {
      if ((reteg == 0 && i == tengely * 2) || (reteg == 2 && i == tengely * 2 + 1))
      {
        for (k = 0; k < 4; k++)
          sor[k] = box[i][k];
        for (j = 0; j < 2; j++)
        {
          for (k = 0; k < 2; k++)
          {
            int ketk = k << 1;
            koord[8 + ketk + (j << 2)].x = ((koord[sor[ketk]].x << 1) + koord[sor[ketk + 1]].x) / 3;
            koord[8 + ketk + (j << 2)].y = ((koord[sor[ketk]].y << 1) + koord[sor[ketk + 1]].y) / 3;
            koord[8 + ketk + (j << 2)].z = ((koord[sor[ketk]].z << 1) + koord[sor[ketk + 1]].z) / 3;
          }
          for (k = 0; k < 2; k++)
          {
            int ketk = k << 1;
            koord[9 + ketk + (j << 2)].x = (koord[sor[ketk]].x + (koord[sor[ketk + 1]].x << 1)) / 3;
            koord[9 + ketk + (j << 2)].y = (koord[sor[ketk]].y + (koord[sor[ketk + 1]].y << 1)) / 3;
            koord[9 + ketk + (j << 2)].z = (koord[sor[ketk]].z + (koord[sor[ketk + 1]].z << 1)) / 3;
          }
          sor[0] = box[i][0], sor[1] = box[i][2], sor[2] = box[i][1], sor[3] = box[i][3];
        }
        for (k = 0; k < 2; k++)
        {
          koord[16 + k].x = ((koord[8 + k].x << 1) + koord[10 + k].x) / 3;
          koord[16 + k].y = ((koord[8 + k].y << 1) + koord[10 + k].y) / 3;
          koord[16 + k].z = ((koord[8 + k].z << 1) + koord[10 + k].z) / 3;
        }
        for (k = 0; k < 2; k++)
        {
          koord[18 + k].x = (koord[8 + k].x + (koord[10 + k].x << 1)) / 3;
          koord[18 + k].y = (koord[8 + k].y + (koord[10 + k].y << 1)) / 3;
          koord[18 + k].z = (koord[8 + k].z + (koord[10 + k].z << 1)) / 3;
        }
        for (k = 0; k < 4; k++)
          sor[k] = box[i][k];
        int sor2[] = {
          sor[0], 8, 16, 12, 8, 9, 
          17, 16, 9, sor[1], 14, 17, 
          12, 16, 18, 13, 16, 17, 
          19, 18, 17, 14, 15, 19, 
          13, 18, 10, sor[2], 18, 19, 
          11, 10, 19, 15, sor[3], 11
        };

        int s = 0;
        for (k = 0; k < 9; k++)
        {
          for (j = 0; j < 4; j++)
          {
            boxkoord[j].x = koord[sor2[s]].x;
            boxkoord[j].y = koord[sor2[s]].y;
            boxkoord[j].z = koord[sor2[s]].z;
            s++;
          }
          if (light)
          {
            float x = vektor[i][0], y = vektor[i][1], z = vektor[i][2];
            float sx = (boxkoord[0].x + boxkoord[1].x + boxkoord[2].x + boxkoord[3].x) / 4;
            float sy = (boxkoord[0].y + boxkoord[1].y + boxkoord[2].y + boxkoord[3].y) / 4;
            float sz = (boxkoord[0].z + boxkoord[1].z + boxkoord[2].z + boxkoord[3].z) / 4;
            float X = sx - LIGHTX, Y = sy - LIGHTY, Z = sz - LIGHTZ;
            arnyalat = (double)(x * X + y * Y + z * Z) 
                        / sqrt((double)(x * x + y * y + z * z)) 
                        / sqrt((double)(X * X + Y * Y + Z * Z)) * -15;

            if (elhelyezes_mod)
              if (melyikkocka != aktiv_kocka)
                arnyalat = 0;
              else
                arnyalat = 15;
            if (arnyalat < 0)
              arnyalat = 0;
          }
          else
            arnyalat = 15;
          setcolor(kocka[melyikkocka][i][k] + arnyalat * 16);
          rectangle3d();
        }
        continue;
      }
      if (reteg == 1 && (i == tengely * 2 || i == tengely * 2 + 1) ||
         (reteg == 0 && i == tengely * 2 + 1) || (reteg == 2 && i == tengely * 2))
      {
        setcolor(16);
        for (k = 0; k < 2; k++)
        {
          boxkoord[k].x = koord[box[i][k]].x;
          boxkoord[k].y = koord[box[i][k]].y;
          boxkoord[k].z = koord[box[i][k]].z;
        }
        for (k = 2; k < 4; k++)
        {
          boxkoord[k].x = koord[box[i][5 - k]].x;
          boxkoord[k].y = koord[box[i][5 - k]].y;
          boxkoord[k].z = koord[box[i][5 - k]].z;
        }
        rectangle3d();
        continue;
      }

      switch (tengely)
      {
      case 0:
        for (k = 0; k < 4; k++)
          sor[k] = box[i][k];
        break;
      case 1:
        if (i < 2)
          for (k = 0; k < 4; k++)
            sor[k] = box[i][k];
        else
          sor[0] = box[i][0], sor[1] = box[i][2], sor[2] = box[i][1], sor[3] = box[i][3];
        break;
      case 2:
        sor[0] = box[i][0], sor[1] = box[i][2], sor[2] = box[i][1], sor[3] = box[i][3];
        break;
      }
      for (k = 0; k < 2; k++)
      {
        int ketk = k << 1;
        koord[8 + k].x = ((koord[sor[ketk]].x << 1) + koord[sor[ketk + 1]].x) / 3;
        koord[8 + k].y = ((koord[sor[ketk]].y << 1) + koord[sor[ketk + 1]].y) / 3;
        koord[8 + k].z = ((koord[sor[ketk]].z << 1) + koord[sor[ketk + 1]].z) / 3;
      }
      for (k = 0; k < 2; k++)
      {
        int ketk = k << 1;
        koord[10 + k].x = (koord[sor[ketk]].x + (koord[sor[ketk + 1]].x << 1)) / 3;
        koord[10 + k].y = (koord[sor[ketk]].y + (koord[sor[ketk + 1]].y << 1)) / 3;
        koord[10 + k].z = (koord[sor[ketk]].z + (koord[sor[ketk + 1]].z << 1)) / 3;
      }
      int sor2[] = {sor[0], sor[2], 9, 8, 10, 11, 9, 8, 10, 11, sor[3], sor[1]};
      int s = 0;
      for (k = 0; k < 3; k++)
      {
        for (j = 0; j < 4; j++)
        {
          boxkoord[j].x = koord[sor2[s]].x;
          boxkoord[j].y = koord[sor2[s]].y;
          boxkoord[j].z = koord[sor2[s]].z;
          s++;
        }
        if (light)
        {
          float x = vektor[i][0], y = vektor[i][1], z = vektor[i][2];
          float sx = (boxkoord[0].x + boxkoord[1].x + boxkoord[2].x + boxkoord[3].x) / 4;
          float sy = (boxkoord[0].y + boxkoord[1].y + boxkoord[2].y + boxkoord[3].y) / 4;
          float sz = (boxkoord[0].z + boxkoord[1].z + boxkoord[2].z + boxkoord[3].z) / 4;
          float X = sx - LIGHTX, Y = sy - LIGHTY, Z = sz - LIGHTZ;
          arnyalat = (double)(x * X + y * Y + z * Z) 
                    / sqrt((double)(x * x + y * y + z * z)) 
                    / sqrt((double)(X * X + Y * Y + Z * Z)) * -15;
          if (elhelyezes_mod)
            if (melyikkocka != aktiv_kocka)
              arnyalat = 0;
            else
              arnyalat = 15;

          if (arnyalat < 0)
            arnyalat = 0;
        }
        else
          arnyalat = 15;
        setcolor(kocka[melyikkocka][i][lapok[tengely][reteg][i][k]] + arnyalat * 16);
        rectangle3d();
      }
    }
  }
  for (i = 0; i < 6; i++)
    vektor[i][0] = vektor[i][1] = vektor[i][2] = 0;
  vektor[0][0] = vektor[4][1] = vektor[2][2] = -1;
  vektor[1][0] = vektor[5][1] = vektor[3][2] = 1;
}
void forgat_atszinez(int melyikkocka, int tengely, int reteg, int irany)
{
  int lapok[3][3][3][4] = {{{{0, 0, 8, 0}, {1, 1, 7, 1}, {2, 2, 6, 2}},
                            {{3, 3, 5, 3}, {4, 4, 4, 4}, {5, 5, 3, 5}},
                            {{6, 6, 2, 6}, {7, 7, 1, 7}, {8, 8, 0, 8}}},
                           {{{6, 8, 2, 0}, {7, 5, 1, 3}, {8, 2, 0, 6}},
                            {{3, 7, 5, 1}, {4, 4, 4, 4}, {5, 1, 3, 7}},
                            {{0, 6, 8, 2}, {1, 3, 7, 5}, {2, 0, 6, 8}}},
                           {{{0, 0, 0, 0}, {3, 3, 3, 3}, {6, 6, 6, 6}},
                            {{1, 1, 1, 1}, {4, 4, 4, 4}, {7, 7, 7, 7}},
                            {{2, 2, 2, 2}, {5, 5, 5, 5}, {8, 8, 8, 8}}}};
  int oldalak[3][4] = {{2, 5, 3, 4}, {0, 4, 1, 5}, {0, 2, 1, 3}};
  int i, k = 0;
  for (i = 0; i < 3; i++)
  {
    int count = (3 - irany * 3) >> 1;
    int s = kocka[melyikkocka][oldalak[tengely][count]][lapok[tengely][reteg][i][count]];
    for (k = 0; k < 3; k++)
    {
      kocka[melyikkocka][oldalak[tengely][count]][lapok[tengely][reteg][i][count]] = kocka[melyikkocka][oldalak[tengely][count + irany]][lapok[tengely][reteg][i][count + irany]];
      count += irany;
    }
    kocka[melyikkocka][oldalak[tengely][count]][lapok[tengely][reteg][i][count]] = s;
  }
  if (reteg != 1)
  {
    int tbl[2][4] = {{0, 2, 8, 6}, {1, 5, 7, 3}};
    int lap = (tengely << 1) + (reteg >> 1);
    int count;
    (tengely != 2) ? (irany *= (reteg - 1)) : (irany *= (1 - reteg));

    (irany != -1) ? (count = 0) : (count = 3);
    int s1 = kocka[melyikkocka][lap][tbl[0][count]];
    int s2 = kocka[melyikkocka][lap][tbl[1][count]];
    for (k = 0; k < 3; k++)
    {
      kocka[melyikkocka][lap][tbl[0][count]] = kocka[melyikkocka][lap][tbl[0][count + irany]];
      kocka[melyikkocka][lap][tbl[1][count]] = kocka[melyikkocka][lap][tbl[1][count + irany]];
      count += irany;
    }
    kocka[melyikkocka][lap][tbl[0][count]] = s1;
    kocka[melyikkocka][lap][tbl[1][count]] = s2;
  }
}

void forgat(int tengely, int reteg, int irany, int rajzol, int elmozdulas)
{
  int i, r[3];
  r[0] = reteg & 1;
  r[1] = (reteg & 2) >> 1;
  r[2] = (reteg & 4) >> 2;
  int oldirany = irany;
  irany *= speed;
  int elmozdul[2] = {0, 0};
  elmozdul[aktiv_kocka] = elmozdulas;
  cleardevice();
  if (!tengely)
  { //x tengely:
    if (rajzol)
    {
      for (i = 0; abs(i) <= 90; i += irany)
      {
        forgat_rajz(0, 0, 2, r[2] * i, kocka_helye[0].x, kocka_helye[0].y, kocka_helye[0].z - elmozdul[0] * i / 90);
        forgat_rajz(0, 0, 1, r[1] * i, kocka_helye[0].x, kocka_helye[0].y, kocka_helye[0].z - elmozdul[0] * i / 90);
        forgat_rajz(0, 0, 0, r[0] * i, kocka_helye[0].x, kocka_helye[0].y, kocka_helye[0].z - elmozdul[0] * i / 90);

        //    x<- z, r<- r, i<- i
        forgat_rajz(1, 1, 0, r[0] * i, kocka_helye[1].x, kocka_helye[1].y, kocka_helye[1].z + elmozdul[1] * i / 90);
        forgat_rajz(1, 1, 1, r[1] * i, kocka_helye[1].x, kocka_helye[1].y, kocka_helye[1].z + elmozdul[1] * i / 90);
        forgat_rajz(1, 1, 2, r[2] * i, kocka_helye[1].x, kocka_helye[1].y, kocka_helye[1].z + elmozdul[1] * i / 90);
        drawscreen();
        if (kbhit())
        {
          int a = getch();
          if (a == 'p')
            ment();
          else if (a == 'm')
            mentkocka();
          else if (a == 'l')
            betoltkocka();
          else
            ungetch(a);
        }
        cleardevice();
      }
    }
    kocka_helye[aktiv_kocka].z -= (elmozdulas * oldirany);
    for (i = 0; i < 3; i++)
    {
      if (r[i])
      {
        forgat_atszinez(0, tengely, r[i] * i, oldirany);
        if (rajzol)
          forgat_atszinez(1, 1, r[i] * i, oldirany);
      }
    }
  }
  else if (tengely == 1) //z tengely:
  {
    if (rajzol)
    {
      for (i = 0; abs(i) <= 90; i += irany)
      {
        forgat_rajz(0, 1, 2, r[2] * i, kocka_helye[0].x, kocka_helye[0].y, kocka_helye[0].z - elmozdul[0] * i / 90);
        forgat_rajz(0, 1, 1, r[1] * i, kocka_helye[0].x, kocka_helye[0].y, kocka_helye[0].z - elmozdul[0] * i / 90);
        forgat_rajz(0, 1, 0, r[0] * i, kocka_helye[0].x, kocka_helye[0].y, kocka_helye[0].z - elmozdul[0] * i / 90);
        //    z<- y, r<- r, i<- -i
        i = -i;
        forgat_rajz(1, 2, 0, r[0] * i, kocka_helye[1].x, kocka_helye[1].y, kocka_helye[1].z + elmozdul[1] * i / 90);
        forgat_rajz(1, 2, 1, r[1] * i, kocka_helye[1].x, kocka_helye[1].y, kocka_helye[1].z + elmozdul[1] * i / 90);
        forgat_rajz(1, 2, 2, r[2] * i, kocka_helye[1].x, kocka_helye[1].y, kocka_helye[1].z + elmozdul[1] * i / 90);
        i = -i;
        drawscreen();
        if (kbhit())
        {
          int a;
          a = getch();
          if (a == 'p')
            ment();
          else if (a == ('m'))
            mentkocka();
          else if (a == ('l'))
            betoltkocka();
          else
            ungetch(a);
        }
        cleardevice();
      }
    }
    kocka_helye[aktiv_kocka].z -= (elmozdulas * oldirany);
    for (i = 0; i < 3; i++)
    {
      if (r[i])
      {
        forgat_atszinez(0, tengely, r[i] * i, oldirany);
        if (rajzol)
          forgat_atszinez(1, 2, r[i] * i, -oldirany);
      }
    }
  }
  else if (tengely == 2) //y tengely:
  {
    if (rajzol)
    {
      for (i = 0; abs(i) <= 90; i += irany)
      {
        forgat_rajz(0, 2, 0, r[0] * i, kocka_helye[0].x, kocka_helye[0].y, kocka_helye[0].z - elmozdul[0] * i / 90);
        forgat_rajz(0, 2, 1, r[1] * i, kocka_helye[0].x, kocka_helye[0].y, kocka_helye[0].z - elmozdul[0] * i / 90);
        forgat_rajz(0, 2, 2, r[2] * i, kocka_helye[0].x, kocka_helye[0].y, kocka_helye[0].z - elmozdul[0] * i / 90);
        //    y<- x, r<- r, i<- -i
        i = -i;
        forgat_rajz(1, 0, 2, r[2] * i, kocka_helye[1].x, kocka_helye[1].y, kocka_helye[1].z + elmozdul[1] * i / 90);
        forgat_rajz(1, 0, 1, r[1] * i, kocka_helye[1].x, kocka_helye[1].y, kocka_helye[1].z + elmozdul[1] * i / 90);
        forgat_rajz(1, 0, 0, r[0] * i, kocka_helye[1].x, kocka_helye[1].y, kocka_helye[1].z + elmozdul[1] * i / 90);
        i = -i;
        drawscreen();
        if (kbhit())
        {
          int a;
          a = getch();
          if (a == 'p')
            ment();
          else if (a == ('m'))
            mentkocka();
          else if (a == ('l'))
            betoltkocka();
          else
            ungetch(a);
        }
        cleardevice();
      }
    }
    kocka_helye[aktiv_kocka].z -= (elmozdulas * oldirany);
    for (i = 0; i < 3; i++)
    {
      if (r[i])
      {
        forgat_atszinez(0, tengely, r[i] * i, oldirany);
        if (rajzol)
          forgat_atszinez(1, 0, r[i] * i, -oldirany);
      }
    }
  }
  //Ezt az�rt vettem ki, hogy ne remegjen a kocka minden forgat�s v�g�n.
  //  if(rajzol) kocka_rajz();
}

int mentkocka()
{
  FILE *fp;
  fp = fopen("rubik.dat", "wt");
  strcpy(kockaAdatFajl, "rubik.dat");
  fwrite("RUBIK", 5, 1, fp);
  fwrite(kocka[0], 2 * 6 * 9 * sizeof(int), 1, fp);
  fclose(fp);
}

int betoltkocka()
{
  FILE *fp;
  fp = fopen(kockaAdatFajl, "rt");
  if (fp)
  {
    char id[6] = {0, 0, 0, 0, 0, 0};
    fread(id, 5, 1, fp);
    if (!strcmp(id, "RUBIK"))
      fread(kocka[0], 2 * 6 * 9 * sizeof(int), 1, fp);
    fclose(fp);
    kocka_rajz();
  }
}

int ment()
{
  typedef struct
  {
    char manufacturer;
    char version;
    char encoding;
    char bits_per_pixel;
    int xmin, ymin;
    int xmax, ymax;
    int hres;
    int vres;
    char palette[48];
    char reserved;
    char colour_planes;
    int bytes_per_line;
    int palette_type;
    char filler[58];
  } PCXHEAD;
  PCXHEAD header;
  mouse_status(BUSY); //homok�ra

  FILE *fp = fopen("out.pcx", "wb");
  header.manufacturer = 0x0a;
  header.version = 5;
  header.encoding = 1;
  header.bits_per_pixel = 8;
  header.xmin = 0;
  header.ymin = 0;
  header.xmax = 640 - 1;
  header.ymax = 480 - 1;
  header.hres = 0;
  header.vres = 0;
  for (unsigned int i = 0; i < 48; i++)
    header.palette[i] = 0;
  header.reserved = 0;
  header.colour_planes = 1;
  header.bytes_per_line = 640;
  header.palette_type = 1;
  for (i = 0; i < 58; i++)
    header.filler[i] = 0;
  for (i = 0; i < sizeof(header); i++)
    fputc(*((char *)&header + i), fp);
  struct e
  {
    unsigned char szin;
    unsigned char hely;
    int osszesen;
    int db;
  } eloszlas[256], seged;
  unsigned char s = 0;
  for (i = 0; i < 256; i++)
    eloszlas[i].szin = eloszlas[i].hely = i, eloszlas[i].osszesen = 0, eloszlas[i].db = 0;
  for (int y = 0; y < 200; y++)
    for (int x = 0; x < 320; x++)
    {
      unsigned char c = getpixel(x, y);
      eloszlas[c].osszesen++;
      if (s != c)
        eloszlas[i].db++;
      s = c;
    }
  for (i = 0; i < 256; i++)
  {
    if (eloszlas[i].db)
      eloszlas[i].osszesen /= eloszlas[i].db;
  }
  for (int k = 1; k < 255; k++)
  {
    int max = k;
    for (int i = k + 1; i < 256; i++)
    {
      if (eloszlas[i].osszesen < eloszlas[max].osszesen)
        max = i;
    }
    //    eloszlas[eloszlas[max].szin].hely=k; eloszlas[eloszlas[k].szin].hely=max;
    eloszlas[max].hely = k;
    eloszlas[k].hely = max;
    seged.szin = eloszlas[max].szin, seged.osszesen = eloszlas[max].osszesen;
    eloszlas[max].szin = eloszlas[k].szin, eloszlas[max].osszesen = eloszlas[k].osszesen;
    eloszlas[k].szin = seged.szin, eloszlas[k].osszesen = seged.osszesen;
  }
  //  char ki;
  unsigned char c = getpixel(0, 0);
  for (long ptr = 1; ptr < 307200L;)
  {
    s = c;
    int o = 1;
    c = getpixel(ptr % 640, ptr / 640);
    ptr++;
    while (s == c && o < 63 /*&&(ptr+1)%640*/)
    {
      o++;
      c = getpixel(ptr % 640, ptr / 640);
      ptr++;
    }
    i = eloszlas[s].hely;
    if ((i & 0xc0) == 0xc0 || o > 1)
    {
      fputc(192 + o, fp);
      fputc(s, fp);
    }
    else
      fputc(s, fp);
  }
  fputc(0x0c, fp);
  for (i = 0; i < 256; i++)
  {
    unsigned char r, g, b;
    asm {
    mov bx,i
    mov ax,0x1015
    int 0x10
    }
    r = _DH, g = _CH, b = _CL;
    fputc(4 * r, fp);
    fputc(4 * g, fp);
    fputc(4 * b, fp);
  }
  fclose(fp);
  mouse_status(ARROW); //nyil
  return 0;
}

void kocka_allapot(int melyikkocka)
{
  float xs, ys, zs;
  float x = kocka_helye[melyikkocka].x;
  float y = kocka_helye[melyikkocka].y;
  float z = kocka_helye[melyikkocka].z;
  xs = zs = ys = MERET * 1.5;
  floatkoord vektor[6];
  int fok = egesz_kocka_elforditasa;
  if (melyikkocka)
    fok = -fok;
  vektor[1].x = vektor[3].z = Sin[540 + fok];
  vektor[0].z = vektor[3].x = Sin[450 + fok];
  vektor[0].x = vektor[2].z = -vektor[1].x;
  vektor[1].z = vektor[2].x = -vektor[0].z;

  for (int i = 0; i < 6; i++)
    vektor[i].y = 0;
  vektor[4].y = -1;
  vektor[5].y = 1;
  vektor[4].x = vektor[4].z = vektor[5].x = vektor[5].z = 0;

  kocka_sarkai[melyikkocka][0].x = (float)x + xs * vektor[0].x + ys * vektor[4].x + zs * vektor[2].x;
  kocka_sarkai[melyikkocka][0].y = (float)y + xs * vektor[0].y + ys * vektor[4].y + zs * vektor[2].y;
  kocka_sarkai[melyikkocka][0].z = (float)z + xs * vektor[0].z + ys * vektor[4].z + zs * vektor[2].z;

  kocka_sarkai[melyikkocka][1].x = (float)x + xs * vektor[1].x + ys * vektor[4].x + zs * vektor[2].x;
  kocka_sarkai[melyikkocka][1].y = (float)y + xs * vektor[1].y + ys * vektor[4].y + zs * vektor[2].y;
  kocka_sarkai[melyikkocka][1].z = (float)z + xs * vektor[1].z + ys * vektor[4].z + zs * vektor[2].z;

  kocka_sarkai[melyikkocka][2].x = (float)x + xs * vektor[1].x + ys * vektor[4].x + zs * vektor[3].x;
  kocka_sarkai[melyikkocka][2].y = (float)y + xs * vektor[1].y + ys * vektor[4].y + zs * vektor[3].y;
  kocka_sarkai[melyikkocka][2].z = (float)z + xs * vektor[1].z + ys * vektor[4].z + zs * vektor[3].z;

  kocka_sarkai[melyikkocka][3].x = (float)x + xs * vektor[0].x + ys * vektor[4].x + zs * vektor[3].x;
  kocka_sarkai[melyikkocka][3].y = (float)y + xs * vektor[0].y + ys * vektor[4].y + zs * vektor[3].y;
  kocka_sarkai[melyikkocka][3].z = (float)z + xs * vektor[0].z + ys * vektor[4].z + zs * vektor[3].z;

  kocka_sarkai[melyikkocka][4].x = (float)x + xs * vektor[0].x + ys * vektor[5].x + zs * vektor[2].x;
  kocka_sarkai[melyikkocka][4].y = (float)y + xs * vektor[0].y + ys * vektor[5].y + zs * vektor[2].y;
  kocka_sarkai[melyikkocka][4].z = (float)z + xs * vektor[0].z + ys * vektor[5].z + zs * vektor[2].z;

  kocka_sarkai[melyikkocka][5].x = (float)x + xs * vektor[1].x + ys * vektor[5].x + zs * vektor[2].x;
  kocka_sarkai[melyikkocka][5].y = (float)y + xs * vektor[1].y + ys * vektor[5].y + zs * vektor[2].y;
  kocka_sarkai[melyikkocka][5].z = (float)z + xs * vektor[1].z + ys * vektor[5].z + zs * vektor[2].z;

  kocka_sarkai[melyikkocka][6].x = (float)x + xs * vektor[1].x + ys * vektor[5].x + zs * vektor[3].x;
  kocka_sarkai[melyikkocka][6].y = (float)y + xs * vektor[1].y + ys * vektor[5].y + zs * vektor[3].y;
  kocka_sarkai[melyikkocka][6].z = (float)z + xs * vektor[1].z + ys * vektor[5].z + zs * vektor[3].z;

  kocka_sarkai[melyikkocka][7].x = (float)x + xs * vektor[0].x + ys * vektor[5].x + zs * vektor[3].x;
  kocka_sarkai[melyikkocka][7].y = (float)y + xs * vektor[0].y + ys * vektor[5].y + zs * vektor[3].y;
  kocka_sarkai[melyikkocka][7].z = (float)z + xs * vektor[0].z + ys * vektor[5].z + zs * vektor[3].z;

  lap_kozepe[melyikkocka][0].x = (kocka_sarkai[melyikkocka][3].x + kocka_sarkai[melyikkocka][4].x) >> 1;
  lap_kozepe[melyikkocka][0].y = (kocka_sarkai[melyikkocka][3].y + kocka_sarkai[melyikkocka][4].y) >> 1;
  lap_kozepe[melyikkocka][0].z = (kocka_sarkai[melyikkocka][3].z + kocka_sarkai[melyikkocka][4].z) >> 1;

  lap_kozepe[melyikkocka][1].x = (kocka_sarkai[melyikkocka][1].x + kocka_sarkai[melyikkocka][6].x) >> 1;
  lap_kozepe[melyikkocka][1].y = (kocka_sarkai[melyikkocka][1].y + kocka_sarkai[melyikkocka][6].y) >> 1;
  lap_kozepe[melyikkocka][1].z = (kocka_sarkai[melyikkocka][1].z + kocka_sarkai[melyikkocka][6].z) >> 1;

  lap_kozepe[melyikkocka][2].x = (kocka_sarkai[melyikkocka][0].x + kocka_sarkai[melyikkocka][5].x) >> 1;
  lap_kozepe[melyikkocka][2].y = (kocka_sarkai[melyikkocka][0].y + kocka_sarkai[melyikkocka][5].y) >> 1;
  lap_kozepe[melyikkocka][2].z = (kocka_sarkai[melyikkocka][0].z + kocka_sarkai[melyikkocka][5].z) >> 1;

  lap_kozepe[melyikkocka][3].x = (kocka_sarkai[melyikkocka][2].x + kocka_sarkai[melyikkocka][7].x) >> 1;
  lap_kozepe[melyikkocka][3].y = (kocka_sarkai[melyikkocka][2].y + kocka_sarkai[melyikkocka][7].y) >> 1;
  lap_kozepe[melyikkocka][3].z = (kocka_sarkai[melyikkocka][2].z + kocka_sarkai[melyikkocka][7].z) >> 1;

  lap_kozepe[melyikkocka][4].x = (kocka_sarkai[melyikkocka][0].x + kocka_sarkai[melyikkocka][2].x) >> 1;
  lap_kozepe[melyikkocka][4].y = (kocka_sarkai[melyikkocka][0].y + kocka_sarkai[melyikkocka][2].y) >> 1;
  lap_kozepe[melyikkocka][4].z = (kocka_sarkai[melyikkocka][0].z + kocka_sarkai[melyikkocka][2].z) >> 1;

  lap_kozepe[melyikkocka][5].x = (kocka_sarkai[melyikkocka][4].x + kocka_sarkai[melyikkocka][6].x) >> 1;
  lap_kozepe[melyikkocka][5].y = (kocka_sarkai[melyikkocka][4].y + kocka_sarkai[melyikkocka][6].y) >> 1;
  lap_kozepe[melyikkocka][5].z = (kocka_sarkai[melyikkocka][4].z + kocka_sarkai[melyikkocka][6].z) >> 1;

  for (i = 0; i < 6; i++)
  {
    koordinata v;
    v.x = 0 - lap_kozepe[melyikkocka][i].x;
    v.y = 0 - lap_kozepe[melyikkocka][i].y;
    v.z = -1000 + lap_kozepe[melyikkocka][i].z; // a z koordin�ta kifel� cs�kken.

    if (skal(vektor[i], v) > 0)
      lathato_lapok[melyikkocka][i] = 1;
    else
      lathato_lapok[melyikkocka][i] = 0;
  }

  for (i = 0; i < 8; i++)
  {
    perspekt_koord[melyikkocka][i].x = kocka_sarkai[melyikkocka][i].x;
    perspekt_koord[melyikkocka][i].y = kocka_sarkai[melyikkocka][i].y;
    perspect(&perspekt_koord[melyikkocka][i].x, &perspekt_koord[melyikkocka][i].y, kocka_sarkai[melyikkocka][i].z);
    perspekt_koord[melyikkocka][i].x += 320;
    perspekt_koord[melyikkocka][i].y = 240 - (perspekt_koord[melyikkocka][i].y + 270);

    //    putpixel(perspekt_koord[melyikkocka][i].x,perspekt_koord[melyikkocka][i].y,255);
    //    for(int k=0;k<100;k++)putpixel(k,100,255);
    //    drawscreen();
    //    getch();
  }
}

floatkoord sum(koordinata v1, floatkoord v2)
{
  floatkoord s;
  s.x = v1.x + v2.x;
  s.y = v1.y + v2.y;
  s.z = v1.z + v2.z;
  return s;
}

float skal(floatkoord v1, koordinata v2)
{
  return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z); //hossz(v1)/hossz(v2);
}

float hossz(floatkoord2d v)
{
  return sqrt(v.x * v.x + v.y * v.y);
}

float hossz(floatkoord v)
{
  return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

float hossz(koordinata v)
{
  return sqrt(((float)v.x) * v.x + (float)v.y * v.y + (float)v.z * v.z);
}

void set(koordinata *v, int x, int y, int z)
{
  v->x = x, v->y = y, v->z = z;
}

int sign(float x)
{
  if (x < 0)
    return -1;
  else
    return 1;
}

void hol_all_az_eger(int &Kocka, int &lap, int &negyzet, int &szin, int mx, int my)
{
  int ki = 0;
  for (Kocka = 0; !ki && Kocka < 2; Kocka++)
  { // kocka_allapot(Kocka);// Mikor ide kerul a vezerles, akkor ennek rendben kell lennie
    int fosarok[6] = {3, 1, 0, 2, 3, 5};
    int bazis[2][6] = {{7, 5, 4, 6, 0, 4}, {0, 2, 1, 3, 2, 6}};
    for (lap = 0; !ki && lap < 6; lap++)
    {
      if (lathato_lapok[Kocka][lap])
      {
        floatkoord2d v;
        // a fosarokhoz igazitjuk a koordinatarendszert
        v.x = (float)mx - perspekt_koord[Kocka][fosarok[lap]].x;
        v.y = (float)my - perspekt_koord[Kocka][fosarok[lap]].y;
        //az aktualis lap bazisvektorai
        floatkoord2d b[2];
        b[0].x = perspekt_koord[Kocka][bazis[0][lap]].x - perspekt_koord[Kocka][fosarok[lap]].x;
        b[0].y = perspekt_koord[Kocka][bazis[0][lap]].y - perspekt_koord[Kocka][fosarok[lap]].y;
        b[1].x = perspekt_koord[Kocka][bazis[1][lap]].x - perspekt_koord[Kocka][fosarok[lap]].x;
        b[1].y = perspekt_koord[Kocka][bazis[1][lap]].y - perspekt_koord[Kocka][fosarok[lap]].y;
        //	float h=hossz(b[0]);
        b[0].x /= 3, b[0].y /= 3;
        //	h=hossz(b[1]);
        b[1].x /= 3, b[1].y /= 3;
        // v vektor felirasa b b�zisban
        floatkoord2d k;
        k.y = (b[0].x * v.y - v.x * b[0].y) / (b[0].x * b[1].y - b[1].x * b[0].y);
        k.x = (v.y - b[1].y * k.y) / b[0].y;
        // megfelel� n�gyzet meghatarozasa:
        if (k.y < 3 && k.y >= 0 && k.x < 3 && k.x >= 0)
        {
          switch (lap) //Ez az �n szem�lyes h�lyes�gem:
          {
          case 0:
            lap = 3;
            break;
          case 1:
            lap = 2;
            break;
          case 2:
            lap = 0;
            break;
          case 3:
            lap = 1;
            break;
          }
          negyzet = (int)k.y * 3 + (int)k.x;
          szin = kocka[Kocka][lap][negyzet];
          //	  kocka[Kocka][lap][negyzet]=15;
          //	  kocka_rajz();
          ki = 1;
        }
      }
    }
  }
  /*  if(ki)
  { kocka[0][lap][negyzet]=0;
    kocka_rajz();
  }                */
  Kocka--;
  lap--;
  if (!ki)
    Kocka = -1;
}

int kocka_beolvasas(void)
{
  int szin[6] = {1, 2, 4, 9, 15, 14};
  int aktiv = 0;
  for (int i = 0; i < 6; i++)
  {
    if (aktiv == i)
      rectangle2D(1 + i * 20, 1, 16, 16, szin[i] + 240, 255);
    else
      rectangle2D(1 + i * 20, 1, 16, 16, szin[i] + 240, 0);
  }
  drawscreen();
  memset(kocka, 127, sizeof(kocka));
  kocka_rajz();
  kocka_allapot(0);
  kocka_allapot(1);
  char a = 0;
  int Kocka = -1, lap, negyzet, sz;
  int szurke = 54;
  int dirty = 0;
  while (a != 27 && (a != 13 || szurke))
  {
    a = 0;
    word mx, my, mb;
    mousepos(&mx, &my, &mb);
    if (mb & 1)
    {
      hol_all_az_eger(Kocka, lap, negyzet, sz, mx, my);
      int masik_kocka_lapja[6] = {2, 3, 4, 5, 0, 1};
      int masik_kocka_negyzete[6][9] = {
          {2, 5, 8, 1, 4, 7, 0, 3, 6},
          {2, 5, 8, 1, 4, 7, 0, 3, 6}, //??? ezek nincsenek kisz�molva.
          {2, 5, 8, 1, 4, 7, 0, 3, 6},
          {2, 5, 8, 1, 4, 7, 0, 3, 6}, //???
          {2, 5, 8, 1, 4, 7, 0, 3, 6}, //???
          {0, 1, 2, 3, 4, 5, 6, 7, 8},
      };
      if (Kocka == 0)
      {
        if (kocka[Kocka][lap][negyzet] != szin[aktiv])
        {
          if (kocka[Kocka][lap][negyzet] > 16)
            szurke--;
          kocka[Kocka][lap][negyzet] = szin[aktiv];
          kocka[1 - Kocka][masik_kocka_lapja[lap]][masik_kocka_negyzete[lap][negyzet]] = szin[aktiv];
          kocka_rajz();
        }
        Kocka = -1;
      }
      if (Kocka == -1)
      {
        for (int i = 0; i < 6; i++)
        {
          if (mx >= 1 + i * 20 && mx <= 1 + i * 20 + 16 && my < 17 && my > 0)
            aktiv = i, i = 6;
          dirty = 1;
        }
        if (dirty)
        {
          for (i = 0; i < 6; i++)
          {
            if (aktiv == i)
              rectangle2D(1 + i * 20, 1, 16, 16, szin[i] + 240, 255);
            else
              rectangle2D(1 + i * 20, 1, 16, 16, szin[i] + 240, 0);
          }
          drawscreen();
          dirty = 0;
        }
      }
    }
    if (kbhit())
    {
      switch (a = getch())
      {
      case 72:
        forgat(1, 7, 1, 1, 0);
        kocka_rajz(), dirty = 1;
        break;
      case 80:
        forgat(1, 7, -1, 1, 0);
        kocka_rajz(), dirty = 1;
        break;
      case 75:
        forgat(2, 7, 1, 1, 0);
        kocka_rajz(), dirty = 1;
        break;
      case 77:
        forgat(2, 7, -1, 1, 0);
        kocka_rajz(), dirty = 1;
        break;
      case 115:
        forgat(0, 7, -1, 1, 0);
        kocka_rajz(), dirty = 1;
        break;
      case 116:
        forgat(0, 7, 1, 1, 0);
        kocka_rajz(), dirty = 1;
        break;
      case 13:
        a = 13;
      }
      if (dirty)
      {
        for (i = 0; i < 6; i++)
        {
          if (aktiv == i)
            rectangle2D(1 + i * 20, 1, 16, 16, szin[i] + 240, 255);
          else
            rectangle2D(1 + i * 20, 1, 16, 16, szin[i] + 240, 0);
        }
        drawscreen();
        dirty = 0;
      }
    }
  }
  rectangle2D(1, 1, 630, 16, 0, 0);
  drawscreen();
  return a;
}
