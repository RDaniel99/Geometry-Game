#include <iostream>
#include <graphics.h>
#include <winbgim.h>
#include <ctime>
#include <fstream>
#include <chrono>
#include <thread>
#include "personalGraphic.h"

using namespace std;

#define MAX_POINTS          90
#define MAX_NAME            25
#define DEFAULT_HEIGHT      600
#define DEFAULT_WIDTH       800
#define TIME_BETWEEN_MOVES  500

///---------------- Pages Variables ----------------------------------------------------------------
int mainPage                 = -1;
int chooseColorPlayer1Page   = -1;
int chooseColorPlayer2Page   = -1;
int chooseGameTypePage       = -1;
int chooseNumberOfPointsPage = -1;
int settingsPage             = -1;
int chooseFirstToWinPage     = -1;
int gamePage                 = -1;
int howtoplay                = -1;
int EndOfGame                = -1;
///-------------------------------------------------------------------------------------------------
///---------------- Geometry structs ---------------------------------------------------------------
struct CPoint
{
    int x, y;
};
struct CSegment
{
    CPoint A, B;
};
struct CCircle
{
    CPoint center;
    int radius;
};
///-------------------------------------------------------------------------------------------------

///---------------- Settings struct of the Game ----------------------------------------------------
struct CSettings
{
    int numberOfPoints;

    int colorOfPlayer1,    // [1, 14]
        colorOfPlayer2;    // [1, 14]

    int  botLevel        ; // 1 - easy, 2 - hard
    bool isPlayingWithBot; // 1 - yes , 0 - no
    int  firstToWin      ;
};
///-------------------------------------------------------------------------------------------------

///---------------- Table game struct --------------------------------------------------------------
struct CTable
{
    CPoint   points[MAX_POINTS]     ;
    CSegment segments[MAX_POINTS]   ;
    bool     isSelected[MAX_POINTS] ; // 'isSelected[i] = true' only when 'i' is already in a segment

    int numberOfSegments,
        windowHeight,
        windowWidth,
        firstWinnings,
        secondWinnings,
        radiusPoints = 4,
        gameColor    = 0;

    CSettings settings  ;
};
///-------------------------------------------------------------------------------------------------

///---------------- Game Engine Functions ----------------------------------------------------------
void    StartGame(CTable &table)                ;
bool    TheGameIsOver(CTable &table)            ;
void    GenerateNRandomPoints(CTable &table)    ;
int     CheckWhatPointIsClicked(CTable &table)  ;
void    PaintPoints(CTable &table)              ;
void    SetupTable(CTable &table)               ;
void    PaintLinePts(CTable &table,
                     int &pIndex1, int &pIndex2);
int     ChooseMoveBotHard(CTable &table)        ;
int     ChooseMoveBotEasy(CTable &table,
                          int &whichOne)        ;
int     GetMove(CTable &table,
                int &playerTurn, int &helper)   ;
void    ColorPoint(int colorP, CTable &table)   ;

///-------------------------------------------------------------------------------------------------

///---------------- Geometry Functions -------------------------------------------------------------
bool    IsPointOnSegment(CSegment &segment, CPoint &point)    ;
bool    SegmentsAreIntersecting(CSegment &s1, CSegment &s2)   ;
int     ComputeOrientation(CPoint &A, CPoint &B, CPoint &C)   ;
bool    CheckCirclesIntersection(CCircle &c1, CCircle &c2)    ;
int     CalculateSqDistanceBetweenPoints(CPoint &A, CPoint &B);
bool    CheckIfSegmentCanBePlaced(CTable &table,
                                  int &firstPointIndex,
                                  int &secondPointIndex)      ;
///-------------------------------------------------------------------------------------------------

///---------------- CSettings Functions ----------------------------------------------------------
void    SetNumberOfPoints(CTable &table, int  newN)     ;
void    SetGameWithBot   (CTable &table, bool status)   ;
void    SetFirstToWin    (CTable &table, int  firstToW) ;
void    SetBotLevel      (CTable &table, int  botLevel) ;
void    SaveSettings     (CTable &table               ) ;
void    ReadSettings     (CSettings &tempSettings     ) ;
void    SetColorPlayer1  (CTable &table, int color    ) ;
void    SetColorPlayer2  (CTable &table, int color    ) ;
///-------------------------------------------------------------------------------------------------

