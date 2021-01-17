#include <mem.h>
#include <dos.h>
#include <conio.h>
#include "src\grmouse.h"
#include "src\graphics.h"

/* Az eg�r kurzor�nak defin�ci�ja */
struct graphcursor
{
	byte busy[22 * 13], arrow[19 * 11];
	word oldx, oldy, oldb;
	byte status;
	byte szel[2];
	byte mag[2];
} gcursor;

/*---------------------*/
/* �ltal�nos eg�rh�v�s */
/*---------------------*/
void cmouse(word *fnum, word *arg1, word *arg2, word *arg3)
{
	struct REGPACK r;
	r.r_ax = *fnum;
	r.r_bx = *arg1;
	r.r_cx = *arg2;
	r.r_dx = *arg3;
	intr(0x33, &r);
	*fnum = r.r_ax;
	*arg1 = r.r_bx;
	*arg2 = r.r_cx;
	*arg3 = r.r_dx;
}

/*------------------------*/
/* Az eg�r inicializ�l�sa */
/*------------------------*/
int mouseinit(void)
{
	word fnum = 0, arg1, arg2, arg3;
	cmouse(&fnum, &arg1, &arg2, &arg3);
	//v�zszintes m�retek
	fnum = 7, arg1 = 0, arg2 = 0, arg3 = 625; //l�nyeges: ax<-7, cx<-0, dx<-639
	cmouse(&fnum, &arg1, &arg2, &arg3);
	//f�gg�leges m�retek
	fnum = 8, arg1 = 0, arg2 = 0, arg3 = 470; //l�nyeges: ax<-8, cx<-0, dx<-479
	cmouse(&fnum, &arg1, &arg2, &arg3);
	setmouse(100, 100);

	byte x[19][11] = {{4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
					  {1, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0},
					  {1, 2, 4, 0, 0, 0, 0, 0, 0, 0, 0},
					  {1, 2, 2, 4, 0, 0, 0, 0, 0, 0, 0},
					  {1, 3, 2, 2, 4, 0, 0, 0, 0, 0, 0},
					  {1, 3, 2, 2, 2, 4, 0, 0, 0, 0, 0},
					  {1, 3, 3, 2, 2, 2, 4, 0, 0, 0, 0},
					  {1, 3, 3, 2, 2, 2, 2, 4, 0, 0, 0},
					  {1, 3, 3, 3, 2, 2, 2, 2, 4, 0, 0},
					  {1, 3, 3, 3, 2, 2, 2, 2, 2, 4, 0},
					  {1, 3, 3, 2, 4, 4, 4, 4, 4, 4, 4},
					  {1, 3, 2, 4, 3, 2, 4, 0, 0, 0, 0},
					  {1, 2, 4, 0, 1, 3, 2, 4, 0, 0, 0},
					  {1, 4, 0, 0, 1, 3, 2, 4, 0, 0, 0},
					  {4, 0, 0, 0, 0, 1, 3, 2, 4, 0, 0},
					  {0, 0, 0, 0, 0, 1, 3, 2, 4, 0, 0},
					  {0, 0, 0, 0, 0, 0, 1, 3, 2, 4, 0},
					  {0, 0, 0, 0, 0, 0, 1, 2, 2, 4, 0},
					  {0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 0}};
	byte y[22][13] = {
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4},
		{1, 1, 3, 3, 3, 3, 2, 2, 2, 2, 2, 1, 4},
		{0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0},
		{0, 1, 2, 3, 3, 2, 2, 2, 2, 1, 1, 4, 0},
		{0, 1, 2, 3, 3, 3, 2, 2, 2, 1, 1, 4, 0},
		{0, 1, 2, 3, 3, 3, 2, 2, 2, 1, 1, 4, 0},
		{0, 1, 2, 3, 3, 3, 2, 2, 2, 1, 1, 4, 0},
		{0, 1, 2, 2, 1, 2, 4, 2, 4, 1, 1, 4, 0},
		{0, 0, 1, 2, 3, 4, 2, 4, 2, 1, 4, 0, 0},
		{0, 0, 0, 1, 2, 3, 4, 2, 1, 4, 0, 0, 0},
		{0, 0, 0, 0, 1, 3, 2, 1, 4, 0, 0, 0, 0},
		{0, 0, 0, 0, 1, 3, 2, 1, 4, 0, 0, 0, 0},
		{0, 0, 0, 1, 2, 3, 2, 2, 1, 4, 0, 0, 0},
		{0, 0, 1, 2, 3, 3, 4, 2, 2, 1, 4, 0, 0},
		{0, 1, 2, 3, 3, 3, 2, 2, 2, 1, 1, 4, 0},
		{0, 1, 2, 3, 3, 3, 4, 2, 2, 1, 1, 4, 0},
		{0, 1, 2, 3, 3, 4, 2, 4, 2, 1, 1, 4, 0},
		{0, 1, 2, 3, 1, 3, 4, 2, 4, 1, 1, 4, 0},
		{0, 1, 2, 1, 3, 1, 2, 4, 2, 4, 1, 4, 0},
		{0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 0},
		{1, 1, 3, 3, 3, 3, 2, 2, 2, 2, 2, 1, 4},
		{4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4}};
	gcursor.szel[0] = 13, gcursor.szel[1] = 11;
	gcursor.mag[0] = 22, gcursor.mag[1] = 19;
	gcursor.status = 1;
	for (int k = 0; k < gcursor.mag[0]; k++)
		for (int i = 0; i < gcursor.szel[0]; i++)
		{
			switch (y[k][i])
			{
				case 0:
					gcursor.busy[i + k * gcursor.szel[0]] = 0;
					break;
				case 1:
					gcursor.busy[i + k * gcursor.szel[0]] = 199;
					break;
				case 2:
					gcursor.busy[i + k * gcursor.szel[0]] = 191;
					break;
				case 3:
					gcursor.busy[i + k * gcursor.szel[0]] = 255;
					break;
				case 4:
					gcursor.busy[i + k * gcursor.szel[0]] = 16;
					break;
			}
		}
	for (k = 0; k < gcursor.mag[1]; k++)
		for (int i = 0; i < gcursor.szel[1]; i++)
		{
			switch (x[k][i])
			{
				case 0:
					gcursor.arrow[i + k * gcursor.szel[1]] = 0;
					break; //�tl�tsz�
				case 1:
					gcursor.arrow[i + k * gcursor.szel[1]] = 199;
					break; //vil�gos sz�rke
				case 2:
					gcursor.arrow[i + k * gcursor.szel[1]] = 191;
					break; //s�t�t sz�rke
				case 3:
					gcursor.arrow[i + k * gcursor.szel[1]] = 255;
					break; //feh�r
				case 4:
					gcursor.arrow[i + k * gcursor.szel[1]] = 16;
					break; //fekete
			}
		}
	/*  for(int i=0;i<16;i++) for(int k=0;k<16;k++) if(edge[i]&(1<<k)) gcursor.cursormask[i*16+16-k]=255;*/
	gcursor.oldx = 100, gcursor.oldy = 100;
	if (fnum == 0xffff)
	{
		setmouse(100, 100); /* Az eg�r pozicion�l�sa */
		showcursor();		/* Az eg�r megjelen�t�se */
		return 1;
	}
	return 0;
}
//Eg�r st�tusz�nak be�ll�t�sa
void mouse_status(int s)
{
	gcursor.status = s;
	showcursor();
}
/*----------------------------*/
/* Az eg�rkurzor bekapcsol�sa */
/*----------------------------*/
void showcursor(void)
{
	drawscreen();
	(gcursor.status) ? putpicture(gcursor.arrow, gcursor.oldx, gcursor.oldy, 11, 19) : putpicture(gcursor.busy, gcursor.oldx, gcursor.oldy, 13, 22);
}

