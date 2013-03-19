#include "trace.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv){
	HWND hwnd;
	hwnd = GetConsoleWindow();

	Painter pen(hwnd);
	system("color 3e");

	Trace t;
	t.add_point(0, 0);
	t.add_point(20, 200);
	t.add_point(200, 200);

	t.drawbezier(pen);

	printf("len is %f", t.bezier_len());
	getchar();
	return 0;
}