///---------------- Main Function ------------------------------------------------------------------
int main()
{
    int CurrentLevel;
    char intAsString [10];

    mainPage = initwindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, "Segments Game");
    setcurrentwindow(mainPage);
    Menu();

    CTable table;
    CSettings tempSettings;
    ReadSettings(tempSettings);

    table.settings = tempSettings;

    table.windowHeight = DEFAULT_HEIGHT;
    table.windowWidth  = DEFAULT_WIDTH ;

    if(table.settings.isPlayingWithBot == false) {
        CurrentLevel = 1;
    }
    else {
        CurrentLevel = table.settings.botLevel + 1;
    }

    while(true)
    {
        SaveSettings(table);
        int x, y;
        getmouseclick(WM_LBUTTONDOWN, x, y);

        if(x == -1 && y == -1) continue;

        if(x >= 40 && x <= 360 && y >= 380 && y <= 460 && getcurrentwindow() == mainPage)
        {
            int currentWindow = getcurrentwindow();
            howtoplay = initwindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, "How To Play");
            setcurrentwindow(howtoplay);
            closegraph(currentWindow);
            HowToPlay();
        }
        if(x >= 630 && x <= 750 && y >= 540 && y <= 580 && getcurrentwindow() == howtoplay)
        {
            int currentWindow = getcurrentwindow();
            mainPage = initwindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, "Segments Game");
            setcurrentwindow(mainPage);
            closegraph(currentWindow);
            Menu();
        }
        if(x >= 40 && x <= 380 && y >= 460 && y <= 510 && getcurrentwindow() == mainPage)
        {
            int currentWindow = getcurrentwindow();
            settingsPage = initwindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, "Settings Page");
            setcurrentwindow(settingsPage);
            closegraph(currentWindow);
            settingsPageWindow();
            continue;
        }

        if(x >= 40 && x <= 160 && y >= 520 && y <= 560 && getcurrentwindow() == mainPage)
        {
            int currentWindow = getcurrentwindow();
            gamePage = initwindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, "Game Play");
            setcurrentwindow(gamePage);
            closegraph(currentWindow);
            StartGame(table);
            continue;
        }

        if(x >= 70 && x <= 350 && y >= 120 && y <= 160 && getcurrentwindow() == settingsPage)
        {
            int currentWindow = getcurrentwindow();
            chooseColorPlayer1Page  = initwindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, "Color for player 1");
            setcurrentwindow(chooseColorPlayer1Page);
            closegraph(currentWindow);
            Player1Color();
            setcolor(table.settings.colorOfPlayer1);
            setfillstyle(SOLID_FILL,table.settings.colorOfPlayer1);
            rectangle(521,341,640,460);
            floodfill(522,342,table.settings.colorOfPlayer1);
            continue;
        }

        if(x >= 70 && x <= 380 && y >= 180 && y <= 220 && getcurrentwindow() == settingsPage)
        {
            int currentWindow = getcurrentwindow();
            chooseNumberOfPointsPage = initwindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, "Number of points");
            setcurrentwindow(chooseNumberOfPointsPage);
            closegraph(currentWindow);
            NumberOfPointsPage();
            ConvertFromIntToString(intAsString, table.settings.numberOfPoints);
            outtextxy(450, 108, intAsString);
            continue;
        }

        if(x >= 70 && x <= 330 && y >= 240 && y <= 280 && getcurrentwindow() == settingsPage)
        {
            int currentWindow = getcurrentwindow();
            chooseGameTypePage = initwindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, "Game Type");
            setcurrentwindow(chooseGameTypePage);
            closegraph(currentWindow);
            GameType();
            if(CurrentLevel == 1)
            {
                setcolor(12);
            setfillstyle(SOLID_FILL,12);
            rectangle(180,260,620,265);
            floodfill(181,261,12);
            }
            if(CurrentLevel == 2)
            {
            setcolor(12);
            setfillstyle(SOLID_FILL,12);
            rectangle(80,320,720,325);
            floodfill(81,321,12);
            }
            if(CurrentLevel == 3)
            {
            setcolor(12);
            setfillstyle(SOLID_FILL,12);
            rectangle(80,380,720,385);
            floodfill(81,381,12);
            }
            continue;
        }

        if(x >= 70 && x <= 330 && y >= 300 && y <= 340 && getcurrentwindow() == settingsPage)
        {
            int currentWindow = getcurrentwindow();
            mainPage = initwindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, "Segments Game");
            setcurrentwindow(mainPage);
            closegraph(currentWindow);
            Menu();
        }

        if(x >= 180 && x <= 620 && y >= 210 && y <= 260 && getcurrentwindow() == chooseGameTypePage)
        {
            CurrentLevel = 1;

            SetGameWithBot(table, false);

            setcolor(12);
            setfillstyle(SOLID_FILL,12);
            rectangle(180,260,620,265);
            floodfill(181,261,12);

            setcolor(15);
            setfillstyle(SOLID_FILL,15);
            rectangle(80,320,720,325);
            floodfill(81,321,15);

            setcolor(15);
            setfillstyle(SOLID_FILL,15);
            rectangle(80,380,720,385);
            floodfill(81,381,15);

            continue;
        }

        if(x >= 80 && x <= 720 && y >= 280 && y <= 320 && getcurrentwindow() == chooseGameTypePage)
        {
            CurrentLevel = 2;

            SetGameWithBot(table, true);
            SetBotLevel(table, 1);

            setcolor(12);
            setfillstyle(SOLID_FILL,12);
            rectangle(80,320,720,325);
            floodfill(81,321,12);

            setcolor(15);
            setfillstyle(SOLID_FILL,15);
            rectangle(180,260,620,265);
            floodfill(181,261,15);

            setcolor(15);
            setfillstyle(SOLID_FILL,15);
            rectangle(80,380,720,385);
            floodfill(81,381,15);

            continue;
        }

        if(x >= 80 && x <= 720 && y >= 340 && y <= 380 && getcurrentwindow() == chooseGameTypePage)
        {
            CurrentLevel = 3;

            SetGameWithBot(table, true);
            SetBotLevel(table, 2);

            setcolor(12);
            setfillstyle(SOLID_FILL,12);
            rectangle(80,380,720,385);
            floodfill(81,381,12);

            setcolor(15);
            setfillstyle(SOLID_FILL,15);
            rectangle(180,260,620,265);
            floodfill(181,261,15);

            setcolor(15);
            setfillstyle(SOLID_FILL,15);
            rectangle(80,320,720,325);
            floodfill(81,321,15);

            continue;
        }

        if(x >= 230 && x <= 570 && y >= 510 && y <= 560 && getcurrentwindow() == chooseGameTypePage)
        {
            int currentWindow = getcurrentwindow();
            mainPage = initwindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, "Segments Game");
            setcurrentwindow(mainPage);
            closegraph(currentWindow);
            Menu();
            continue;
        }

        if(x >= 500 && x <= 530 && y >= 126 && y <= 156 && getcurrentwindow() == chooseNumberOfPointsPage)
        {
            int tempPointsNumber = table.settings.numberOfPoints;

            tempPointsNumber = tempPointsNumber + 10;

            if(tempPointsNumber > 90)
            {
                    tempPointsNumber = 10;
            }

            ConvertFromIntToString(intAsString, tempPointsNumber);
            outtextxy(450, 108, intAsString);
            SetNumberOfPoints(table, tempPointsNumber);
            continue;
        }

        if(x >= 500 && x <= 530 && y >= 90 && y <= 120 && getcurrentwindow() == chooseNumberOfPointsPage)
        {
            int tempPointsNumber = table.settings.numberOfPoints;

            tempPointsNumber = tempPointsNumber - 10;

            if(tempPointsNumber < 10)
            {
                    tempPointsNumber = 90;
            }

            ConvertFromIntToString(intAsString, tempPointsNumber);
            outtextxy(450, 108, intAsString);
            SetNumberOfPoints(table, tempPointsNumber);
            continue;
        }

        if(x >= 70 && x <= 310 && y >= 150 && y <= 200 && getcurrentwindow() == chooseNumberOfPointsPage)
        {
            int currentWindow = getcurrentwindow();
            settingsPage = initwindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, "Settings Page");
            setcurrentwindow(settingsPage);
            closegraph(currentWindow);
            settingsPageWindow();
            continue;
        }

        if(getcurrentwindow() == chooseColorPlayer1Page)
        {

            if(getpixel(x, y) >= 1  &&
               getpixel(x, y) <= 14 &&
               getpixel(x, y) != table.settings.colorOfPlayer2)
            {
                SetColorPlayer1(table, getpixel(x, y));
                setcolor(table.settings.colorOfPlayer1);
                setfillstyle(SOLID_FILL,table.settings.colorOfPlayer1);
                rectangle(521,341,640,460);
                floodfill(522,342,table.settings.colorOfPlayer1);
                continue;
            }

            if(x >= 460 && x <= 550 && y >= 160 && y <= 190)
            {
                int currentWindow = getcurrentwindow();
                chooseColorPlayer2Page  = initwindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, "Color for player 2");
                setcurrentwindow(chooseColorPlayer2Page);
                closegraph(currentWindow);
                Player2Color();
                setcolor(table.settings.colorOfPlayer2);
                setfillstyle(SOLID_FILL,table.settings.colorOfPlayer2);
                rectangle(521,341,640,460);
                floodfill(522,342,table.settings.colorOfPlayer2);
                continue;
            }

            if(x >= 460 && x <= 770 && y >= 220 && y <= 260)
            {
                int currentWindow = getcurrentwindow();
                settingsPage = initwindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, "Settings Page");
                setcurrentwindow(settingsPage);
                closegraph(currentWindow);
                settingsPageWindow();
                continue;
            }
        }

       if(getcurrentwindow() == chooseColorPlayer2Page)
       {
           if(getpixel(x, y) >= 1  &&
              getpixel(x, y) <= 14 &&
              getpixel(x, y) != table.settings.colorOfPlayer1)
            {
                SetColorPlayer2(table, getpixel(x, y));
                setcolor(table.settings.colorOfPlayer2);
                setfillstyle(SOLID_FILL,table.settings.colorOfPlayer2);
                rectangle(521,341,640,460);
                floodfill(522,342,table.settings.colorOfPlayer2);

                continue;
            }

           if(x >= 460 && x <= 770 && y >= 220 && y <= 260)
            {
                int currentWindow = getcurrentwindow();
                settingsPage = initwindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, "Settings Page");
                setcurrentwindow(settingsPage);
                closegraph(currentWindow);
                settingsPageWindow();
                continue;
            }
       }
    }


    getch();
    closegraph();

    return 0;
}
///-------------------------------------------------------------------------------------------------

