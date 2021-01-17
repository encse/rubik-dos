#define BUSY 0
#define ARROW 1
typedef unsigned int word;
typedef unsigned char byte;
void cmouse(word *fnum, word *arg1, word *arg2, word *arg3);
int mouseinit(void);
void showcursor(void);
void hidecursor(void);
void mousepos(word *x, word *y, word *b);
void setmouse(word x, word y);
void mbuttom(void);
void mousegen(struct graphcursor *mask);
void mouse_status(int status);