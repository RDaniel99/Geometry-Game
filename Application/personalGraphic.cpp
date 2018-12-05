/// -----------------------------------------------SETTINGS MENU SELECTARE CULORI-----------------------------------------------------------///
void chooseColour()
{
    int gd = DETECT, gm;

    initwindow(800,600,"TestPicture");
    readimagefile("TestPicture.jpg",0,0,800,600);
    settextstyle(3,HORIZ_DIR,2);
    outtextxy(310, 100, "CHOSE COLOUR");
    outtextxy(320, 130, "FOR PLAYER 1");
    outtextxy(260, 115, "<----");
    outtextxy(310, 300, "CHOSE COLOUR");
    outtextxy(320, 330, "FOR PLAYER 2");
    outtextxy(470, 315, "---->");

    setcolor(1);
    setfillstyle(SOLID_FILL,1);
    rectangle(0,50,80,130);
    floodfill(51,51,1);

    setcolor(2);
    setfillstyle(SOLID_FILL,2);
    rectangle(81,50,160,130);
    floodfill(82,82,2);

    setcolor(3);
    setfillstyle(SOLID_FILL,3);
    rectangle(161,50,240,130);
    floodfill(162,51,3);

    setcolor(4);
    setfillstyle(SOLID_FILL,4);
    rectangle(0,131,80,210);
    floodfill(1,132,4);

    setcolor(5);
    setfillstyle(SOLID_FILL,5);
    rectangle(81,131,160,210);
    floodfill(82,132,5);

    setcolor(6);
    setfillstyle(SOLID_FILL,6);
    rectangle(161,131,240,210);
    floodfill(162,132,6);

    setcolor(7);
    setfillstyle(SOLID_FILL,7);
    rectangle(0,211,80,290);
    floodfill(1,212,7);

    setcolor(8);
    setfillstyle(SOLID_FILL,8);
    rectangle(81,211,160,290);
    floodfill(82,212,8);

    setcolor(9);
    setfillstyle(SOLID_FILL,9);
    rectangle(161,211,240,290);
    floodfill(162,212,9);

    setcolor(10);
    setfillstyle(SOLID_FILL,10);
    rectangle(0,291,80,370);
    floodfill(1,292,10);

    setcolor(11);
    setfillstyle(SOLID_FILL,11);
    rectangle(81,291,160,370);
    floodfill(82,292,11);

    setcolor(12);
    setfillstyle(SOLID_FILL,12);
    rectangle(161,291,240,370);
    floodfill(162,292,12);

    setcolor(13);
    setfillstyle(SOLID_FILL,13);
    rectangle(0,371,80,450);
    floodfill(1,372,13);

    setcolor(14);
    setfillstyle(SOLID_FILL,14);
    rectangle(81,371,160,450);
    floodfill(82,372,14);

    setcolor(15);
    setfillstyle(SOLID_FILL,15);
    rectangle(161,371,240,450);
    floodfill(162,372,15);


    setcolor(1);
    setfillstyle(SOLID_FILL,1);
    rectangle(560,50,640,130);
    floodfill(561,51,1);

    setcolor(2);
    setfillstyle(SOLID_FILL,2);
    rectangle(641,50,720,130);
    floodfill(642,51,2);

    setcolor(3);
    setfillstyle(SOLID_FILL,3);
    rectangle(721,50,800,130);
    floodfill(722,51,3);

    setcolor(4);
    setfillstyle(SOLID_FILL,4);
    rectangle(560,131,640,210);
    floodfill(561,132,4);

    setcolor(5);
    setfillstyle(SOLID_FILL,5);
    rectangle(641,131,720,210);
    floodfill(642,132,5);

    setcolor(6);
    setfillstyle(SOLID_FILL,6);
    rectangle(721,131,800,210);
    floodfill(722,132,6);

    setcolor(7);
    setfillstyle(SOLID_FILL,7);
    rectangle(560,211,640,290);
    floodfill(561,212,7);

    setcolor(8);
    setfillstyle(SOLID_FILL,8);
    rectangle(641,211,720,290);
    floodfill(642,212,8);

    setcolor(9);
    setfillstyle(SOLID_FILL,9);
    rectangle(721,211,800,290);
    floodfill(722,212,9);

    setcolor(10);
    setfillstyle(SOLID_FILL,10);
    rectangle(560,291,640,370);
    floodfill(561,292,10);

    setcolor(11);
    setfillstyle(SOLID_FILL,11);
    rectangle(641,291,720,370);
    floodfill(642,292,11);

    setcolor(12);
    setfillstyle(SOLID_FILL,12);
    rectangle(721,291,800,370);
    floodfill(722,292,12);

    setcolor(13);
    setfillstyle(SOLID_FILL,13);
    rectangle(560,371,640,450);
    floodfill(561,372,13);

    setcolor(14);
    setfillstyle(SOLID_FILL,14);
    rectangle(641,371,720,450);
    floodfill(642,372,14);

    setcolor(15);
    setfillstyle(SOLID_FILL,15);
    rectangle(721,371,800,450);
    floodfill(722,372,15);

    getch();
}