///---------------- Color a point ------------------------------------------------------------------

void ColorPoint(int colorP, int pInd, CTable &table)
{
    setcolor(colorP);

    circle(table.points[pInd].x, table.points[pInd].y, table.radiusPoints);

    setfillstyle(SOLID_FILL, ColorP);

    floodfill(table.points[pInd].x, table.points[pInd].y, ColorP);
}
///-------------------------------------------------------------------------------------------------

///---------------- Generator of N Random Points ---------------------------------------------------
void GenerateNRandomPoints(CTable &table)
{
    srand(time(NULL));

    for(int pointIndex = 1; pointIndex <= table.settings.numberOfPoints; ++pointIndex)
    {
        int xCoordinate = rand() % table.windowWidth ;
        int yCoordinate = rand() % table.windowHeight;

        CPoint newPoint;

        newPoint.x = xCoordinate;
        newPoint.y = yCoordinate;

        CCircle c1;
        c1.center = newPoint;
        c1.radius = table.radiusPoints;

        int minX = c1.center.x - 3 * c1.radius;
        int maxX = c1.center.x + 3 * c1.radius;
        int minY = c1.center.y - 3 * c1.radius;
        int maxY = c1.center.y + 3 * c1.radius;

        if(minX > 0 && maxX < table.windowWidth && minY > 0 && maxY < table.windowHeight)
        {
            bool flag = false;
            for(int i = 1; i < pointIndex; ++i)
            {
                CCircle c2;
                c2.center = table.points[i];
                c2.radius = table.radiusPoints;

                if(CheckCirclesIntersection(c1, c2))
                {
                    flag = true;
                    break;
                }
            }

            if(flag)
            {
                --pointIndex;
            }
            else
            {
                table.points[pointIndex] = newPoint;
            }
        }
        else
        {
            --pointIndex;
        }
    }
}
///-------------------------------------------------------------------------------------------------

