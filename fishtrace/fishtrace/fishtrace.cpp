#include "fishtrace.h"
#include <cstdlib>
#include <assert.h>

void FishTrace::draw(Painter& p){
	if(trace_type == enm_line)
		drawline(p);
	else if(trace_type == enm_bezier)
		drawbezier(p);
}

void FishTrace::drawline(Painter& p){
	for(uint32_t i = 1; i < num; i++)
		p.drawline(this->p[i - 1], this->p[i]);
}

void FishTrace::drawbezier(Painter& pen){
	Point psrc = this->p[0];
	if(num == 3){
		float t = 1.0 / 100;
		for(; t < 1; t += 0.01){
			Point c1, c2, cm;
// 			gen_control_point(this->p[0], this->p[1], t, c1);
// 			gen_control_point(this->p[1], this->p[2], t, c2);
// 			gen_control_point(c1, c2, t, cm);
			gen_bezier_point(this->p[0], this->p[1], this->p[2], t, cm);
			pen.drawline(psrc, cm);
			psrc = cm;
		}
	}
}

void TraceFactory::gen_line_trace(FishTrace& trace){
	trace.trace_type = enm_line;

	uint32_t x1 = 0, y1 = 0;
	gen_begin_pos(x1, y1);
	trace.add_point(x1, y1);

	uint32_t x2 = 0, y2 = 0;
	gen_end_pos(x1, y1, x2, y2);
	trace.add_point(x2, y2);
	float dx = x1 - x2;
	float dy = y1 - y2;
	trace.len = pow(dx, 2) + pow(dy, 2);
}

void TraceFactory::gen_begin_pos(uint32_t& x, uint32_t& y){
	x = rand() % SCN_WIDTH;	 
	y = rand() % SCN_HEIGHT;

	switch(x % 4){
		case 0:{
			x = 0;
			if(y < SCN_HEIGHT / 8) y += SCN_HEIGHT / 8;
			if(y > (SCN_HEIGHT - SCN_HEIGHT / 8)) y -= SCN_HEIGHT / 8;
			break;
			   }
		case 1:{
			x = x % (SCN_WIDTH / 2);
			(y % 2) ? (y = 0) : (y = SCN_HEIGHT);
			break;
			   }
		case 2:{
			x = x % (SCN_WIDTH / 2) + (SCN_WIDTH / 2);
			(y % 2) ? (y = 0) : (y = SCN_HEIGHT);
			break;
			   }
		case 3:{
			x = SCN_WIDTH;
			if(y < SCN_HEIGHT / 8) y += SCN_HEIGHT / 8;
			if(y > (SCN_HEIGHT - SCN_HEIGHT / 8)) y -= SCN_HEIGHT / 8;
			break;
			   }
	}
}

void TraceFactory::gen_end_pos(uint32_t begin_x, uint32_t begin_y, uint32_t& x, uint32_t& y){
	gen_begin_pos(x, y);

	if(begin_x == x && (x == 0 || x == SCN_WIDTH))
		x == 0 ? x = SCN_WIDTH : x = 0;

	if(begin_y == y && (y == 0 || y == SCN_HEIGHT))
		y == 0 ? y = SCN_HEIGHT : y = 0;

	if(begin_x < SCN_WIDTH / 4 && x < SCN_WIDTH / 4){
		int32_t dy = begin_y - y;
		if(dy > (0 - SCN_WIDTH / 4) && dy < 0){
			if(x == 0) 
				y += SCN_HEIGHT / 4;
			else
				x += SCN_WIDTH / 4;
		}else if(dy < SCN_WIDTH / 4 && dy > 0){
			if(x == 0)
				y -= SCN_HEIGHT / 4;
			else
				x += SCN_HEIGHT / 4;
		}
	}
	if(begin_x > SCN_WIDTH * 0.75 && x > SCN_WIDTH * 0.75){
		int32_t dy = begin_y - y;
		if(dy > (0 - SCN_WIDTH / 4) && dy < 0){
			if(x == SCN_WIDTH) 
				y += SCN_HEIGHT / 4;
			else
				x -= SCN_WIDTH / 4;
		}else if(dy < SCN_WIDTH / 4 && dy > 0){
			if(x == SCN_WIDTH)
				y -= SCN_HEIGHT / 4;
			else
				x -= SCN_HEIGHT / 4;
		}
	}
}

void TraceFactory::gen_bezier_trace(FishTrace& trace){
	trace.trace_type = enm_bezier;

	uint32_t x1 = 0, y1 = 0;
	gen_begin_pos(x1, y1);

	uint32_t x2 = 0, y2 = 0;
	gen_end_pos(x1, y1, x2, y2);

	uint32_t x3 = (x1 + x2) / 2;
	uint32_t y3 = (y1 + y2) / 2;

	int32_t x = x3;
	int32_t y = y3;

	//随机左右偏移
	(x1 % 2 == 0) ? (x += SCN_WIDTH / 2) : (x -= SCN_WIDTH / 2);

	//调整到中间
	if(x < 0) x3 += SCN_WIDTH / 2;
	if(x > SCN_WIDTH) x3 -= SCN_WIDTH / 2;		

	(y1 % 2 == 0) ? (y += SCN_HEIGHT / 2) : (y -= SCN_HEIGHT / 2);
	if(y < 0) y3 += SCN_HEIGHT / 2;
	if(y > SCN_HEIGHT) y3 -= SCN_HEIGHT / 2;

	trace.add_point(x1, y1);
	trace.add_point(x3, y3);
	trace.add_point(x2, y2);

	trace.len = bezier_len(Point(x1, y1), Point(x3, y3), Point(x2, y2));
}

void TraceFactory::gen_bezier_trace2(FishTrace& trace){
	trace.trace_type = enm_bezier;

	uint32_t x1 = 0, y1 = 0;
	gen_begin_pos(x1, y1);

	uint32_t x2 = 0, y2 = 0;
	gen_end_pos(x1, y1, x2, y2);

	uint32_t x3 = (x1 + x2) / 2;
	uint32_t y3 = (y1 + y2) / 2;

	int32_t x = x3;
	int32_t y = y3;

	//随机左右偏移
	(x1 % 2 == 0) ? (x += SCN_WIDTH / 2) : (x -= SCN_WIDTH / 2);

	//调整到中间
	if(x < 0) x3 += SCN_WIDTH / 2;
	if(x > SCN_WIDTH) x3 -= SCN_WIDTH / 2;		

	(y1 % 2 == 0) ? (y += SCN_HEIGHT / 2) : (y -= SCN_HEIGHT / 2);
	if(y < 0) y3 += SCN_HEIGHT / 2;
	if(y > SCN_HEIGHT) y3 -= SCN_HEIGHT / 2;

	trace.add_point(x1, y1);
	trace.add_point(x3, y3);
	trace.add_point(x2, y2);
	trace.len = bezier_len(Point(x1, y1), Point(x3, y3), Point(x2, y2));
}

float TraceFactory::bezier_len(Point& p0, Point& p1, Point& p2){
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