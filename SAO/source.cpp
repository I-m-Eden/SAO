//
//	source.cpp
//	SAO
//
//	Created by Eden on 17/11/18.
//	Copyright © 2018年 author. All rights reserved.
//
//
//	用Visual Studio 2017编译之前注意事项：
//	项目属性->c/c++->预处理器->预处理器定义 中添加：
//	_CRT_SECURE_NO_DEPRECATE
//	_CRT_NONSTDC_NO_DEPRECATE
//	项目属性->c/c++->代码生成->运行库 修改为 "多线程调试 (/MTd)"
//

//#define DEBUGGING

#define _WIN32_WINNT _WIN32_WINNT_MAXVER

#include "head.h"
#include "vector2.h"
#include "datastruct.h"
#include "winpaint.h"
#include "toolibrary.h"
#include "coloresource.h"
#include "processresource.h"
using namespace std;
#define ref(i,x,y)for(int i=(x);i<=(y);++i)
#define def(i,x,y)for(int i=(x);i>=(y);--i)
const double pi = acos(-1);
const double eps = 1e-5;
string constr(int s) {
	string res = "";
	if (s < 0)res = "-" + constr(-s); else
		if (s < 10)res = res + char(s + 48); else
			res = constr(s / 10), res = res + char(s % 10 + 48);
	return res;
}
void flushmouse() { while (GetAsyncKeyState(VK_LBUTTON) & 0x8000)delay(1); while (peekmsg())delay(1); }
void flushkey() { FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE)); }
template <typename T> T sqr(T x) { return x * x; }

namespace showtitle {
	string titlename = "Mystery";
	void Main() {
		clearscreen(WHITE234);
		textbox t1;
		t1.setbox(0, 0, _winw, _winh);
		t1.setstyle(CYANDARK, 60, 0, "黑体", lgcenterhorizontal | lgcentervertical);
		t1.resethighlight();
		t1.text = titlename.c_str();
		t1.paint();
		flushpaint();
		delay(1000);
		int period = 300;
		def(i, period, 0) {
			t1.clear(WHITE234);
			t1.textcolor = mixrgb(CYANDARK, WHITE234, 1.0*i / period);
			t1.paint();
			flushpaint();
			delay(1);
		}
	}
}
namespace paintboard {
	typedef vector < pair<pair<int, int>, COLORREF > > dotset;
	dotset getcircle(int r, COLORREF linecolor) {
		dotset S;
		if (r <= 0 || r > 1000)return S;
		ref(X, -r, +r) {
			int Y = round(sqrt(r * r - X * X));
			S.push_back(make_pair(make_pair(X, Y), linecolor));
			S.push_back(make_pair(make_pair(X, Y), linecolor));
		}
		ref(Y, -r, +r) {
			int X = round(sqrt(r * r - Y * Y));
			S.push_back(make_pair(make_pair(X, Y), linecolor));
			S.push_back(make_pair(make_pair(X, Y), linecolor));
		}
		sort(S.begin(), S.end());
		S.erase(unique(S.begin(), S.end()), S.end());
		return S;
	}
	bool vis[2100][2100];
	pair<int, int> q[200010];
	dotset getGScircle(int r, COLORREF linecolor, COLORREF bkcolor,int rg = 2) {
		dotset S;
		int rn = r + 50 + rg;
		if (r <= 0 || rn*2+1>=2100)return S;
		vector<COLORREF> VC;
		ref(i, 0, 9)VC.push_back(mixrgb(linecolor, bkcolor, 1.0 - 1.0*i / 10));
		int h = 0, t = 0;
		q[++t] = make_pair(0, r); vis[0 + rn][r + rn] = 1;
		while (h++ < t) {
			int x = q[h].first, y = q[h].second;
			int num = fabs((sqr(x) + sqr(y)) / r - r) / rg * 5;
			if (num > 9)continue;
			if (num < 0)num = 0;
			S.push_back(make_pair(q[h], VC[num]));
			if (!vis[x - 1 + rn][y + rn])q[++t] = make_pair(x - 1, y), vis[x - 1 + rn][y + rn] = 1;
			if (!vis[x + 1 + rn][y + rn])q[++t] = make_pair(x + 1, y), vis[x + 1 + rn][y + rn] = 1;
			if (!vis[x + rn][y - 1 + rn])q[++t] = make_pair(x, y - 1), vis[x + rn][y - 1 + rn] = 1;
			if (!vis[x + rn][y + 1 + rn])q[++t] = make_pair(x, y + 1), vis[x + rn][y + 1 + rn] = 1;
		}
		ref(i, 1, t)vis[q[i].first + rn][q[i].second + rn] = 0;
		return S;
	}
	void paintdotset(dotset S, double X = 0, double Y = 0) {
		ref(i, 0, (int)S.size() - 1)
			pdot(S[i].first.first + X, S[i].first.second + Y, S[i].second);
	}
	void Clearscreen(COLORREF bkcolor) {
		BYTE r = GetRValue(bkcolor), g = GetGValue(bkcolor), b = GetBValue(bkcolor);
		for (int i = 0; i < _pDataSize; i += 3)_pData[i] = b;
		for (int i = 1; i < _pDataSize; i += 3)_pData[i] = g;
		for (int i = 2; i < _pDataSize; i += 3)_pData[i] = r;
	}
	void Paintdotset(dotset S, double X = 0, double Y = 0) {
		ref(i, 0, (int)S.size() - 1)
			Pdot(S[i].first.first + X, S[i].first.second + Y, S[i].second);
	}
}
namespace test {
	struct P {
		COLORREF c1, c2;
		double x, y, r, rg; paintboard::dotset s;
		P() { x = _winw / 2, y = _winh / 2; r = 12; rg = 2; c1 = BLACK; c2 = WHITE; s.clear(); }
		void init() {
			s = paintboard::getGScircle(r, c1, c2, rg);
		}
		void paint() {
			paintboard::Paintdotset(s,x,y);
		}
	};
	P parr[100];
	void Main() {
		P user; user.init();

		clearscreen(WHITE234);
		beginPdot(0);
		{
			ref(i, 0, 99) parr[i] = user, parr[i].r = i, parr[i].init();
			int period = 500;
			ref(i, 1, period) {
				paintboard::Clearscreen(WHITE234);
				int rr = 28 + round(cos(1.0*i/period * pi*10) * 10 *(period-i)/period);
				parr[rr].paint();
				flushPdot();
				flushpaint();
				delay(1);
			}
		}
		goto end;

		while (!_isquit) {
			peekmsg();
			if (iskeydown(VK_ESCAPE)) {
				closewin(hwnd); break;
			}
			user.paint();
			flushPdot();
			flushpaint();
			delay(1);
		}

		end:

		endPdot();
	}
}