///---------------- Check if a segment can be placed -----------------------------------------------
bool CheckIfSegmentCanBePlaced(CTable &table, int &firstPointIndex, int &secondPointIndex)
{
    if(firstPointIndex == secondPointIndex)
        return false;
    if(table.isSelected[firstPointIndex]  )
        return false;
    if(table.isSelected[secondPointIndex] )
        return false;

    CSegment segmentToCheck;
    segmentToCheck.A = table.points[firstPointIndex] ;
    segmentToCheck.B = table.points[secondPointIndex];

    for(int segmentIndex = 1; segmentIndex <= table.numberOfSegments; ++segmentIndex)
    {
        if(SegmentsAreIntersecting(segmentToCheck, table.segments[segmentIndex]) == true)
        {
            return false;
        }
    }

    for(int circleIndex = 1; circleIndex <= table.settings.numberOfPoints; ++circleIndex)
    {
        if(circleIndex == firstPointIndex || circleIndex == secondPointIndex || !IsPointOnSegment(segmentToCheck, table.points[circleIndex]))
        {
            continue;
        }

        int xMin = table.points[circleIndex].x - table.radiusPoints;
        int xMax = table.points[circleIndex].x + table.radiusPoints;
        int yMin = table.points[circleIndex].y - table.radiusPoints;
        int yMax = table.points[circleIndex].y + table.radiusPoints;

        CPoint P1, P2, P3, P4;
        P1.x = xMin, P1.y = yMin;
        P2.x = xMin, P2.y = yMax;
        P3.x = xMax, P3.y = yMin;
        P4.x = xMax, P4.y = yMax;

        int o1 = ComputeOrientation(table.points[firstPointIndex], table.points[secondPointIndex], P1);
        int o2 = ComputeOrientation(table.points[firstPointIndex], table.points[secondPointIndex], P2);
        int o3 = ComputeOrientation(table.points[firstPointIndex], table.points[secondPointIndex], P3);
        int o4 = ComputeOrientation(table.points[firstPointIndex], table.points[secondPointIndex], P4);

        if(!(o1 == o2 && o2 == o3 && o3 == o4 && o4 != 0))
        {
            cout << "intersects " << circleIndex << ' ' << table.points[circleIndex].x << ' ' << table.points[circleIndex].y << '\n';
            return false;
        }
    }

    return true;
}
///-------------------------------------------------------------------------------------------------