/*----------------------------*/
/* Az eg�rkurzor kikapcsol�sa */
/*----------------------------*/
void hidecursor(void)
{
}

/*----------------------------------------------------------*/
/* Az eg�r poz�ci�j�nak �s a gombok �llapot�nak lek�rdez�se */
/*----------------------------------------------------------*/
void mousepos(word *x, word *y, word *b)
{
	word fnum = 3, arg1, arg2, arg3;
	cmouse(&fnum, &arg1, &arg2, &arg3);
	//  char flag=0;
	//  if(arg2>630)arg2=630,flag=1;
	//  if(arg3>470)arg3=470,flag=1;
	*x = arg2;
	*y = arg3;
	*b = arg1;
	if (arg2 != gcursor.oldx || arg3 != gcursor.oldy)
	{	// putpicture(gcursor.screenmask,gcursor.oldx,gcursor.oldy,16,16);
		// getpicture(gcursor.screenmask,*x,*y,16,16);
		// mivel a putpicture direkt �ri el a k�perny�t, ez�rt nem kell elt�rolni, hogy mi va az eg�r alatt.
		drawscreen();
	}

	if ((arg2 == gcursor.oldx && arg3 == gcursor.oldy) || gcursor.oldb != *b)
		(gcursor.status) ? putpicture(gcursor.arrow, arg2, arg3, 11, 19) : putpicture(gcursor.busy, arg2, arg3, 13, 22);
	//    putpicture(gcursor.cursormask,arg2,arg3,16,16);
	//  if(flag) setmouse(arg2,arg3);
	gcursor.oldx = *x, gcursor.oldy = *y, gcursor.oldb = *b;
}

/*---------------------------------*/
/* Az eg�r poz�ci�j�nak be�ll�t�sa */
/*---------------------------------*/
void setmouse(word x, word y)
{
	gcursor.oldx = x, gcursor.oldy = y;
	word fnum = 4, arg1, arg2 = x, arg3 = y;
	cmouse(&fnum, &arg1, &arg2, &arg3);
}