namespace physics {
	struct ph_dot {
		double m; vector2 x, v, a;
		ph_dot(double mm, vector2 xx) {
			m = mm; x = xx; v = a = vector2();
		}
		void next(double dt) {
			v = v * (1 - 0.5*dt);
			v = v + a * dt;
			x = x + v * dt;
			a = vector2();
		}
	};
}

namespace test2 {
	class flexball {
	public:
		vector<physics::ph_dot>P;
		//elastic force=(le-e_le)*e_k
		double e_le, e_k;
		//reposive force=r_f-r_s/area
		double r_f, r_s;
		
		void generate(int n, double r) { //initiate shape
			ref(i, 0, n-1) {
				double ang = pi * 2 * i / n;
				P.push_back(physics::ph_dot(1.0, vector2(cos(ang), sin(ang))*r));
			}
		}
		void next(double dt) {
			int n = P.size();
			double area = 0.0;
			ref(i, 0, n - 1) {
				vector2 a = P[i].x, b = P[i == n - 1 ? 0 : i + 1].x;
				area += a ^ b;
			}
			area = fabs(area) / 2;
			ref(i, 0, n - 1) {
				vector2 p1 = P[i == 0 ? n - 1 : i - 1].x;
				vector2 p2 = P[i].x;
				vector2 p3 = P[i == n - 1 ? 0 : i + 1].x;
				p1 = p1 - p2; double n1 = norm(p1);
				p3 = p3 - p2; double n3 = norm(p3);
				vector2 q1 = p1 / n1, q3 = p3 / n3;
				vector2 F = q1 * ((n1-e_le)*e_k)
					+ q3 * ((n3-e_le)*e_k);
				vector2 FF = q1 + q3; if ((p1^p3) > 0)FF = vector2()-FF;
				F = F + FF / norm(FF) * (r_f - r_s / area);
				P[i].a = P[i].a + F / P[i].m;
			}
			ref(i, 0, n - 1)P[i].next(dt);
		}
	};
	void Main() {
		flexball A;
		int nn = 50; double rr = 50;
		A.generate(nn, 50);
		A.e_le = rr * sin(180.0/nn)/2;
		A.e_k = 10;
		A.r_f = 500;
		A.r_s = A.r_f * rr * rr * pi;
		bool fg = 0;
		while (1) {
			peekmsg(); delay(1);
			clearscreen(WHITE234);
			if (getkeydown() == VK_ESCAPE)break;
			if (islbuttondown())fg = 1;
			if (islbuttonup())fg = 0;
			if (fg) {
				int X = getmousex(hwnd), Y = getmousey(hwnd);
				vector2 p = vector2(X - _winw / 2, Y - _winh / 2);
				int S = 0;
				ref(i, 0, A.P.size() - 1) {
					vector2 r = (p - A.P[i].x);
					double nr = norm(r); nr = max(nr, 50);
					//A.P[i].a = vector2(0, 10);
					A.P[i].a = A.P[i].a + r * (10000.0 / nr / nr);
					S = S + (int)nr;
				}
				S /= A.P.size();
				S = max(S, 0); S = min(S/2, 120);
				setf(RGB(150, 150, 150)); fcircle(X, Y, 16);
				setf(RGB(S, S, S)); fcircle(X, Y, 10);
			}
			int n = A.P.size();
			ref(i, 0, A.P.size() - 1) {
				pline(_winw / 2 + A.P[i==0?n-1:i-1].x.x, _winh / 2 + A.P[i==0?n-1:i-1].x.y,
					_winw / 2 + A.P[i].x.x, _winh / 2 + A.P[i].x.y);
			}
			flushpaint();
			A.next(0.01);
		}
	}
}

void Main() {
	showtitle::Main();
	//test::Main();
	test2::Main();
}

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevinstance,
	LPSTR lpCmdline,
	int nCmdshow
) {

	if (checkprev("Mystery"))return 0;
	initwin(hInstance, 800, 600, "Mystery");
	showwin(nCmdshow);

	srand((UINT)time(0));

	Main();

	closewin(hwnd);
	return 0;
}