///---------------- Check if the game is over ------------------------------------------------------
bool TheGameIsOver(CTable &table)
{
    for(int firstPoint = 1; firstPoint <= table.settings.numberOfPoints; ++firstPoint)
    {

        if(!table.isSelected[firstPoint])
        {

            for(int secondPoint = firstPoint + 1; secondPoint <= table.settings.numberOfPoints; ++secondPoint)
            {

                if(!table.isSelected[secondPoint])
                {
                    if(CheckIfSegmentCanBePlaced(table, firstPoint, secondPoint))
                    {
                        return false;
                    }
                }
            }
        }
    }

    return true;
}
///-------------------------------------------------------------------------------------------------

///---------------- Check if two given segments are intersecting -----------------------------------
bool SegmentsAreIntersecting(CSegment &s1, CSegment &s2)
{
    int o1 = ComputeOrientation(s1.A, s1.B, s2.A);
    int o2 = ComputeOrientation(s1.A, s1.B, s2.B);
    int o3 = ComputeOrientation(s2.A, s2.B, s1.A);
    int o4 = ComputeOrientation(s2.A, s2.B, s1.B);

    if(o1 != o2 && o3 != o4)
        return true;

    if(o1 == 0 && IsPointOnSegment(s1, s2.A) == true)
        return true;
    if(o2 == 0 && IsPointOnSegment(s1, s2.B) == true)
        return true;
    if(o3 == 0 && IsPointOnSegment(s2, s1.A) == true)
        return true;
    if(o4 == 0 && IsPointOnSegment(s2, s1.B) == true)
        return true;

    return false;
}
///-------------------------------------------------------------------------------------------------

