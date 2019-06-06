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
#include "imageresource.h"
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

namespace showtitle {
	string titlename = "Sword Art Offline";
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
	}
}

void Main() {
	showtitle::Main();
}

int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevinstance,
	LPSTR lpCmdline,
	int nCmdshow
) {

	if (checkprev("SAO"))return 0;
	initwin(hInstance, 800, 600, "SAO");
	showwin(nCmdshow);

	srand((UINT)time(0));

	Main();

	closewin(hwnd);
	return 0;
}
