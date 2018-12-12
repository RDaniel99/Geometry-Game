#include <iostream>
#include <graphics.h>
#include <winbgim.h>
#include <ctime>
#include "personalGraphic.h"

using namespace std;

#define MAX_POINTS 105
#define MAX_NAME   25

///---------------- Pages Variables ----------------------------------------------------------------
int mainPage                ;
int settingsPage            ;
int chooseColorPage         ;
int chooseGameTypePage      ;
int chooseNumberOfPointsPage;
int chooseFirstToWinPage    ;
int gamePage                ;
///-------------------------------------------------------------------------------------------------

///---------------- Geometry structs ---------------------------------------------------------------
struct CPoint {
    int x, y;
};
struct CSegment {
    CPoint A, B;
};
struct CCircle {
    CPoint center;
    int radius;
};
///-------------------------------------------------------------------------------------------------

///---------------- Settings struct of the Game ----------------------------------------------------
struct CSettings {
    int numberOfPoints;
    int colorOfPlayer1,
        colorOfPlayer2;

    int  botLevel        ;
    bool isPlayingWithBot;
    int  firstToWin      ;
};
///-------------------------------------------------------------------------------------------------

///---------------- Table game struct --------------------------------------------------------------
struct CTable {
    CPoint   points[MAX_POINTS]     ;
    CSegment segments[MAX_POINTS]   ;
    bool     isSelected[MAX_POINTS] ; // 'isSelected[i] = true' only when 'i' is already in a segment