///---------------- Check if a given point is on a given segment -----------------------------------
bool IsPointOnSegment(CSegment &segment, CPoint &point)
{
    if(max(segment.A.x, segment.B.x) >= point.x &&
            min(segment.A.x, segment.B.x) <= point.x &&
            max(segment.A.y, segment.B.y) >= point.y &&
            min(segment.A.y, segment.B.y) <= point.y)
        return true;

    return false;
}
///-------------------------------------------------------------------------------------------------

///---------------- Check if point C is on the right/left side of segment [AB] ---------------------
int ComputeOrientation(CPoint &A, CPoint &B, CPoint &C)
{
    int rez = (B.y - A.y) * (C.x - B.x) -
              (B.x - A.x) * (C.y - B.y) ;

    if(!rez)
        return 0;

    return rez / abs(rez);
}
///-------------------------------------------------------------------------------------------------

///---------------- Set number of points on the table ----------------------------------------------
void SetNumberOfPoints(CTable &table, int newN)
{
    table.settings.numberOfPoints = newN;
}
///---------------- Set Color Of Player 1 ----------------------------------------------------------
void SetColorPlayer1(CTable &table, int newC1)
{
    table.settings.colorOfPlayer1 = newC1;
}
///-------------------------------------------------------------------------------------------------

///---------------- Set Color Of Player 2 ----------------------------------------------------------
void SetColorPlayer2(CTable &table, int newC2)
{
    table.settings.colorOfPlayer2 = newC2;
}
///-------------------------------------------------------------------------------------------------

///---------------- Set if user is playing with BOT ------------------------------------------------
void SetGameWithBot(CTable &table, bool status)
{
    table.settings.isPlayingWithBot = status;
}
///-------------------------------------------------------------------------------------------------

///---------------- Set the winning score ----------------------------------------------------------
void SetFirstToWin(CTable &table, int firstToW)
{
    table.settings.firstToWin = firstToW;
}
///-------------------------------------------------------------------------------------------------

///---------------- Set the bot level --------------------------------------------------------------
void SetBotLevel(CTable &table, int botLevel)
{
    table.settings.botLevel = botLevel;
}
///-------------------------------------------------------------------------------------------------

///---------------- Paint Points On The Table ------------------------------------------------------
void PaintPoints(CTable &table)
{
    for(int pInd = 1; pInd <= table.settings.numberOfPoints; pInd++)
    {

        fillellipse(table.points[pInd].x, table.points[pInd].y, table.radiusPoints, table.radiusPoints);
    }
}
///---------------- Initialize Table ---------------------------------------------------------------
void SetupTable(CTable &table)
{
    table.numberOfSegments  = 0;

    memset(table.points,     0, sizeof(table.points))    ;
    memset(table.segments,   0, sizeof(table.segments))  ;
    memset(table.isSelected, 0, sizeof(table.isSelected));
}
///-------------------------------------------------------------------------------------------------

