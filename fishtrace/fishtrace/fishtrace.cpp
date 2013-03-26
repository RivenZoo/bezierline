#include "fishtrace.h"
#include <cstdlib>
#include <assert.h>

#define line_y(k, c, x) ((k*x)+c)
#define line_x(k, c, y) ((y-c)/k)

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
		double t = 1.0 / 100;
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

bool TraceFactory::gen_line_trace(FishTrace& trace){
	trace.trace_type = enm_line;

	Point p1, p2;
	gen_pos(p1, p2);
	trace.add_point(p1);
	trace.add_point(p2);

	float dx = p1.scn_x - p2.scn_x;
	float dy = p1.scn_y - p2.scn_y;
	trace.len = pow(dx, 2) + pow(dy, 2);
	return true;
}


bool TraceFactory::gen_line_trace(Point& begin, Point& end, bool sign, FishTrace& trace){
	trace.trace_type = enm_line;

	uint32_t seed = rand();
	float d = seed % (SCN_HEIGHT / 4);

	Point p1, p2;
	if(!gen_parallel_line(begin, end, sign, p1, p2))
		return false;

	trace.add_point(p1);
	trace.add_point(p2);

	float dx = p1.scn_x - p2.scn_x;
	float dy = p1.scn_y - p2.scn_y;
	trace.len = pow(dx, 2) + pow(dy, 2);
	return true;
}

