#include <iostream>
#include <ctime>

using namespace std;

#define MAX_POINTS 105

struct CPoint {
    int x, y;
};

struct CSegment {
    CPoint A, B;
};

struct CTable {
    CPoint   points[MAX_POINTS]     ;
    bool     isSelected[MAX_POINTS] ; // 'isSelected[i] = true' only when 'i' is already in a segment
    CSegment segments[MAX_POINTS]   ;

    int numberOfSegments,
        numberOfPoints  ;

    int windowHeight,
        windowWidth ;
};

struct CSettings {
    int numberOfPoints;
    int colorOfPlayer1,
        colorOfPlayer2;
};

struct CLine {
    int a, b, c;
};

CPoint  LineIntersections(CLine &line1, CLine &line2);
void    GenerateNRandomPoints(CTable &table, int N);
bool    TheGameIsOver(CTable &table);
bool    SegmentsAreIntersecting(CSegment &s1, CSegment &s2);
CLine   ComputeLineEquationForSegment(CSegment &segment);

int main() {
    cout << "Hello world!\n";

    return 0;
}

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

bool SegmentsAreIntersecting(CSegment &s1, CSegment &s2) {
    CLine line1, line2;

    line1 = ComputeLineEquationForSegment(s1);
    line2 = ComputeLineEquationForSegment(s2);

    CPoint pointIntersection = LineIntersections(line1, line2);
}

CLine ComputeLineEquationForSegment(CSegment &segment) {

}