///---------------- Engine of the game -------------------------------------------------------------
void StartGame(CTable &table)
{
    int current = 1;
    table.firstWinnings = 0;
    table.secondWinnings = 0;
    do
    {

        cleardevice();
        SetupTable(table);
        GenerateNRandomPoints(table);
        PaintPoints(table);
        while(true) {
            int idxP = CheckWhatPointIsClicked(table);

            if(idxP == -1) {
                continue;
            }

            cout << idxP << '\n';

            ColorPoint(RED, idxP, table);
        }
        setcolor(table.settings.colorOfPlayer1);

        int playerToMove = 0; // 0 - first, 1 - second

        while(!TheGameIsOver(table))
        {

            int firstPointIndex  = -1,
                secondPointIndex = -1;

            while(secondPointIndex < 0)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(TIME_BETWEEN_MOVES));
                int x = GetMove(table, playerToMove, firstPointIndex);
                if(x != -1)
                {
                    if(table.isSelected[x] == false)
                    {
                        if(firstPointIndex == -1)
                        {
                            firstPointIndex = x;
                            fillellipse(table.points[firstPointIndex].x, table.points[firstPointIndex].y, table.radiusPoints, table.radiusPoints);
                        }
                        else
                        {
                            secondPointIndex = x;
                            fillellipse(table.points[secondPointIndex].x, table.points[secondPointIndex].y, table.radiusPoints, table.radiusPoints);
                        }
                    }
                }
            }

            if(CheckIfSegmentCanBePlaced(table, firstPointIndex, secondPointIndex) == true)
            {
                PaintLinePts(table, firstPointIndex, secondPointIndex);
                playerToMove = 1 - playerToMove;
                if(current == 1)
                {
                    current = 2;
                    setcolor(table.settings.colorOfPlayer2);
                }
                else
                {
                    current = 1;
                    setcolor(table.settings.colorOfPlayer1);
                }

            }
            else
            {
                fillellipse(table.points[firstPointIndex].x, table.points[firstPointIndex].y, table.radiusPoints, table.radiusPoints);
                fillellipse(table.points[secondPointIndex].x, table.points[secondPointIndex].y, table.radiusPoints, table.radiusPoints);
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(TIME_BETWEEN_MOVES * 2));
        int statusWindow = initwindow(200, 150, "End of the game");
        outtextxy(20, 20, "End of the game.");
        if(playerToMove)
        {
            outtextxy(20, 40, "Player 1 wins");
            table.firstWinnings++;
            current = 1;
        }
        else
        {
            outtextxy(20, 60, "Player 2 wins");
            table.secondWinnings++;
            current = 1;
        }
        outtextxy(20, 80, "Current score: ");
        char p[10];
        ConvertFromIntToString(p, table.firstWinnings);
        outtextxy(20, 100, p);
        ConvertFromIntToString(p, table.secondWinnings);
        outtextxy(20, 120, p);

        std::this_thread::sleep_for(std::chrono::milliseconds(TIME_BETWEEN_MOVES * 10));
        setcurrentwindow(gamePage);
        closegraph(statusWindow);
    }
    while(max(table.firstWinnings, table.secondWinnings) < table.settings.firstToWin);
    if(table.secondWinnings > table.firstWinnings)
    {
        EndOfGame = initwindow (DEFAULT_WIDTH,DEFAULT_HEIGHT, "Winner");
        setcurrentwindow(EndOfGame);
        Player1Won();
        closegraph(gamePage);
        delay(5000);
    }
    if(table.firstWinnings > table.secondWinnings)
    {
        EndOfGame = initwindow (DEFAULT_WIDTH,DEFAULT_HEIGHT, "Winner");
        setcurrentwindow(EndOfGame);
        closegraph(gamePage);
        Player2Won();
        delay(5000);
    }
    mainPage = initwindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, "Segments Game");
    setcurrentwindow(mainPage);
    closegraph(EndOfGame);
    Menu();
}
///-------------------------------------------------------------------------------------------------

///---------------- Check What Point Is Clicked ----------------------------------------------------

int CheckWhatPointIsClicked(CTable &table)
{
    int x, y;
    getmouseclick(WM_LBUTTONDOWN, x, y);

    if(x < 0 && y < 0)
        return -1;

    CCircle clickCircle;
    clickCircle.center.x = x;
    clickCircle.center.y = y;
    clickCircle.radius = 0;

    for(int pointIndex = 1; pointIndex <= table.settings.numberOfPoints; ++pointIndex)
    {
        CCircle pointCircle;
        pointCircle.center = table.points[pointIndex];
        pointCircle.radius = table.radiusPoints;
        if(CheckCirclesIntersection(pointCircle, clickCircle))
        {
            return pointIndex;
        }
    }

    return -1;
}

///-------------------------------------------------------------------------------------------------

///---------------- Verify Two Circles Are Intersecting --------------------------------------------
bool CheckCirclesIntersection(CCircle &c1, CCircle &c2)
{
    return (CalculateSqDistanceBetweenPoints(c1.center, c2.center) -
            (c1.radius + c2.radius) * (c1.radius + c2.radius)
           ) < 0;
}
///-------------------------------------------------------------------------------------------------

///---------------- Calculate Square Dist Between Two Points ---------------------------------------
int CalculateSqDistanceBetweenPoints(CPoint &A, CPoint &B)
{
    return (A.x - B.x) * (A.x - B.x) + (A.y - B.y) * (A.y - B.y);
}
///-------------------------------------------------------------------------------------------------

