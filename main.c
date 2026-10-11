#include <pspkernel.h>
#include <pspctrl.h>
#include <pspdisplay.h>
#include <string.h>
#include "font.h"

PSP_MODULE_INFO("PSP Auto", 0, 0, 2);
PSP_MAIN_THREAD_ATTR(THREAD_ATTR_USER);
#define W 480
#define H 272
#define PITCH 512
#define RGB(r,g,b) (0xff000000u | ((unsigned)(b)<<16) | ((unsigned)(g)<<8) | (unsigned)(r))
#define BG RGB(12,19,30)
#define PANEL RGB(26,39,55)
#define PANEL2 RGB(36,53,73)
#define WHITE RGB(240,246,251)
#define MUTED RGB(164,185,203)
#define BLUE RGB(83,177,244)
#define GREEN RGB(100,218,165)
#define ORANGE RGB(255,192,108)
static unsigned int *fb = (unsigned int*)0x04000000;
static volatile int running = 1;
static int selected = 0; /* 0..3 */
static int page = -1; /* -1 home, 0..3 details */
static int playing = 0;
static int theme = 0;
static const char *names[] = {"NAVIGATION", "MUSIC", "PHONE", "SETTINGS"};

static int exit_cb(int a,int b,void *c){(void)a;(void)b;(void)c;running=0;return 0;}
static int callbacks(SceSize a, void *b){(void)a;(void)b;int id=sceKernelCreateCallback("exit",exit_cb,0);sceKernelRegisterExitCallback(id);sceKernelSleepThreadCB();return 0;}
static void start_callbacks(void){int id=sceKernelCreateThread("callbacks",callbacks,0x11,0x1000,0,0);if(id>=0)sceKernelStartThread(id,0,NULL);}