    int numberOfSegments,
        windowHeight    ,
        windowWidth     ,
        firstWinnings   ,
        secondWinnings  ,
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
int     ChooseMoveBotEasy(CTable &table)        ;
int     ChooseMoveBotHard(CTable &table)        ;
int     GetMove(CTable &table, int &playerTurn) ;
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
///-------------------------------------------------------------------------------------------------


///---------------- Main Function ------------------------------------------------------------------
int main() {
    mainPage = initwindow(600, 800, "Segments Game");

    CTable table;

    table.windowHeight = 800;
    table.windowWidth = 600;
    SetNumberOfPoints(table, 15);
    SetFirstToWin(table, 2);
    SetGameWithBot(table, true);
    SetBotLevel(table, 1);

    outtextxy(300, 0, "Welcome to Segments Game");

    rectangle(50, 50, 164, 90);
    outtextxy(60, 60, "Game Settings");

    rectangle(50, 100, 164, 140);
    outtextxy(90, 110, "Play");

    while(true) {
        int x, y;
        getmouseclick(WM_LBUTTONDOWN, x, y);

        if(!(x < 0 && y < 0)) {
            if(x >= 50 && x <= 164 && y >= 50 && y <= 90 && getcurrentwindow() == mainPage) {
                settingsPage = initwindow(600, 800, "Settings Page");
                setcurrentwindow(settingsPage);
                outtextxy(10, 10, "Settings Page. In Progress...");
            }
            else {
                if(x >= 50 && x <= 164 && y >= 100 && y <= 140 && getcurrentwindow() == mainPage) {
                    gamePage = initwindow(600, 800, "Game Play");
                    setcurrentwindow(gamePage);
                    StartGame(table);
                }
            }
        }
    }

    getch();
    closegraph();

    return 0;
}
///-------------------------------------------------------------------------------------------------


///---------------- Generator of N Random Points ----------------------------------------------------
void GenerateNRandomPoints(CTable &table) {
    srand(time(NULL));

    for(int pointIndex = 1; pointIndex <= table.settings.numberOfPoints; ++pointIndex) {
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

        if(minX > 0 && maxX < table.windowWidth && minY > 0 && minY < table.windowHeight) {
            bool flag = false;
            for(int i = 1; i < pointIndex; ++i) {
                CCircle c2;
                c2.center = table.points[i];
                c2.radius = table.radiusPoints;

                if(CheckCirclesIntersection(c1, c2)) {
                    flag = true;
                    break;
                }
            }

            if(flag) {
                --pointIndex;
            }
            else {
                table.points[pointIndex] = newPoint;
            }
        }
        else {
            --pointIndex;
        }
    }
}
///-------------------------------------------------------------------------------------------------

///---------------- Check if a segment can be placed -----------------------------------------------
bool CheckIfSegmentCanBePlaced(CTable &table, int &firstPointIndex, int &secondPointIndex) {
    if(firstPointIndex == secondPointIndex  ) return false;
    if(table.isSelected[firstPointIndex]    ) return false;
    if(table.isSelected[secondPointIndex]   ) return false;

    CSegment segmentToCheck;
    segmentToCheck.A = table.points[firstPointIndex] ;
    segmentToCheck.B = table.points[secondPointIndex];

    for(int segmentIndex = 1; segmentIndex <= table.numberOfSegments; ++segmentIndex) {
        if(SegmentsAreIntersecting(segmentToCheck, table.segments[segmentIndex]) == true) {
            return false;
        }
    }

    return true;
}
///-------------------------------------------------------------------------------------------------

///---------------- Check if the game is over ------------------------------------------------------
bool TheGameIsOver(CTable &table) {
    for(int firstPoint = 1; firstPoint <= table.settings.numberOfPoints; ++firstPoint) {

        if(!table.isSelected[firstPoint]) {

            for(int secondPoint = firstPoint + 1; secondPoint <= table.settings.numberOfPoints; ++secondPoint) {

                if(!table.isSelected[secondPoint]) {
                    if(CheckIfSegmentCanBePlaced(table, firstPoint, secondPoint)) {
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
bool SegmentsAreIntersecting(CSegment &s1, CSegment &s2) {
    int o1 = ComputeOrientation(s1.A, s1.B, s2.A);
    int o2 = ComputeOrientation(s1.A, s1.B, s2.B);
    int o3 = ComputeOrientation(s2.A, s2.B, s1.A);
    int o4 = ComputeOrientation(s2.A, s2.B, s1.B);

    if(o1 != o2 && o3 != o4) return true;

    if(o1 == 0 && IsPointOnSegment(s1, s2.A) == true) return true;
    if(o2 == 0 && IsPointOnSegment(s1, s2.B) == true) return true;
    if(o3 == 0 && IsPointOnSegment(s2, s1.A) == true) return true;
    if(o4 == 0 && IsPointOnSegment(s2, s1.B) == true) return true;

    return false;
}
///-------------------------------------------------------------------------------------------------

///---------------- Check if a given point is on a given segment -----------------------------------
bool IsPointOnSegment(CSegment &segment, CPoint &point) {
    if(max(segment.A.x, segment.B.x) >= point.x &&
       min(segment.A.x, segment.B.x) <= point.x &&
       max(segment.A.y, segment.B.y) >= point.y &&
       min(segment.A.y, segment.B.y) <= point.y)
        return true;

    return false;
}
///-------------------------------------------------------------------------------------------------

///---------------- Check if point C is on the right/left side of segment [AB] ---------------------
int ComputeOrientation(CPoint &A, CPoint &B, CPoint &C) {
    int rez = (B.y - A.y) * (C.x - B.x) -
              (B.x - A.x) * (C.y - B.y) ;

    if(!rez) return 0;

    return rez / abs(rez);
}
///-------------------------------------------------------------------------------------------------

///---------------- Set number of points on the table ----------------------------------------------
void SetNumberOfPoints(CTable &table, int newN) {
    table.settings.numberOfPoints = newN;
}
///-------------------------------------------------------------------------------------------------

///---------------- Set if user is playing with BOT ------------------------------------------------
void SetGameWithBot(CTable &table, bool status) {
    table.settings.isPlayingWithBot = status;
}
///-------------------------------------------------------------------------------------------------

///---------------- Set the winning score ----------------------------------------------------------
void SetFirstToWin(CTable &table, int firstToW) {
    table.settings.firstToWin = firstToW;
}
///-------------------------------------------------------------------------------------------------

///---------------- Set the bot level --------------------------------------------------------------
void SetBotLevel(CTable &table, int botLevel) {
    table.settings.botLevel = botLevel;
}
///-------------------------------------------------------------------------------------------------

///---------------- Paint Points On The Table ------------------------------------------------------
void PaintPoints(CTable &table) {
    for(int pInd = 1; pInd <= table.settings.numberOfPoints; pInd++) {
        setcolor(WHITE);
        fillellipse(table.points[pInd].x, table.points[pInd].y, table.radiusPoints, table.radiusPoints);
    }
}
///---------------- Initialize Table ---------------------------------------------------------------
void SetupTable(CTable &table) {
    table.numberOfSegments  = 0;

    memset(table.points,     0, sizeof(table.points))    ;
    memset(table.segments,   0, sizeof(table.segments))  ;
    memset(table.isSelected, 0, sizeof(table.isSelected));
}
///-------------------------------------------------------------------------------------------------

///---------------- Engine of the game -------------------------------------------------------------
void StartGame(CTable &table) {
    table.firstWinnings = 0;
    table.secondWinnings = 0;
    do {
        cleardevice();
        SetupTable(table);
        GenerateNRandomPoints(table);
        PaintPoints(table);

        int playerToMove = 0; // 0 - first, 1 - second

        while(!TheGameIsOver(table)) {

            int firstPointIndex  = -1,
                secondPointIndex = -1;

            while(secondPointIndex < 0) {
                int x = GetMove(table, playerToMove);
                if(x != -1) {
                    if(table.isSelected[x] == false) {
                        if(firstPointIndex == -1) {
                            firstPointIndex = x;
                            setcolor(RED);
                            fillellipse(table.points[firstPointIndex].x , table.points[firstPointIndex].y , table.radiusPoints, table.radiusPoints);
                        }
                        else {
                            secondPointIndex = x;
                            setcolor(RED);
                            fillellipse(table.points[secondPointIndex].x, table.points[secondPointIndex].y, table.radiusPoints, table.radiusPoints);
                        }
                    }
                }
            }

            if(CheckIfSegmentCanBePlaced(table, firstPointIndex, secondPointIndex) == true) {
                cout << "Player " << playerToMove + 1 << " can place segment at: " << firstPointIndex << ' ' << secondPointIndex << '\n';

                PaintLinePts(table, firstPointIndex, secondPointIndex);

                playerToMove = 1 - playerToMove;
            }
            else {
                cout << "Player " << playerToMove + 1 << "can NOT place segment at: " << firstPointIndex << ' ' << secondPointIndex << '\n';
                setcolor(WHITE);
                fillellipse(table.points[firstPointIndex].x , table.points[firstPointIndex].y , table.radiusPoints, table.radiusPoints);
                fillellipse(table.points[secondPointIndex].x, table.points[secondPointIndex].y, table.radiusPoints, table.radiusPoints);
            }
        }

        if(playerToMove) {
            cout << "First Player Wins\n";
            table.firstWinnings++;
        }
        else {
            cout << "Second player Wins\n";
            table.secondWinnings++;
        }
        cout << table.firstWinnings << ' ' << table.secondWinnings << ' ' << table.settings.firstToWin << '\n';
    } while(max(table.firstWinnings, table.secondWinnings) < table.settings.firstToWin);

    cout << "[DBG] Stop Game...\n";

    setcurrentwindow(mainPage);
    closegraph(gamePage);
}
///-------------------------------------------------------------------------------------------------

///---------------- Check What Point Is Clicked ----------------------------------------------------

int CheckWhatPointIsClicked(CTable &table) {
    int x, y;
    getmouseclick(WM_LBUTTONDOWN, x, y);

    if(x < 0 && y < 0) return -1;

    CCircle clickCircle;
    clickCircle.center.x = x;
    clickCircle.center.y = y;
    clickCircle.radius = 0;

    for(int pointIndex = 1; pointIndex <= table.settings.numberOfPoints; ++pointIndex) {
        CCircle pointCircle;
        pointCircle.center = table.points[pointIndex];
        pointCircle.radius = table.radiusPoints;
        if(CheckCirclesIntersection(pointCircle, clickCircle)) {
            return pointIndex;
        }
    }

    return -1;
}

///-------------------------------------------------------------------------------------------------

///---------------- Verify Two Circles Are Intersecting --------------------------------------------
bool CheckCirclesIntersection(CCircle &c1, CCircle &c2) {
    return (CalculateSqDistanceBetweenPoints(c1.center, c2.center) -
            (c1.radius + c2.radius) * (c1.radius + c2.radius)
           ) < 0;
}
///-------------------------------------------------------------------------------------------------

///---------------- Calculate Square Dist Between Two Points ---------------------------------------
int CalculateSqDistanceBetweenPoints(CPoint &A, CPoint &B) {
    return (A.x - B.x) * (A.x - B.x) + (A.y - B.y) * (A.y - B.y);
}
///-------------------------------------------------------------------------------------------------

///---------------- Paint Line between Two Given Points Indexes ------------------------------------
void PaintLinePts(CTable &table, int &pIndex1, int &pIndex2) {
    table.isSelected[pIndex1]  = true;
    table.isSelected[pIndex2] = true;

    setcolor(RED);

    line(table.points[pIndex1].x , table.points[pIndex1].y,
         table.points[pIndex2].x, table.points[pIndex2].y);

    ++table.numberOfSegments;
    table.segments[table.numberOfSegments].A = table.points[pIndex1] ;
    table.segments[table.numberOfSegments].B = table.points[pIndex2];
}
///-------------------------------------------------------------------------------------------------

///----------------- Choose Bot Level Easy Move ----------------------------------------------------
int ChooseMoveBotEasy(CTable &table) {
    int indexChosen = rand() % table.settings.numberOfPoints + 1;

    while(table.isSelected[indexChosen] == true) {
        ++indexChosen;
        if(indexChosen == table.settings.numberOfPoints + 1) {
            indexChosen = 1;
        }
    }

    return indexChosen;
}
///-------------------------------------------------------------------------------------------------

///---------------- Get Move of the Current Player -------------------------------------------------
int GetMove(CTable &table, int &playerTurn) {
    if(playerTurn == 1 && table.settings.isPlayingWithBot == true) {
        if(table.settings.botLevel == 1) {
            return ChooseMoveBotEasy(table);
        }

        return ChooseMoveBotHard(table);
    }

    return CheckWhatPointIsClicked(table);
}
///-------------------------------------------------------------------------------------------------

///----------------- Choose Bot Level Hard Move ----------------------------------------------------
int ChooseMoveBotHard(CTable &table) {
    int indexChosen = rand() % table.settings.numberOfPoints + 1;

    while(table.isSelected[indexChosen] == true) {
        ++indexChosen;
        if(indexChosen == table.settings.numberOfPoints + 1) {
            indexChosen = 1;
        }
    }

    return indexChosen;
}
///-------------------------------------------------------------------------------------------------
