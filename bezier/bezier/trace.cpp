#include "trace.h"
#include <cstdlib>

float Trace::bezier_len(Point& p0, Point& p1, Point& p2){
	Point a, b;
	a.scn_x = p0.scn_x - 2*p1.scn_x + p2.scn_x;
	a.scn_y = p0.scn_y - 2*p1.scn_y + p2.scn_y;
	b.scn_x = 2*p1.scn_x - 2*p0.scn_x;
	b.scn_y = 2*p1.scn_y - 2*p0.scn_y;
	float A = 4*(a.scn_x*a.scn_x + a.scn_y*a.scn_y);
	float B = 4*(a.scn_x*b.scn_x + a.scn_y*b.scn_y);
	float C = b.scn_x*b.scn_x + b.scn_y*b.scn_y;

	float Sabc = 2*sqrt(A+B+C);
	float A_2 = sqrt(A);
	float A_32 = 2*A*A_2;
	float C_2 = 2*sqrt(C);
	float BA = B/A_2;

	return ( A_32*Sabc + A_2*B*(Sabc-C_2) + (4*C*A-B*B)*log( (2*A_2+BA+Sabc)/(BA+C_2) ) )/(4*A_32);
}