static void rect(int x,int y,int w,int h,unsigned int color){
 int j,i,x2=x+w,y2=y;
 if(x<0)x=0;if(y<0)y=0;if(x2>W)x2=W;if(y2>H)y2=H;
 for(j=y;j<y2;j++)for(i=x;i<x2;i++)fb[j*PITCH+i]=color;
}
static void dot(int x,int y,unsigned int color){if(x>=0&&x<W&&y>=0&&y<H)fb[y*PITCH+x]=color;}
static void line(int x0,int y0,int x1,int y1,unsigned int col){
 int dx=x1>x0?x1-x0:x0-x1, sx=x0<x1?1:-1;
 int dy=-(y1>y0?y1-y0:y0-y1), sy=y0<y1?1:-1,err=dx+dy;
 while(1){int e;dot(x0,y0,col);if(x0==x1&&y0==y1)break;e=2*err;if(e>=dy){err+=dy;x0+=sx;}if(e<=dx){err+=dx;y0+=sy;}}
}
static void label(int x,int y,const char *s,unsigned int color,int scale){
 int i,j,k,bit;unsigned char ch;
 while(*s && x<W-8*scale){ch=(unsigned char)*s++;if(ch<32||ch>126)ch='?';
 for(j=0;j<12;j++)for(i=0;i<8;i++){bit=(font8x12[ch-32][j]>>(7-i))&1;if(bit)for(k=0;k<scale;k++)rect(x+i*scale,y+j*scale+k,scale,1,color);}
 x+=8*scale;}
}
static void border(int x,int y,int w,int h,unsigned int c){rect(x,y,w,2,c);rect(x,y+h-2,w,2,c);rect(x,y,2,h,c);rect(x+w-2,y,2,h,c);}
static void arrow(int x,int y,unsigned int col){line(x,y+12,x+14,y-2,col);line(x+14,y-2,x+28,y+12,col);line(x+14,y-2,x+14,y+25,col);}
static void musicicon(int x,int y,unsigned int col){rect(x+17,y,4,26,col);rect(x+21,y,14,4,col);rect(x+31,y+2,4,9,col);rect(x+7,y+24,15,8,col);rect(x+26,y+21,15,8,col);}
static void battery(int x,int y,unsigned int col){border(x,y,28,14,col);rect(x+28,y+4,3,6,col);rect(x+4,y+4,18,6,col);}
static void phoneicon(int x,int y,unsigned int col){border(x,y,20,31,col);rect(x+7,y+25,6,2,col);}
static void cog(int x,int y,unsigned int col){border(x,y,25,25,col);rect(x+9,y+9,7,7,col);rect(x+10,y-4,5,8,col);rect(x+10,y+21,5,8,col);rect(x-4,y+10,8,5,col);rect(x+21,y+10,8,5,col);}
static void navart(int x,int y){
 int i;
 rect(x,y,192,121,RGB(25,54,65));
 for(i=0;i<4;i++){line(x+i*49,y,x+i*49-41,y+120,RGB(43,83,90));}
 for(i=0;i<3;i++){line(x,y+15+i*49,x+191,y+41+i*49,RGB(43,83,90));}
 line(x+50,y+101,x+93,y+77,RGB(103,177,229));line(x+93,y+77,x+93,y+35,RGB(103,177,229));
 line(x+93,y+35,x+145,y+35,RGB(103,177,229));
 rect(x+48,y+98,6,6,WHITE);arrow(x+132,y+29,GREEN);
}
static void card(int x,int y,int w,int h,int isselected){rect(x,y,w,h,PANEL);if(isselected)border(x,y,w,h,BLUE);}
static void header(void){
 rect(0,0,W,45,RGB(17,27,41));rect(0,44,W,1,RGB(39,61,82));
 label(18,13,"PSP",WHITE,2);label(70,13,"AUTO",BLUE,2);
 label(192,17,"v0.2  OFFLINE DEMO",MUTED,1);
 rect(402,13,9,9,ORANGE);label(418,13,"OFF",MUTED,1);battery(445,29,GREEN);
}
static void footer(const char *msg){rect(0,246,W,26,RGB(17,27,41));label(16,253,msg,MUTED,1);}
static void home(void){
 header();label(20,56,"DASHBOARD",WHITE,1);
 card(18,77,218,157,selected==0);card(244,77,218,75,selected==1);card(244,160,105,74,selected==2);card(357,160,105,74,selected==3);
 navart(31,102);label(31,86,"NAVIGATION",BLUE,1);
 label(39,215,"NO ROUTE SET",WHITE,1);
 label(258,88,"MUSIC",GREEN,1);musicicon(257,108,GREEN);
 label(309,113,"NO TRACK",WHITE,1);label(309,128,"CONNECTED",MUTED,1);
 label(257,170,"PHONE",ORANGE,1);phoneicon(261,190,ORANGE);
 label(370,170,"SETTINGS",BLUE,1);cog(384,194,BLUE);
 footer("D-PAD: CHOOSE     X: OPEN     START: EXIT");
}
static void back_header(const char *name){header();label(20,57,name,WHITE,2);}
static void navigation(void){
 back_header("NAVIGATION");
 rect(18,94,283,141,PANEL);navart(29,104);
 rect(315,94,147,141,PANEL);label(328,109,"NEXT TURN",BLUE,1);
 arrow(363,138,BLUE);label(331,180,"AWAITING",WHITE,1);label(331,196,"PHONE LINK",WHITE,1);
 label(34,223,"MAP DEMO",MUTED,1);footer("CIRCLE: BACK     START: EXIT");
}
static void music(void){
 back_header("MUSIC");rect(18,94,142,141,PANEL2);musicicon(57,135,GREEN);
 rect(173,94,289,141,PANEL);label(187,111,"NOW PLAYING",GREEN,1);
 label(187,138,"NO MEDIA CONNECTED",WHITE,1);
 label(187,158,"PHONE BRIDGE COMING SOON",MUTED,1);
 label(187,199,playing?"[ PAUSE ]":"[ PLAY ]",GREEN,1);
 label(283,199,"< PREV    NEXT >",MUTED,1);
 footer("X: TOGGLE DEMO PLAY    CIRCLE: BACK");
}
static void phone(void){
 back_header("PHONE");rect(18,94,444,141,PANEL);
 phoneicon(45,119,ORANGE);label(93,108,"COMPANION DEVICE",ORANGE,1);
 label(93,139,"SAMSUNG ANDROID",WHITE,2);
 label(93,174,"STATUS: NOT CONNECTED",MUTED,1);
 label(93,195,"ANDROID BRIDGE PLANNED FOR v0.3",MUTED,1);
 footer("CIRCLE: BACK     START: EXIT");
}
static void settings(void){
 back_header("SETTINGS");rect(18,94,444,141,PANEL);
 label(36,110,"PSP AUTO 0.2",WHITE,2);
 label(36,149,"NETWORK: OFFLINE",MUTED,1);
 label(36,170,"MODE: GRAPHICAL DEMONSTRATION",MUTED,1);
 label(36,196,theme?"THEME ACCENT: GREEN":"THEME ACCENT: BLUE",BLUE,1);
 footer("X: CHANGE ACCENT LABEL   CIRCLE: BACK");
}
static void render(void){
 rect(0,0,W,H,BG);
 if(page<0)home();else if(page==0)navigation();else if(page==1)music();else if(page==2)phone();else settings();
 sceKernelDcacheWritebackAll();
}
int main(void){
 SceCtrlData pad;unsigned int previous=0;
 start_callbacks();sceCtrlSetSamplingCycle(0);sceCtrlSetSamplingMode(PSP_CTRL_MODE_DIGITAL);
 sceDisplaySetMode(0,W,H);
 sceDisplaySetFrameBuf((void*)0x04000000,PITCH,PSP_DISPLAY_PIXEL_FORMAT_8888,PSP_DISPLAY_SETBUF_IMMEDIATE);
 render();
 while(running){unsigned int pressed;
 sceCtrlPeekBufferPositive(&pad,1);pressed=pad.Buttons & ~previous;
 if(pressed & PSP_CTRL_START)break;
 if(page<0){
 if(pressed & PSP_CTRL_LEFT)selected=(selected+3)%4;
 if(pressed & PSP_CTRL_RIGHT)selected=(selected+1)%4;
 if(pressed & PSP_CTRL_UP)selected=(selected+3)%4;
 if(pressed & PSP_CTRL_DOWN)selected=(selected+1)%4;
 if(pressed & PSP_CTRL_CROSS)page=selected;
 }else{
 if(pressed & PSP_CTRL_CIRCLE)page=-1;
 if((pressed & PSP_CTRL_CROSS)&&page==1)playing=!playing;
 if((pressed & PSP_CTRL_CROSS)&&page==3)theme=!theme;
 }
 if(pressed)render();previous=pad.Buttons;sceDisplayWaitVblankStart();}
 sceKernelExitGame();return 0;
}
