#ifndef FISHTRACE_H__
#define FISHTRACE_H__

#include <string.h>
#include <iostream>
#include "base_typedef.h"
#include <windows.h>
#include <cmath>

#define MAX_TRACE_POINT 5
#define SCN_WIDTH		1024
#define SCN_HEIGHT		738

enum TraceType{
	enm_line = 1,
	enm_bezier,
};

struct Point{
	float scn_x;
	float scn_y;

	Point(){scn_x = 0; scn_y = 0;}
	Point(float x, float y) : scn_x(x), scn_y(y){}

	Point& operator=(const Point& r){
		this->scn_x = r.scn_x;
		this->scn_y = r.scn_y;
		return *this;
	}
};

class Painter{
public:
	Painter(HWND hwnd = NULL) : hwnd_(hwnd){
		hdc_ = GetDC(hwnd);
	}

	virtual ~Painter(){ReleaseDC(hwnd_, hdc_);}

	void drawline(Point& p1, Point& p2){
		MoveToEx(hdc_, p1.scn_x, p1.scn_y, NULL);
		LineTo(hdc_, p2.scn_x, p2.scn_y);
	}

protected:
	HWND hwnd_;
	HDC hdc_;
};

struct FishTrace{
	FishTrace(){reset();}
	~FishTrace(){}

	void reset(){
		num = 0;
		memset(p, 0, sizeof(Point) * MAX_TRACE_POINT);
		trace_type = 0;
	}

	void add_point(Point& a){
		if(num >= MAX_TRACE_POINT)return;
		p[num].scn_x = a.scn_x;
		p[num].scn_y = a.scn_y;
		num++;
	}

	void add_point(uint32_t x, uint32_t y){
		if(num >= MAX_TRACE_POINT)return;
		p[num].scn_x = x;
		p[num].scn_y = y;
		num++;
	}

	void trace(){
		for(uint32_t i = 0; i < num; i++){
			std::cout << "(" << p[i].scn_x << ", " << p[i].scn_y << ") ";
		}
		std::cout << std::endl;
	}

	void draw(Painter& p);

protected:
	void drawline(Painter& p);
	void drawbezier(Painter& p);
	void gen_control_point(Point& p1, Point& p2, float t, Point& pc){
		int32_t dx = p2.scn_x - p1.scn_x;
		int32_t dy = p2.scn_y - p1.scn_y;

		pc.scn_x = p1.scn_x + dx * t;
		pc.scn_y = p1.scn_y + dy * t;
	}

public:
	void gen_bezier_point(Point& p0, Point& p1, Point& p2, float t, Point& pb){
		float st = 1 - t;
		pb.scn_x = pow(st, 2) * p0.scn_x + 2 * t * st * p1.scn_x + pow(t, 2) * p2.scn_x;
		pb.scn_y = pow(st, 2) * p0.scn_y + 2 * t * st * p1.scn_y + pow(t, 2) * p2.scn_y;
	}

public:
	Point	 p[MAX_TRACE_POINT];
	uint32_t num;
	uint8_t  trace_type;
	uint32_t len;
};

class TraceFactory{
public:
	TraceFactory(){}
	virtual ~TraceFactory(){}

	static void gen_line_trace(FishTrace& trace);
	static void gen_bezier_trace(FishTrace& trace);
	static void gen_bezier_trace2(FishTrace& trace);

	static float bezier_len(Point& p0, Point& p1, Point& p2);

private:
	static void gen_begin_pos(uint32_t& x, uint32_t& y);
	static void gen_end_pos(uint32_t begin_x, uint32_t begin_y, uint32_t& x, uint32_t& y);
};

#endif