///---------------- Paint Line between Two Given Points Indexes ------------------------------------
void PaintLinePts(CTable &table, int &pIndex1, int &pIndex2)
{
    table.isSelected[pIndex1]  = true;
    table.isSelected[pIndex2] = true;

    line(table.points[pIndex1].x, table.points[pIndex1].y,
         table.points[pIndex2].x, table.points[pIndex2].y);

    ++table.numberOfSegments;
    table.segments[table.numberOfSegments].A = table.points[pIndex1] ;
    table.segments[table.numberOfSegments].B = table.points[pIndex2];

}
///-------------------------------------------------------------------------------------------------

///----------------- Choose Bot Level Easy Move ----------------------------------------------------
int ChooseMoveBotHard(CTable &table)
{
    int indexChosen = rand() % table.settings.numberOfPoints + 1;

    while(table.isSelected[indexChosen] == true)
    {
        ++indexChosen;
        if(indexChosen == table.settings.numberOfPoints + 1)
        {
            indexChosen = 1;
        }
    }

    return indexChosen;
}
///-------------------------------------------------------------------------------------------------

///---------------- Get Move of the Current Player -------------------------------------------------
int GetMove(CTable &table, int &playerTurn, int &helper)
{
    if(playerTurn == 1 && table.settings.isPlayingWithBot == true)
    {
        if(table.settings.botLevel == 2)
        {
            return ChooseMoveBotHard(table);
        }

        return ChooseMoveBotEasy(table, helper);
    }

    return CheckWhatPointIsClicked(table);
}
///-------------------------------------------------------------------------------------------------

///----------------- Choose Bot Level Hard Move ----------------------------------------------------
int ChooseMoveBotEasy(CTable &table, int &whichOne)
{
    int minimumDif = 10000;
    int idx1 = -1,
        idx2 = -1;

    for(int firstPoint = 1; firstPoint <= table.settings.numberOfPoints; ++firstPoint)
    {

        if(!table.isSelected[firstPoint])
        {

            for(int secondPoint = firstPoint + 1; secondPoint <= table.settings.numberOfPoints; ++secondPoint)
            {

                if(!table.isSelected[secondPoint])
                {
                    if(CheckIfSegmentCanBePlaced(table, firstPoint, secondPoint))
                    {
                        int withMinus = 0;
                        int withPlus  = 0;
                        for(int anyPoint = 1; anyPoint <= table.settings.numberOfPoints; ++anyPoint)
                        {
                            if(table.isSelected[anyPoint] == false)
                            {
                                int orr = ComputeOrientation(table.points[firstPoint], table.points[secondPoint], table.points[anyPoint]);
                                if(orr < 0)
                                {
                                    withMinus++;
                                }
                                else
                                {
                                    withPlus++ ;
                                }
                            }
                        }

                        if(minimumDif > abs(withMinus - withPlus))
                        {
                            minimumDif = abs(withMinus - withPlus);
                            idx1 =  firstPoint;
                            idx2 = secondPoint;
                        }
                    }
                }
            }
        }
    }

    return (whichOne == -1 ? idx1 : idx2);
}
///-------------------------------------------------------------------------------------------------

///---------------- Read Settings from .txt file ---------------------------------------------------
void ReadSettings(CSettings &tempSettings) {
    ifstream fin("settings.txt");

    fin >> tempSettings.botLevel;
    fin >> tempSettings.colorOfPlayer1;
    fin >> tempSettings.colorOfPlayer2;
    fin >> tempSettings.firstToWin;
    fin >> tempSettings.isPlayingWithBot;
    fin >> tempSettings.numberOfPoints;

    fin.close();
}
///-------------------------------------------------------------------------------------------------

///---------------- Write Settings to .txt file ----------------------------------------------------
void SaveSettings(CTable &table) {
    ofstream fout("settings.txt");

    fout << table.settings.botLevel << '\n';
    fout << table.settings.colorOfPlayer1 << '\n';
    fout << table.settings.colorOfPlayer2 << '\n';
    fout << table.settings.firstToWin << '\n';
    fout << table.settings.isPlayingWithBot << '\n';
    fout << table.settings.numberOfPoints << '\n';

    fout.close();
}
///-------------------------------------------------------------------------------------------------
