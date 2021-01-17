struct scankoord
{
  int x;
  int y;
};

int setgraphmode();
void drawscreen();
void closegraph();
void setcolor(int);
void paletteinit();
void cleardevice();
void setbkcolor(int);
void perspect(int *, int *, int);
int line2d(int x1, int y1, int x2, int y2, struct scankoord *f);
void rectangle2D(int x, int y, int szel, int mag, int szin, int keret);
void rectangle3d();
char getpixel(int x, int y);
void putpixel(int x, int y, char c);
void putpicture(char *src, int x, int y, int szel, int mag);
void getpicture(char *dest, int x, int y, int szel, int mag);
int test_screen_draw_speed(int lapok);
void teszt_szamolasi_sebesseg(void);
