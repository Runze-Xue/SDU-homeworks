#include<windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);   //回调函数 WnProc声明



//主函数创建；
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdline, int iCmdShow)//四个函数分别为 实例句柄 指向前一个实例句柄  
{
	static TCHAR szAppName[] = TEXT("五子棋");
	HWND hwnd; //窗口句柄
	MSG msg;
	WNDCLASS wndclass; // 定义wndclass,WNDCLASS是一个

	//以下为窗口创建的框架

	wndclass.style = CS_HREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;//传递实例句柄
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);//为窗口设置图标
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);//设置了白色的背景（传递画刷句柄）
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;

	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("需要在windows NT下才能执行"), szAppName, MB_ICONERROR);//没什么用，用来判断是否为windows环境
		return 0;
	}
	hwnd = CreateWindow(
		szAppName,//窗口类名称
		TEXT("薛润泽的五子棋")，//窗口的标题
		WS_OVERLAPPEDWINDOW,   //窗口风格：一个带有标题的，系统菜单的，可调边框的窗口。多个“|”链接的其他宏的结合体
		CW_USEDEFAULT,//初始坐标x
		CW_USEDEFAULT,//初始坐标y
		CW_USEDEFAULT,//初始x方向尺寸
		CW_USEDEFAULT,//初始y方向尺寸 CW 意思是create window 创建窗口
		NULL,         //父窗口句柄
		NULL,		 //窗口菜单句柄
		hInstance,   //程序实例句柄
		NULL         //创建参数      
	);
	ShowWindow(hwnd, iCmdShow); //创建窗口
	UpdateWindow(hwnd);          //更新窗口

	while (GetMessage(&msg, NULL, 0, 0))  //核心 消息循环
	{
		TranslateMessage(&msg);            //将鼠标信息转换
		DispatchMessage(&msg);             
	}
	return  msg.wParam;  //主函数结束
}


//WndProc  回调函数的实现  CALLBACK 类型为回调函数
LRESULT CALLBACK WndProc(HWND hwnd ,UINT message, WPARAM wParam ,LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rect;

	switch (message)
	{
	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rect);
		DrawText(hdc, TEXT("大家好，这是我的第一个窗口程序"), - 1, & rect, 
			DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		EndPaint(hwnd, &ps);
		return 0;
	
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);//其他剩余的消息交给windows默认处理

}