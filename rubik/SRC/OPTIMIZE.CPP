#include <string.h>
#include <fstream.h>
struct forgatas
{
  char t, r, i;
};

char *sum = "      ";
int ekv(forgatas a, forgatas b)
{
  if (a.t == b.t && a.r == b.r && a.i == b.i)
    return 1;
  return 0;
}

int cpy(forgatas *a, forgatas b)
{
  a->t = b.t;
  a->r = b.r;
  a->i = b.i;
}

void add(forgatas a, forgatas b)
{
  strcpy(sum, "      ");
  int s;
  if (a.t != b.t)
  {
    memcpy(sum, &a, 3), memcpy(sum + 3, &b, 3);
    return;
  }
  a.r -= '0';
  b.r -= '0';
  if (a.i == b.i && !(a.r & b.r))
    a.r += (b.r + '0'), memcpy(sum, &a, 3), sum[3] = NULL;
  else if (a.i != b.i && a.r == b.r)
    sum[0] = NULL;
  else if (a.i != b.i && a.r & b.r)
  {
    s = a.r - (a.r & b.r);
    b.r -= (b.r & a.r);
    a.r = s;
    if (!a.r)
      b.r += '0', memcpy(sum, &b, 3), sum[3] = NULL;
    else if (!b.r)
      a.r += '0', memcpy(sum, &a, 3), sum[3] = NULL;
    else
      a.r += '0', b.r += '0', memcpy(sum, &a, 3), memcpy(sum + 3, &b, 3);
  }
  else
  {
    a.r += '0', b.r += '0';
    memcpy(sum, &a, 3);
    memcpy(sum + 3, &b, 3);
  }
}

char ford_r(char a)
{
  char b = '0';
  a -= '0';
  if (a & 1)
    b += 4;
  if (a & 2)
    b += 2;
  if (a & 4)
    b += 1;
  return b;
}

char ford_i(char a)
{
  if (a == '-')
    return '+';
  return '-';
}

int optimize(char *in)
{
  int hossz = strlen(in) / 3, i, k = 0, s;
  int oldhossz;
  char *ad;
  forgatas *a = (forgatas *)in, S;
  do
  {
    oldhossz = hossz;
    for (i = 0; i < hossz; i++)
    {
      if (a[i].r == '7')
      {
        cpy(&S, a[i]);
        for (k = i; k < hossz - 1; k++)
        {
          if (a[k + 1].t == S.t)
            cpy(a + k, a[k + 1]);
          else if (a[k + 1].t == 'x' && S.t == 'y')
            a[k].t = 'z', a[k].r = ford_r(a[k + 1].r), a[k].i = ford_i(a[k + 1].i);
          else if (a[k + 1].t == 'y' && S.t == 'x')
            a[k].t = 'z', a[k].r = ford_r(a[k + 1].r), a[k].i = a[k + 1].i;
          else if (a[k + 1].t == 'x' && S.t == 'z')
            a[k].t = 'y', a[k].r = ford_r(a[k + 1].r), a[k].i = a[k + 1].i;
          else if (a[k + 1].t == 'z' && S.t == 'x')
            a[k].t = 'y', a[k].r = a[k + 1].r, a[k].i = ford_i(a[k + 1].i);
          else if (a[k + 1].t == 'y' && S.t == 'z')
            a[k].t = 'x', a[k].r = a[k + 1].r, a[k].i = ford_i(a[k + 1].i);
          else if (a[k + 1].t == 'z' && S.t == 'y')
            a[k].t = 'x', a[k].r = a[k + 1].r, a[k].i = a[k + 1].i;
          if (S.i != '-' && a[k + 1].t != S.t)
            a[k].i = ford_i(a[k].i), a[k].r = ford_r(a[k].r);
        }
        hossz--, i--;
      }
    }
    //    ofstream out("alga");
    for (i = 0; i < hossz - 1; i++)
    { //if(i>=0)
      {
        add(a[i], a[i + 1]);
        s = strlen(sum);
        memcpy(in + i * 3, sum, s);
        memcpy(in + i * 3 + s, in + i * 3 + 6, (hossz - i) * 3 - s);
        if (s == 3)
          hossz--, i--;
        if (!s)
          hossz -= 2, i--; //i-=2 volt;
      }
      //      for(int z=0;z<hossz;z++) out << a[i].t << a[i].r << a[i].i;
      //      out <<endl;
    }

    for (i = 0; i < hossz; i++)
    {
      if (i < hossz - 3 && ekv(a[i], a[i + 1]) && ekv(a[i + 1], a[i + 2]) && ekv(a[i + 2], a[i + 3]))
        memcpy(in + i * 3, in + i * 3 + 12, (hossz - i) * 3 - 12), i--, hossz -= 4;
      else if (i < hossz - 2 && a[i].t == a[i + 1].t && a[i + 1].t == a[i + 2].t && a[i].i == a[i + 1].i && a[i + 1].i == a[i + 2].i)
      {
        char b = 0;
        a[i].r -= '0', a[i + 1].r -= '0', a[i + 2].r -= '0';
        if (a[i].r & 1 && a[i + 1].r & 1 && a[i + 2].r & 1)
          b = 1, a[i].r -= 1, a[i + 1].r -= 1, a[i + 2].r -= 1;
        if (a[i].r & 2 && a[i + 1].r & 2 && a[i + 2].r & 2)
          b += 2, a[i].r -= 2, a[i + 1].r -= 2, a[i + 2].r -= 2;
        if (a[i].r & 4 && a[i + 1].r & 4 && a[i + 2].r & 4)
          b += 4, a[i].r -= 4, a[i + 1].r -= 4, a[i + 2].r -= 4;
        if (b)
        {
          s = 0;
          if (a[i + 1].r && a[i + 2].r)
          {
            a[i + 1].r += '0', a[i + 2].r += '0';
            add(a[i + 1], a[i + 2]);
            s = strlen(sum);
            memcpy(in + i * 3 + 3, sum, s);
          }
          else if (a[i + 2].r)
            a[i + 1].r = a[i + 2].r + '0', s = 3;
          else if (a[i + 1].r)
            a[i + 1].r += '0', s = 3;
          a[i].r = b + '0';
          a[i].i = ford_i(a[i].i);
          memcpy(in + i * 3 + s + 3, in + i * 3 + 9, (hossz - i) * 3 - 3 - s);
          i--, hossz -= ((6 - s) / 3);
        }
        else
          a[i].r += '0', a[i + 1].r += '0', a[i + 2].r += '0';
      }
      else if (i < hossz - 1 && a[i].t == a[i + 1].t && a[i].r == a[i + 1].r && a[i].i != a[i + 1].i)
        memcpy(in + i * 3, in + i * 3 + 6, (hossz - i) * 3 - 6), i--, hossz -= 2;
    }
  } while (hossz != oldhossz);
  return hossz * 3;
}