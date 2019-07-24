// 屏幕溶解.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <windows.h>
using namespace std;
int ScreenWidth, ScreenHeight;
int Interval = 100;

LRESULT CALLBACK Melter(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam) {

	switch (Msg) {
		case WM_CREATE: {
			// 创建
			HDC Desktop = GetDC(HWND_DESKTOP);
			HDC Window = GetDC(hwnd);
			BitBlt(Window, 0, 0, ScreenWidth, ScreenHeight, Desktop, 0, 0, SRCCOPY);
			ReleaseDC(hwnd, Window);
			RestoreDC((HDC)HWND_DESKTOP, (int)Desktop);
			SetTimer(hwnd, 0, Interval, 0);
			ShowWindow(hwnd, SW_SHOW);
		}; break;
		case WM_PRINT: {
			ValidateRect(hwnd, 0);
		}; break;
		case WM_TIMER: {
			HDC Window = GetDC(hwnd);
			int k = (rand() % ScreenHeight) - (150 / 2), y = (rand() % 15), Width = (rand() % 150);
			BitBlt(Window, k, y, Width, ScreenHeight, Window, k, 0, SRCCOPY);
			ReleaseDC(hwnd, Window);
		}; break;
		case WM_DESTROY: {
			KillTimer(hwnd, 0);
			PostQuitMessage(0);
		}; break;
			
	}

	return DefWindowProc(hwnd, Msg, wParam, lParam);

}

int main(HINSTANCE hIns, HINSTANCE Prev, LPSTR Cmd, int showCmd) {

	// 获取屏幕宽度 ===> GetSystemMetrics 获取系统各项参数
	
	ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	ScreenHeight = GetSystemMetrics(SM_CXSCREEN);

	// 初始化wndClass
	WNDCLASS wndClass = {
		0, Melter, 0, 0, hIns, 0, LoadCursor(0, IDC_ARROW), 0, 0, L"ScreenMelter"
	};
	// 对所有窗口进行溶解 ===》 RegisterClass(&wndClass) 注册窗口
	if (RegisterClass(&wndClass)) {

		// 创建窗口
		HWND hwnd = CreateWindowExA(
			WS_EX_TOPMOST, // 创建窗口的样式 ---> 窗口置顶
			"ScreenMelter", // 创建窗口的类名
			0, // 窗口名字
			WS_POPUP, // 弹出式窗口
			0,// X坐标
			0,// Y坐标
			ScreenWidth, // 窗口宽 
			ScreenHeight, // 窗口长
			HWND_DESKTOP, // 父窗口的句柄（桌面）
			0, // 菜单
			hIns, // 实例句柄
			0); // 参数指针
		if (hwnd) {
			// 获取句柄
			srand(GetTickCount64());
			MSG msg = { 0 };
			while (msg.message != WM_QUIT)
			{
				// 检查线程队伍中是否有已发布的消息
				if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
					TranslateMessage(&msg);
					// 调度消息
					DispatchMessage(&msg);
				}
			}
			return msg.wParam;
		}

	}
	else {
		MessageBeep(MB_OK);
		cout << "注册窗口失败" << endl;
	}
	

	//system("pause");


}

