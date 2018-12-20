#include<iostream>
#include<graphics.h>
#include<conio.h>
#include<wingdi.h>
using namespace std;

const char *img1 = "MainScreenMenu.jpg";
const char *img2 = "NumberOfPointsMenu.jpg";
const char *img3 = "SettingsMenu.jpg";
const char *img4 = "GameMode.jpg";
const char *img5 = "Color1.jpg";
const char *img6 = "Color2.jpg";

/// -----------------------------------------------SETTINGS MENU SELECTARE CULORI-----------------------------------------------------------///

int Player1Color()
{
    readimagefile(img5,0,0,800,600);

    setcolor(1);
    setfillstyle(SOLID_FILL,1);
    rectangle(0,0,120,120);
    floodfill(51,51,1);

    setcolor(2);
    setfillstyle(SOLID_FILL,2);
    rectangle(121,0,240,120);
    floodfill(122,51,2);

    setcolor(3);
    setfillstyle(SOLID_FILL,3);
    rectangle(241,0,360,120);
    floodfill(242,51,3);

    setcolor(4);
    setfillstyle(SOLID_FILL,4);
    rectangle(0,121,120,240);
    floodfill(1,122,4);

    setcolor(5);
    setfillstyle(SOLID_FILL,5);
    rectangle(121,121,240,240);
    floodfill(122,122,5);

    setcolor(6);
    setfillstyle(SOLID_FILL,6);
    rectangle(241,121,360,240);
    floodfill(242,132,6);

    setcolor(7);
    setfillstyle(SOLID_FILL,7);
    rectangle(0,241,120,360);
    floodfill(1,242,7);

    setcolor(8);
    setfillstyle(SOLID_FILL,8);
    rectangle(121,241,240,360);
    floodfill(122,242,8);

    setcolor(9);
    setfillstyle(SOLID_FILL,9);
    rectangle(241,241,360,360);
    floodfill(243,242,9);

    setcolor(10);
    setfillstyle(SOLID_FILL,10);
    rectangle(0,361,120,480);
    floodfill(1,362,10);

    setcolor(11);
    setfillstyle(SOLID_FILL,11);
    rectangle(121,361,240,480);
    floodfill(122,362,11);

    setcolor(12);
    setfillstyle(SOLID_FILL,12);
    rectangle(241,361,360,480);
    floodfill(242,362,12);

    setcolor(13);
    setfillstyle(SOLID_FILL,13);
    rectangle(0,481,120,600);
    floodfill(1,482,13);

    setcolor(14);
    setfillstyle(SOLID_FILL,14);
    rectangle(121,481,240,600);
    floodfill(122,482,14);

    setcolor(15);
    setfillstyle(SOLID_FILL,15);
    rectangle(241,481,360,600);
    floodfill(242,482,15);


}
int Player2Color()
{

    readimagefile(img6,0,0,800,600);

    setcolor(1);
    setfillstyle(SOLID_FILL,1);
    rectangle(0,0,120,120);
    floodfill(51,51,1);

    setcolor(2);
    setfillstyle(SOLID_FILL,2);
    rectangle(121,0,240,120);
    floodfill(122,51,2);

    setcolor(3);
    setfillstyle(SOLID_FILL,3);
    rectangle(241,0,360,120);
    floodfill(242,51,3);

    setcolor(4);
    setfillstyle(SOLID_FILL,4);
    rectangle(0,121,120,240);
    floodfill(1,122,4);

    setcolor(5);
    setfillstyle(SOLID_FILL,5);
    rectangle(121,121,240,240);
    floodfill(122,122,5);

    setcolor(6);
    setfillstyle(SOLID_FILL,6);
    rectangle(241,121,360,240);
    floodfill(242,132,6);

    setcolor(7);
    setfillstyle(SOLID_FILL,7);
    rectangle(0,241,120,360);
    floodfill(1,242,7);

    setcolor(8);
    setfillstyle(SOLID_FILL,8);
    rectangle(121,241,240,360);
    floodfill(122,242,8);

    setcolor(9);
    setfillstyle(SOLID_FILL,9);
    rectangle(241,241,360,360);
    floodfill(243,242,9);

    setcolor(10);
    setfillstyle(SOLID_FILL,10);
    rectangle(0,361,120,480);
    floodfill(1,362,10);

    setcolor(11);
    setfillstyle(SOLID_FILL,11);
    rectangle(121,361,240,480);
    floodfill(122,362,11);

    setcolor(12);
    setfillstyle(SOLID_FILL,12);
    rectangle(241,361,360,480);
    floodfill(242,362,12);

    setcolor(13);
    setfillstyle(SOLID_FILL,13);
    rectangle(0,481,120,600);
    floodfill(1,482,13);

    setcolor(14);
    setfillstyle(SOLID_FILL,14);
    rectangle(121,481,240,600);
    floodfill(122,482,14);

    setcolor(15);
    setfillstyle(SOLID_FILL,15);
    rectangle(241,481,360,600);
    floodfill(242,482,15);


}
void settingsPageWindow()
{
    readimagefile(img3,0,0,800,600);
}
void NumberOfPointsPage()
{
    settextstyle(3,HORIZ_DIR,2);

    readimagefile(img2,0,0,800,600);

    setbkcolor(8);

    setcolor(15);

    outtextxy(500,90,"  <  ");

    outtextxy(500,126,"  >  ");


}
void Menu()
 {
    readimagefile(img1,0,0,800,600);

 }
void GameType()
{
    readimagefile(img4,0,0,800,600);
}

void ConvertFromIntToString(char where[], int what) {
    int digits[10];

    memset(digits, 0, sizeof(digits));

    while(what) {
        digits[++digits[0]] = what % 10;
        what /= 10;
    }

    for(int i = digits[0]; i > 0; --i) {
        where[digits[0] - i] = digits[i] + '0';
    }

    where[digits[0]] = 0;
}