bool TraceFactory::gen_line_trace(Point& begin, Point& end, bool sign, uint32_t d, FishTrace& trace){
	trace.trace_type = enm_line;

	Point p1, p2;
	if(!gen_parallel_line(begin, end, sign, d, p1, p2))
		return false;

	trace.add_point(p1);
	trace.add_point(p2);

	float dx = p1.scn_x - p2.scn_x;
	float dy = p1.scn_y - p2.scn_y;
	trace.len = pow(dx, 2) + pow(dy, 2);
	return true;
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

void TraceFactory::gen_pos(Point& p1, Point& p2){
	uint32_t seed = rand();
	float r1 = rand() % 76 / 100.0 + 0.125;
	float r2 = rand() % 76 / 100.0 + 0.125;

	Point cross_p[4] = {Point(0, SCN_HEIGHT * r1), Point(SCN_WIDTH * r2, 0), Point(SCN_WIDTH, SCN_HEIGHT * r1), Point(SCN_WIDTH * r2, SCN_HEIGHT)};
	uint32_t fisrt_index = seed % 4;
	p1 = cross_p[fisrt_index];
	if(fisrt_index != 3){
		cross_p[fisrt_index] = cross_p[3];
	}
	uint32_t second_index = seed % 3;
	p2 = cross_p[second_index];
}

bool TraceFactory::gen_parallel_line(Point& begin, Point& end, bool sign/*+-*/, Point& p1, Point& p2){
	uint32_t seed = rand();
	uint32_t d = seed % (SCN_HEIGHT / 4);

	gen_parallel_line(begin, end, sign, d, p1, p2);
	return true;
}

bool TraceFactory::gen_parallel_line(Point& begin, Point& end, bool sign, uint32_t d, Point& p1, Point& p2){
	if(begin.scn_x == end.scn_x){
		p1.scn_x = begin.scn_x;
		if(sign)
			p1.scn_x += d;
		else
			p1.scn_x -= d;
		if(p1.scn_x < SCN_WIDTH * 0.125) 
			return false;
		else if(p1.scn_x > SCN_WIDTH * 0.875)
			return false;
		p2.scn_x = p1.scn_x;
		p1.scn_y = begin.scn_y;
		p2.scn_y = end.scn_y;
	}else if(begin.scn_y == end.scn_y){
		p1.scn_y = begin.scn_y;
		if(sign)
			p1.scn_y += d;
		else
			p1.scn_y -= d;
		if(p1.scn_y < SCN_HEIGHT * 0.125) 
			return false;
		else if(p1.scn_y > SCN_HEIGHT * 0.875)
			return false;
		p2.scn_y = p1.scn_y;
		p1.scn_x = begin.scn_x;
		p2.scn_x = end.scn_x;
	}else{
		double k = (begin.scn_y - end.scn_y) / (begin.scn_x - end.scn_x);
		double c = (begin.scn_x * end.scn_y - end.scn_x * begin.scn_y) / (begin.scn_x - end.scn_x);

		if(sign)
			c += d;
		else
			c -= d;

		Point cross_p[4] = {Point(0, line_y(k, c, 0)), Point(line_x(k, c, 0), 0), Point(SCN_WIDTH, line_y(k, c, SCN_WIDTH)), Point(line_x(k, c, SCN_HEIGHT), SCN_HEIGHT)};
		uint32_t counter = 0;
		for(uint32_t i = 0; i < 4; i++){
			if(check_point(cross_p[i])){
				if(counter == 0)
					p1 = cross_p[i];
				else
					p2 = cross_p[i];
				counter++;
				if(counter >= 2) break;
			}
		}

		if(counter < 2)
			return false;

		if(p1.scn_x < SCN_WIDTH * 0.125 && p2.scn_x < SCN_WIDTH * 0.125)
			return false;
		if(p1.scn_y < SCN_HEIGHT * 0.125 && p2.scn_y < SCN_HEIGHT * 0.125)
			return false;
		if(p1.scn_x > SCN_WIDTH * 0.875 && p2.scn_x > SCN_WIDTH * 0.875)
			return false;
		if(p1.scn_y > SCN_HEIGHT * 0.875 && p2.scn_y > SCN_HEIGHT * 0.875)
			return false;
	}
	return true;
}

bool TraceFactory::gen_bezier_trace(FishTrace& trace){
	trace.trace_type = enm_bezier;

	Point p1, p2;
	gen_pos(p1, p2);

	float x3 = (p1.scn_x + p2.scn_x) / 2.0;
	float y3 = (p1.scn_y + p2.scn_y) / 2.0;

	float x = x3;
	float y = y3;

	//随机左右偏移
	((int32_t)p1.scn_x % 2 == 0) ? (x += SCN_WIDTH / 2) : (x -= SCN_WIDTH / 2);

	//调整到中间
	if(x < 0) x3 += SCN_WIDTH / 2;
	if(x > SCN_WIDTH) x3 -= SCN_WIDTH / 2;		

	((int32_t)p1.scn_y % 2 == 0) ? (y += SCN_HEIGHT / 2) : (y -= SCN_HEIGHT / 2);
	if(y < 0) y3 += SCN_HEIGHT / 2;
	if(y > SCN_HEIGHT) y3 -= SCN_HEIGHT / 2;

	trace.add_point(p1);
	trace.add_point(x3, y3);
	trace.add_point(p2);

	trace.len = bezier_len(p1, Point(x3, y3), p2);
	return true;
}

bool TraceFactory::gen_bezier_trace2(FishTrace& trace){
	trace.trace_type = enm_bezier;

	Point p1, p2;
	gen_pos(p1, p2);

	float pecent = rand() % 4 / 10.0 + 0.3;

	float x3 = (p1.scn_x + p2.scn_x) * pecent;
	float y3 = (p1.scn_y + p2.scn_y) * pecent;
	
	bool direct = true;
	if((p1.scn_x - p2.scn_x) > (0 - SCN_WIDTH / 4.0) && (p1.scn_x - p2.scn_x) < SCN_WIDTH / 4.0){
		direct = true;
	} else if((p1.scn_y - p2.scn_y) > (0 - SCN_HEIGHT / 4.0) && (p1.scn_y - p2.scn_y) < SCN_HEIGHT / 4.0){
		direct = false;
	}
	else if((int32_t)p2.scn_x % 2 == 0){
		direct = true;
	}else{
		direct = false;
	}

	if(direct){
		//随机左右偏移
		((int32_t)p1.scn_x % 2 == 0) ? (x3 += SCN_WIDTH / 2) : (x3 -= SCN_WIDTH / 2);
		
		//调整到中间
		if(x3 < 0) 
			x3 += SCN_WIDTH;
		else if(x3 > SCN_WIDTH) 
			x3 -= SCN_WIDTH;
	}else{
		((int32_t)p1.scn_y % 2 == 0) ? (y3 += SCN_HEIGHT / 2) : (y3 -= SCN_HEIGHT / 2);

		if(y3 < 0) 
			y3 += SCN_HEIGHT;
		else if(y3 > SCN_HEIGHT) 
			y3 -= SCN_HEIGHT;
	}
	
	trace.add_point(p1);
	trace.add_point(x3, y3);
	trace.add_point(p2);
	trace.len = bezier_len(p1, Point(x3, y3), p2);
	return true;
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