#ifndef TRACE_H__
#define TRACE_H__

#include <string.h>
#include <windows.h>
#include <cmath>

#define MAX_TRACE_POINT 5

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

class Trace{
public:
	Trace(){reset();}
	~Trace(){}

	void reset(){
		num = 0;
		memset(p, 0, sizeof(Point) * MAX_TRACE_POINT);
	}

	void add_point(float x, float y){
		if(num >= MAX_TRACE_POINT)return;
		p[num].scn_x = x;
		p[num].scn_y = y;
		num++;
	}

	void drawbezier(Painter& pen){
		Point psrc = this->p[0];
		if(num == 3){
			float t = 1.0 / 100;
			for(; t < 1; t += 0.01){
				Point cm;
				gen_bezier_point(this->p[0], this->p[1], this->p[2], t, cm);
				pen.drawline(psrc, cm);
				psrc = cm;
			}
		}
	}

	float bezier_len(){
		if(num != 3) return 0;
		return bezier_len(p[0], p[1], p[2]);
	}

	float bezier_len(Point& p0, Point& p1, Point& p2);

protected:
	void gen_bezier_point(Point& p0, Point& p1, Point& p2, float t, Point& pb){
		float st = 1 - t;
		pb.scn_x = pow(st, 2) * p0.scn_x + 2 * t * st * p1.scn_x + pow(t, 2) * p2.scn_x;
		pb.scn_y = pow(st, 2) * p0.scn_y + 2 * t * st * p1.scn_y + pow(t, 2) * p2.scn_y;
	}

public:
	Point	 p[MAX_TRACE_POINT];
	unsigned int num;
};

#endif