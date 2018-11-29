#include <iostream>
#include <ctime>

using namespace std;

#define MAX_POINTS 105
#define MAX_NAME   25

///---------------- Geometry structs ---------------------------------------------------------------
struct CPoint {
    int x, y;
};
struct CSegment {
    CPoint A, B;
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

    char namePlayer1[MAX_NAME],
         namePlayer2[MAX_NAME];
};
///-------------------------------------------------------------------------------------------------

///---------------- Table game struct --------------------------------------------------------------
struct CTable {
    CPoint   points[MAX_POINTS]     ;
    CSegment segments[MAX_POINTS]   ;
    bool     isSelected[MAX_POINTS] ; // 'isSelected[i] = true' only when 'i' is already in a segment

    int numberOfSegments,
        numberOfPoints  ,
        windowHeight    ,
        windowWidth     ,
        firstWinnings   ,
        secondWinnings  ;

    CSettings settings  ;
};
///-------------------------------------------------------------------------------------------------

///---------------- Game Engine Functions ----------------------------------------------------------
void    StartGame(CTable &table)                    ;
bool    TheGameIsOver(CTable &table)                ;
void    GenerateNRandomPoints(CTable &table, int N) ;
///-------------------------------------------------------------------------------------------------

///---------------- Geometry Functions -------------------------------------------------------------
bool    IsPointOnSegment(CSegment &segment, CPoint &point) ;
bool    SegmentsAreIntersecting(CSegment &s1, CSegment &s2);
int     ComputeOrientation(CPoint &A, CPoint &B, CPoint &C);
///-------------------------------------------------------------------------------------------------

///---------------- CSettings Functions ----------------------------------------------------------
void    setNumberOfPoints(CTable &table, int &newN);
void    setGameWithBot(CTable &table, bool &status);
void    setFirstToWin(CTable &table, int &firstToW);
///-------------------------------------------------------------------------------------------------


///---------------- Main Function ------------------------------------------------------------------
int main() {
    cout << "Hello world!\n";

    return 0;
}
///-------------------------------------------------------------------------------------------------


///---------------- Generator of N Random Points ----------------------------------------------------
void GenerateNRandomPoints(CTable &table, int N) {
    srand(time(NULL));

    while(N--) {
        int xCoordinate = rand() % table.windowWidth ;
        int yCoordinate = rand() % table.windowHeight;

        CPoint newPoint;

        newPoint.x = xCoordinate;
        newPoint.y = yCoordinate;

        table.points[++table.numberOfPoints] = newPoint;
    }
}
///-------------------------------------------------------------------------------------------------

///---------------- Check if the game is over ------------------------------------------------------
bool TheGameIsOver(CTable &table) {
    for(int firstPoint = 1; firstPoint <= table.numberOfPoints; ++firstPoint) {

        if(!table.isSelected[firstPoint]) {

            for(int secondPoint = 1; secondPoint <= table.numberOfPoints; ++secondPoint) {

                if(!table.isSelected[secondPoint]) {
                    CSegment segmentToCheck;
                    segmentToCheck.A = table.points[firstPoint];
                    segmentToCheck.B = table.points[secondPoint];

                    bool flag = false;
                    for(int segmentIndex = 1; segmentIndex <= table.numberOfSegments; ++segmentIndex) {
                        if(SegmentsAreIntersecting(segmentToCheck, table.segments[segmentIndex]) == true) {
                            flag = true;
                            break;
                        }
                    }

                    if(!flag) {
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
void setNumberOfPoints(CTable &table, int &newN) {
    table.settings.numberOfPoints = newN;
}
///-------------------------------------------------------------------------------------------------

///---------------- Set if user is playing with BOT ------------------------------------------------
void setGameWithBot(CTable &table, bool &status) {
    table.settings.isPlayingWithBot = status;
}
///-------------------------------------------------------------------------------------------------

///---------------- Set the winning score ----------------------------------------------------------
void setFirstToWin(CTable &table, int &firstToW) {
    table.settings.firstToWin = firstToW;
}
///-------------------------------------------------------------------------------------------------

///---------------- Engine of the game -------------------------------------------------------------
void StartGame(CTable &table) {
    do {
        /// ... Initializari etc
        /// Functie de clear a ecranului
        GenerateNRandomPoints(table, table.settings.numberOfPoints);

        int playerToMove = 0; // 0 - first, 1 - second

        while(!TheGameIsOver(table)) {

            /// ... Partea de grafica, selectie a punctelor etc

            playerToMove = 1 - playerToMove;
        }

        if(!playerToMove) {
            table.firstWinnings++;
        }
        else {
            table.secondWinnings++;
        }
    } while(max(table.firstWinnings, table.secondWinnings) < table.settings.firstToWin);
}
///-------------------------------------------------------------------------------------------------
