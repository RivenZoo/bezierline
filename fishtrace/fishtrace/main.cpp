#include "fishtrace.h"
#include <time.h>
#include<stdio.h> 
#include<stdlib.h> 
#include<tchar.h> 
#include<windows.h>
#include <assert.h>

extern "C" WINBASEAPI HWND WINAPI GetConsoleWindow(); 

#define TRACE_NUM 10
FishTrace g_trace[TRACE_NUM];

void test_bezier1(HWND hwnd);
void test_bezier2(HWND hwnd);
void test_parallel_line(HWND hwnd);
void test_gen_pos(HWND hwnd);

int main(int argc,char *argv[]) 
{ 
	srand((unsigned int)time(NULL));
	HWND hwnd; 
	hwnd = GetConsoleWindow();
	system("color 3d");

	Painter pen(hwnd);
	
	FishTrace t;
// 	TraceFactory::gen_bezier_trace(t);
//	TraceFactory::gen_bezier_trace2(t);
// 	t.draw(pen);

//	test_bezier1(hwnd);
 	test_bezier2(hwnd);

// 	Point a(0, 200), b(20, 200), c(400, 600);
// 	float blen = TraceFactory::bezier_len(a, b, c);

//	Point a(1024, 111), b(411, 412), c(822, 738), d;
	//for(float i = 0.01; i < 1; i += 0.01){
	//	t.gen_bezier_point(a, b, c, 0.4203, d);
	//	std::cout << "(" << d.scn_x << ", " << d.scn_y << ")" << std::endl;
	//}
// 	t.add_point(a);
// 	t.add_point(b);
// 	t.add_point(c);
// 	t.trace_type = enm_bezier;
// 	float blen = TraceFactory::bezier_len(a, b, c);
	//t.trace();
// 	std::cout << "bezier len is " << blen << std::endl;

//	t.draw(pen);

// 	Point p1(0, 200), p2(400, 0);
// 	pen.drawline(p1, p2);
// 	TraceFactory::gen_line_trace(p1, p2, true, t);
// 	TraceFactory::gen_line_trace(p1, p2, true, )
// 	t.draw(pen);

//	test_parallel_line(hwnd);

//	test_gen_pos(hwnd);
	getchar(); 
	return 0; 
}

void test_bezier1(HWND hwnd){
	Painter pen(hwnd);
	for(int32_t i = 0; i < TRACE_NUM; i++){
		TraceFactory::gen_bezier_trace(g_trace[i]);
		g_trace[i].draw(pen);
	}
}

void test_bezier2(HWND hwnd){
	Painter pen(hwnd);
	for(int32_t i = 0; i < TRACE_NUM; i++){
		TraceFactory::gen_bezier_trace2(g_trace[i]);
		g_trace[i].draw(pen);
		//g_trace[i].trace();
	}
}

void test_parallel_line(HWND hwnd){
	Point p1(0, 200), p2(400, 0);
	Painter pen(hwnd);
	uint32_t seed = rand();
	uint32_t d = seed % (SCN_HEIGHT / 4) + 10;
	TraceFactory::gen_line_trace(p1, p2, true, d, g_trace[0]);
	bool sign = true;
	uint32_t counter = 0;
	for(int32_t i = 1; i < TRACE_NUM; i++){
		if(TraceFactory::gen_line_trace(g_trace[i - 1].p[0], g_trace[i - 1].p[1], sign, d, g_trace[i])){
			//g_trace[i].trace();
			counter++;
		}else
			break;
	}
	//assert(counter != 0);
	for(uint32_t i = 0; i < counter; i++){
		//g_trace[i].trace();
		g_trace[i].draw(pen);
	}
}

void test_gen_pos(HWND hwnd){
	Painter pen(hwnd);
	Point p1, p2;
	for(uint32_t i = 0; i < TRACE_NUM; i++){
		TraceFactory::gen_pos(p1, p2);
		pen.drawline(p1, p2);
		//p1.pos();
		//p2.pos();
	}
}