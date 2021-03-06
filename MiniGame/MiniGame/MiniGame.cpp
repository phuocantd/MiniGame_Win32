// MiniGame.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "MiniGame.h"
#include <windowsx.h>
#include <WinUser.h>
#include <commctrl.h>
#include <Unknwn.h>
#include <gdiplus.h>
#include <windowsx.h>
#include <time.h>


using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib") 

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct);
void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
void OnDestroy(HWND hwnd);
void drawImagine(HWND hwnd, WCHAR* st, Rect rt);
void OnPaint(HWND hwnd);
void OnLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags);
void OnLButtonUp(HWND hwnd, int x, int y, UINT keyFlags);
void OnMouseMove(HWND hwnd, int x, int y, UINT keyFlags);
void readData();
void loadScore();
void loadGame1(HWND hwnd);
void loadGame2(HWND hwnd);
void printResultPlay(HWND hwnd);
void updateChart();
void DecreaseSeconds(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime);
void loadGame(HWND hwnd);

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_MINIGAME, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MINIGAME));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
WNDCLASSEXW wcex;
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	//WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MINIGAME));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_BTNFACE + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MINIGAME);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
int WIDTH = 1000;
int HEIGHT = 600;
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		200, 70, WIDTH, HEIGHT, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//

HMENU hPopupMenu;
POINT point;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hWnd, WM_CREATE, OnCreate);
		HANDLE_MSG(hWnd, WM_COMMAND, OnCommand);
		HANDLE_MSG(hWnd, WM_PAINT, OnPaint);
		HANDLE_MSG(hWnd, WM_DESTROY, OnDestroy);
		HANDLE_MSG(hWnd, WM_LBUTTONUP, OnLButtonUp);
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

HWND lblTitle, btnStart, btnIntro, btnScore, btnExit;
HWND btnIntroExit, btnIntroBack, lblIntroTitle;
HWND lblScoreTitle, lblScoreValue;
HWND hListScore, lblScoreName, btnScoreBack, btnScoreExit;
HFONT hFont;
Rect rectLeft(50, 150, 400, 250), rectRight(550, 150, 400, 250), rectTop(300, 80, 400, 250);
HWND btnPlayBack, lblQues, btnAnsRight, btnAnsLeft;

const int BUFFERSIZE = 260;
WCHAR buffer[BUFFERSIZE];
WCHAR curPath[BUFFERSIZE];
WCHAR configPath[BUFFERSIZE];

const int SCORESIZE = 10;
WCHAR arrScore[2][SCORESIZE][BUFFERSIZE];
const int FLAGSIZE = 21;
WCHAR arrFlag[FLAGSIZE][BUFFERSIZE];
bool appear[FLAGSIZE] = { 0 };
bool ans = 0, click = 0;
int count = 0, valueScore = 0;
WCHAR nameScore[BUFFERSIZE];
HWND lblNoti, lblValueScore, lblNotiImport, txtName, btnOK;
bool checkUpdate = 0;

HWND lblSeconds;
int seconds = 10;
int game = 0;

BOOL OnCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct)
{
	// Tạo đường dẫn tuyệt đối tới vi tri hien hanh
	GetCurrentDirectory(BUFFERSIZE, curPath);
	readData();			//Đọc dữ liệu

	//hThisPC = TreeView_InsertItem(hTreeView, &tvInsert);

	// Lấy font hệ thống ban đầu
	LOGFONT lf;
	GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);
	hFont = CreateFont(40, 50,
		lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
		lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
		lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
		lf.lfPitchAndFamily, lf.lfFaceName);
	
	
	//Menu Chinh
	lblTitle = CreateWindowEx(NULL, L"STATIC", L"Mini Game", WS_CHILD | WS_VISIBLE | SS_CENTER,
		120, 50, 800, 600, hwnd,
		NULL, lpCreateStruct->hInstance, NULL);
	SetWindowFont(lblTitle, hFont, TRUE);
	hFont = CreateFont(20, 15,
		lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
		lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
		lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
		lf.lfPitchAndFamily, lf.lfFaceName);
	btnStart = CreateWindowEx(NULL, L"BUTTON", L"Bắt đầu",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 400, 120, 180, 70, hwnd,
		(HMENU)IDC_BTNSTART, lpCreateStruct->hInstance, NULL);
	SetWindowFont(btnStart, hFont, TRUE);
	btnIntro = CreateWindowEx(NULL, L"BUTTON", L"Hướng dẫn",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 400, 220, 180, 70, hwnd,
		(HMENU)IDC_BTNINTRO, lpCreateStruct->hInstance, NULL);
	SetWindowFont(btnIntro, hFont, TRUE);
	btnScore = CreateWindowEx(NULL, L"BUTTON", L"Điểm số",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 400, 320, 180, 70, hwnd,
		(HMENU)IDC_BTNSCORE, lpCreateStruct->hInstance, NULL);
	SetWindowFont(btnScore, hFont, TRUE);
	btnExit = CreateWindowEx(NULL, L"BUTTON", L"Thoát",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 400, 420, 180, 70, hwnd,
		(HMENU)IDC_BTNEXIT, lpCreateStruct->hInstance, NULL);
	SetWindowFont(btnExit, hFont, TRUE);
	/*ShowWindow(lblTitle, 0);
	ShowWindow(btnStart, 0);
	ShowWindow(btnIntro, 0);
	ShowWindow(btnScore, 0);
	ShowWindow(btnExit, 0);*/

	//window khi choi
	btnPlayBack = CreateWindowEx(NULL, L"BUTTON", L"Quay lại",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 400, 450, 180, 70, hwnd,
		(HMENU)IDC_BTN_PLAYBACK, lpCreateStruct->hInstance, NULL);
	SetWindowFont(btnPlayBack, hFont, TRUE);
	ShowWindow(btnPlayBack, 0);
	hFont = CreateFont(50, 40,
		lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
		lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
		lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
		lf.lfPitchAndFamily, lf.lfFaceName);

	//Thời gian khi chơi
	lblSeconds = CreateWindowEx(NULL, L"STATIC", L"10", WS_CHILD | WS_VISIBLE | SS_CENTER,
		450, 20, 100, 50, hwnd,
		(HMENU)NULL, lpCreateStruct->hInstance, NULL);
	SetWindowFont(lblSeconds, hFont, TRUE);
	

	//Game 1
	lblQues = CreateWindowEx(NULL, L"STATIC", L"ARGENTINA", WS_CHILD | WS_VISIBLE | SS_CENTER,
		100, 70, 800, 50, hwnd,
		NULL, lpCreateStruct->hInstance, NULL);
	SetWindowFont(lblQues, hFont, TRUE);
	ShowWindow(lblQues, 0);
	
	hFont = CreateFont(28, 20,
		lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
		lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
		lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
		lf.lfPitchAndFamily, lf.lfFaceName);
	//game 2 
	btnAnsLeft = CreateWindowEx(NULL, L"BUTTON", L"ARGENTINA",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 100, 350, 300, 90, hwnd,
		(HMENU)IDC_BTN_ANSLEFT, lpCreateStruct->hInstance, NULL);
	SetWindowFont(btnAnsLeft, hFont, TRUE);
	btnAnsRight = CreateWindowEx(NULL, L"BUTTON", L"ARGENTINA",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 580, 350, 300, 90, hwnd,
		(HMENU)IDC_BTN_ANSRIGHT, lpCreateStruct->hInstance, NULL);
	SetWindowFont(btnAnsRight, hFont, TRUE);
	ShowWindow(btnAnsLeft, 0);
	ShowWindow(btnAnsRight, 0);

	hFont = CreateFont(20, 15,
		lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
		lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
		lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
		lf.lfPitchAndFamily, lf.lfFaceName);

	//Thong bao diem so 
	//Neu lot vao top 10 thi nhap ten nguoi choi
	lblNoti = CreateWindowEx(NULL, L"STATIC", L"Điểm số bạn đạt được là:", WS_CHILD | WS_VISIBLE | SS_CENTER,
		100, 70, 800, 50, hwnd,
		NULL, lpCreateStruct->hInstance, NULL);
	SetWindowFont(lblNoti, hFont, TRUE);
	lblNotiImport = CreateWindowEx(NULL, L"STATIC", L"Bạn đã đạt được Top 10 người chơi xuất sắc nhất. Vui lòng nhập tên tại đây", 
		WS_CHILD | WS_VISIBLE | SS_CENTER,
		150, 250, 700, 50, hwnd,
		NULL, lpCreateStruct->hInstance, NULL);
	SetWindowFont(lblNotiImport, hFont, TRUE);
	txtName = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", L"Vô danh",
		WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
		300, 330, 400, 35, hwnd,
		(HMENU)NULL, lpCreateStruct->hInstance, NULL);
	SetWindowFont(txtName, hFont, TRUE);
	btnOK = CreateWindowEx(NULL, L"BUTTON", L"OK",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 400, 420, 180, 70, hwnd,
		(HMENU)IDC_BTNOK, lpCreateStruct->hInstance, NULL);
	SetWindowFont(btnScore, hFont, TRUE);
	hFont = CreateFont(40, 35,
		lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
		lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
		lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
		lf.lfPitchAndFamily, lf.lfFaceName);
	lblValueScore = CreateWindowEx(NULL, L"STATIC", L"0/1000", WS_CHILD | WS_VISIBLE | SS_CENTER,
		220, 150, 500, 100, hwnd,
		NULL, lpCreateStruct->hInstance, NULL);
	SetWindowFont(lblValueScore, hFont, TRUE);
	ShowWindow(lblSeconds, 0);
	ShowWindow(lblNoti, 0);
	ShowWindow(lblValueScore, 0);
	ShowWindow(txtName, 0);
	ShowWindow(btnOK, 0);
	ShowWindow(lblNotiImport, 0);
	
	//Window trong phan huong dan
	hFont = CreateFont(30, 22,
		lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
		lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
		lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
		lf.lfPitchAndFamily, lf.lfFaceName);
	lblIntroTitle = CreateWindowEx(NULL, L"STATIC", L"Game bao gồm 2 chế độ: +) Chọn lá cờ phù hợp với tên quốc gia. +) Chọn tên quốc gia phù hợp với lá cờ. Trả lời bằng cách chọn vào cờ hoặc tên quốc gia. Mỗi câu có 10s, điểm được tính bằng cách lấy số giây còn lại nhân với 10. Điểm số sẽ tự động tăng khi trả lời đúng.", 
		WS_CHILD | WS_VISIBLE | SS_CENTER,
		100, 100, 800, 500, hwnd,
		NULL, lpCreateStruct->hInstance, NULL);
	SetWindowFont(lblIntroTitle, hFont, TRUE);
	hFont = CreateFont(20, 15,
		lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
		lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
		lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
		lf.lfPitchAndFamily, lf.lfFaceName);
	btnIntroExit = CreateWindowEx(NULL, L"BUTTON", L"Thoát",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 550, 400, 180, 70, hwnd,
		(HMENU)IDC_BTNEXIT, lpCreateStruct->hInstance, NULL);
	SetWindowFont(btnIntroExit, hFont, TRUE);
	btnIntroBack = CreateWindowEx(NULL, L"BUTTON", L"Quay lại",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 250, 400, 180, 70, hwnd,
		(HMENU)IDC_BTN_INTROBACK, lpCreateStruct->hInstance, NULL);
	SetWindowFont(btnIntroBack, hFont, TRUE);
	ShowWindow(btnIntroBack, 0);
	ShowWindow(btnIntroExit, 0);
	ShowWindow(lblIntroTitle, 0);

	//Window diem so luc choi
	lblScoreTitle = CreateWindowEx(NULL, L"STATIC", L"Điểm: ", WS_CHILD | WS_VISIBLE | SS_CENTER,
		850, 20, 50, 100, hwnd,
		NULL, lpCreateStruct->hInstance, NULL);
	lblScoreValue = CreateWindowEx(NULL, L"STATIC", L"0", WS_CHILD | WS_VISIBLE | SS_CENTER,
		900, 20, 50, 50, hwnd,
		NULL, lpCreateStruct->hInstance, NULL);
	ShowWindow(lblScoreTitle, 0);
	ShowWindow(lblScoreValue, 0);

	//Window xem diem
	hFont = CreateFont(30, 25,
		lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
		lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
		lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
		lf.lfPitchAndFamily, lf.lfFaceName);
	lblScoreName = CreateWindowEx(NULL, L"STATIC", L"BẢNG XẾP HẠNG", WS_CHILD | WS_VISIBLE | SS_CENTER,
		100, 50, 800, 500, hwnd,
		NULL, lpCreateStruct->hInstance, NULL);
	SetWindowFont(lblScoreName, hFont, TRUE);

	hFont = CreateFont(15, 10,
		lf.lfEscapement, lf.lfOrientation, lf.lfWeight,
		lf.lfItalic, lf.lfUnderline, lf.lfStrikeOut, lf.lfCharSet,
		lf.lfOutPrecision, lf.lfClipPrecision, lf.lfQuality,
		lf.lfPitchAndFamily, lf.lfFaceName);
	hListScore = CreateWindowEx(WS_EX_CLIENTEDGE, WC_LISTVIEW, NULL,
		WS_CHILD | WS_VISIBLE | WS_VSCROLL | LVS_REPORT,
		150, 100, 700, 300, hwnd,
		NULL, lpCreateStruct->hInstance, NULL);
	SetWindowFont(hListScore, hFont, TRUE);

	LVCOLUMN col;
	col.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	col.fmt = LVCFMT_LEFT;
	// Cột thứ nhất
	col.cx = 500;
	col.pszText = (LPWSTR)L"Tên";
	ListView_InsertColumn(hListScore, 0, &col);
	// Cột thứ hai
	col.cx = 200;
	col.pszText = (LPWSTR)L"Điểm số";
	ListView_InsertColumn(hListScore, 1, &col);

	btnScoreExit = CreateWindowEx(NULL, L"BUTTON", L"Thoát",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 550, 420, 180, 70, hwnd,
		(HMENU)IDC_BTNEXIT, lpCreateStruct->hInstance, NULL);
	SetWindowFont(btnScoreExit, hFont, TRUE);
	btnScoreBack = CreateWindowEx(NULL, L"BUTTON", L"Quay lại",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 250, 420, 180, 70, hwnd,
		(HMENU)IDC_BTN_SCOREBACK, lpCreateStruct->hInstance, NULL);
	SetWindowFont(btnScoreBack, hFont, TRUE);
	
	ShowWindow(lblScoreName, 0);
	ShowWindow(hListScore, 0);
	ShowWindow(btnScoreBack, 0);
	ShowWindow(btnScoreExit, 0);

	
	return true;
}

void setPlay()
{
	click = 1;
	valueScore = 0;
	wsprintf(buffer, L"0");
	SetWindowText(lblScoreValue, buffer);
	count = 0;
}

void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	// Parse the menu selections:

	switch (id)
	{
	case IDC_BTNSTART:
		
		ShowWindow(lblScoreTitle, 4);
		ShowWindow(lblScoreValue, 4);

		ShowWindow(lblTitle, 0);
		ShowWindow(btnStart, 0);
		ShowWindow(btnIntro, 0);
		ShowWindow(btnScore, 0);
		ShowWindow(btnExit, 0);

		ShowWindow(btnPlayBack, 4);
		//ShowWindow(lblQues, 4);
		//ShowWindow(btnAnsLeft, 4);
		//ShowWindow(btnAnsRight, 4);
		ShowWindow(lblSeconds, 4);
		loadGame(hwnd);
		
		SetTimer(hwnd, IDT_TIMER, 1000,	(TIMERPROC)DecreaseSeconds);   // Timer callback   

		setPlay();
		break;
	case IDC_BTNINTRO:
		ShowWindow(lblTitle, 0);
		ShowWindow(btnStart, 0);
		ShowWindow(btnIntro, 0);
		ShowWindow(btnScore, 0);
		ShowWindow(btnExit, 0);

		ShowWindow(btnIntroBack, 4);
		ShowWindow(btnIntroExit, 4);
		ShowWindow(lblIntroTitle, 4);
		break;
	case IDC_BTNSCORE:
		ShowWindow(lblTitle, 0);
		ShowWindow(btnStart, 0);
		ShowWindow(btnIntro, 0);
		ShowWindow(btnScore, 0);
		ShowWindow(btnExit, 0);

		loadScore();
		ShowWindow(lblScoreName, 4);
		ShowWindow(hListScore, 4);
		ShowWindow(btnScoreBack, 4);
		ShowWindow(btnScoreExit, 4);
		break;
	case IDC_BTNEXIT:
		DestroyWindow(hwnd);
		break;
	case IDC_BTN_ANSLEFT:
		count++;
		if (!ans)
		{
			valueScore += 10 * seconds;
		}
		wsprintf(buffer, L"%d", valueScore);
		SetWindowText(lblScoreValue, buffer);
		if (count < 10)
		{
			loadGame(hwnd);
		}
		else
		{
			KillTimer(hwnd, IDT_TIMER); // Hủy bỏ timer
			click = 0;
			count = 0;
			printResultPlay(hwnd);
		}
		break;
	case IDC_BTN_ANSRIGHT:
		count++;
		if (ans)
		{
			valueScore += 10 * seconds;
		}
		wsprintf(buffer, L"%d", valueScore);
		SetWindowText(lblScoreValue, buffer);
		if (count < 10)
		{
			loadGame(hwnd);
		}
		else
		{
			KillTimer(hwnd, IDT_TIMER); // Hủy bỏ timer
			click = 0;
			count = 0;
			printResultPlay(hwnd);
		}
		break;
	case IDC_BTN_PLAYBACK:
		drawImagine(hwnd, (WCHAR*)L"NONE", rectRight);
		drawImagine(hwnd, (WCHAR*)L"NONE", rectLeft);
		drawImagine(hwnd, (WCHAR*)L"NONE", rectTop);

		ShowWindow(lblTitle, 4);
		ShowWindow(btnStart, 4);
		ShowWindow(btnIntro, 4);
		ShowWindow(btnScore, 4);
		ShowWindow(btnExit, 4);

		ShowWindow(btnPlayBack, 0);
		ShowWindow(lblQues, 0);

		ShowWindow(btnAnsLeft, 0);
		ShowWindow(btnAnsRight, 0);

		ShowWindow(lblScoreTitle, 0);
		ShowWindow(lblScoreValue, 0);

		ShowWindow(lblSeconds, 0);
		KillTimer(hwnd, IDT_TIMER);
		break;
	case IDC_BTNOK:
		ShowWindow(lblTitle, 4);
		ShowWindow(btnStart, 4);
		ShowWindow(btnIntro, 4);
		ShowWindow(btnScore, 4);
		ShowWindow(btnExit, 4);

		ShowWindow(lblNoti, 0);
		ShowWindow(lblValueScore, 0);
		ShowWindow(txtName, 0);
		ShowWindow(btnOK, 0);
		ShowWindow(lblNotiImport, 0);

		if (checkUpdate)
		{
			Edit_GetText(txtName, nameScore, BUFFERSIZE);
			updateChart();
		}

		break;
	case IDC_BTN_INTROBACK:
		ShowWindow(lblTitle, 4);
		ShowWindow(btnStart, 4);
		ShowWindow(btnIntro, 4);
		ShowWindow(btnScore, 4);
		ShowWindow(btnExit, 4);

		ShowWindow(btnIntroBack, 0);
		ShowWindow(btnIntroExit, 0);
		ShowWindow(lblIntroTitle, 0);
		break;
	case IDC_BTN_SCOREBACK:
		ShowWindow(lblTitle, 4);
		ShowWindow(btnStart, 4);
		ShowWindow(btnIntro, 4);
		ShowWindow(btnScore, 4);
		ShowWindow(btnExit, 4);

		ShowWindow(lblScoreName, 0);
		ShowWindow(hListScore, 0);
		ShowWindow(btnScoreBack, 0);
		ShowWindow(btnScoreExit, 0);
		break;
	case IDM_ABOUT:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hwnd, About);
		break;
	case IDM_EXIT:
		DestroyWindow(hwnd);
		break;
		//	default:
			//	break;
	}
}

void OnDestroy(HWND hwnd)
{
	PostQuitMessage(0);
}



//Gdiplus::Graphics *graphics;
void drawImagine(HWND hwnd, WCHAR* st, Rect rt)
{
	GdiplusStartupInput gdiplusStartupInput;

	ULONG_PTR gdiplusToken;

	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	//GetCurrentDirectory(BUFFERSIZE, curPath);
	wsprintf(configPath, L"%s\\flag\\%s.png", curPath, st);

	Gdiplus::Graphics *graphics = Gdiplus::Graphics::FromHDC(GetDC(hwnd));
	
	Gdiplus::Image* image = new Image(configPath);
	Gdiplus::Status result = graphics->DrawImage(image, rt);
	GdiplusShutdown(gdiplusToken);
	Gdiplus::Color col = Color::Aqua;
}
void OnPaint(HWND hwnd)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);
	// TODO: Add any drawing code that uses hdc here...
	//drawImagine(hwnd, (WCHAR*)L"korea.png");

	EndPaint(hwnd, &ps);
}


void OnLButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{
	
}

void OnLButtonUp(HWND hwnd, int x, int y, UINT keyFlags)
{
	if (click)
	{
		count++;
		if (x >= 50 && x <= 450 && y >= 150 && y <= 400)
		{
			if (!ans)
			{
				valueScore += 10*seconds;
			}
		}
		if (x >= 550 && x <= 950 && y >= 150 && y <= 400)
		{
			if (ans)
			{
				valueScore += 10*seconds;
			}
		}
		wsprintf(buffer, L"%d", valueScore);
		SetWindowText(lblScoreValue, buffer);
		if (count < 10)
		{
			loadGame(hwnd);
		}
		else
		{
			KillTimer(hwnd, IDT_TIMER); // Hủy bỏ timer
			click = 0;
			count = 0;
			printResultPlay(hwnd);
		}
	}
}

void OnMouseMove(HWND hwnd, int x, int y, UINT keyFlags)
{

}

void readData()
{
	WCHAR index[10];
	wsprintf(configPath, L"%s\\config.ini", curPath);
	for (int i = 0; i < SCORESIZE; i++)
	{
		wsprintf(index, L"%d", i);
		GetPrivateProfileString(L"name", index, L"Default value", arrScore[0][i], BUFFERSIZE, configPath);
		GetPrivateProfileString(L"score", index, L"Default value", arrScore[1][i], BUFFERSIZE, configPath);
		
	}
	for (int i = 0; i < FLAGSIZE; i++)
	{
		wsprintf(index, L"%d", i);
		GetPrivateProfileString(L"flag", index, L"Default value", arrFlag[i], BUFFERSIZE, configPath);
	}
}

void loadScore()
{
	ListView_DeleteAllItems(hListScore);
	int itemIndex = 0;
	for (int i = 0; i < 10; i++)
	{
		LV_ITEM row;
		row.mask = LVIF_TEXT | LVIF_PARAM;
		row.cColumns = 150;
		row.iItem = itemIndex;
		row.iSubItem = 0; // Cột đầu tiên - Tên
		row.pszText = arrScore[0][i];
		row.lParam = (LPARAM)arrScore[0][i];

		// Kết hợp đường dẫn với tên thư mục là sẽ ra path ngay
		ListView_InsertItem(hListScore, &row);
		ListView_SetItemText(hListScore, itemIndex, 1, arrScore[1][i]);
		itemIndex++; // Qua dòng kế
	}
}

void loadGame(HWND hwnd)
{
	srand(time(0));
	game = rand() % 2;
	if (game)
	{
		ShowWindow(lblQues, 4);
		ShowWindow(btnAnsLeft, 0);
		ShowWindow(btnAnsRight, 0);
		drawImagine(hwnd, (WCHAR*)L"NONE", rectTop);
		click = 1;
		loadGame1(hwnd);
	}
	else
	{
		ShowWindow(lblQues, 0);
		ShowWindow(btnAnsLeft, 4);
		ShowWindow(btnAnsRight, 4);
		drawImagine(hwnd, (WCHAR*)L"NONE", rectRight);
		drawImagine(hwnd, (WCHAR*)L"NONE", rectLeft);
		click = 0;
		loadGame2(hwnd);
	}
}

void loadGame1(HWND hwnd)
{

	seconds = 10;
	//WCHAR buffer[3];
	wsprintf(buffer, L"10");
	SetWindowText(lblSeconds, buffer); // Set text mới
	int pos, postmp;
	srand(time(0));
	do
	{
		pos = rand() % FLAGSIZE;
	} while (appear[pos]);
	do
	{
		postmp = rand() % FLAGSIZE;
	} while (postmp==pos);
	ans = rand() % 2;
	appear[pos] = 1;

	SetWindowText(lblQues, arrFlag[pos]);
	SetTimer(hwnd, IDT_TIMER, 1000, (TIMERPROC)DecreaseSeconds);   // Timer callback   
	if (ans)
	{
		drawImagine(hwnd, arrFlag[pos], rectRight);
		drawImagine(hwnd, arrFlag[postmp], rectLeft);
	}
	else
	{
		drawImagine(hwnd, arrFlag[pos], rectLeft);
		drawImagine(hwnd, arrFlag[postmp], rectRight);
	}
}

void loadGame2(HWND hwnd)
{

	seconds = 10;
	//WCHAR buffer[3];
	wsprintf(buffer, L"10");
	SetWindowText(lblSeconds, buffer); // Set text mới

	int pos, postmp;
	srand(time(0));
	do
	{
		pos = rand() % FLAGSIZE;
	} while (appear[pos]);
	do
	{
		postmp = rand() % FLAGSIZE;
	} while (postmp == pos);

	ans = rand() % 2;
	appear[pos] = 1;

	//SetWindowText(lblQues, arrFlag[pos]);
	drawImagine(hwnd, arrFlag[pos], rectTop);


	SetTimer(hwnd, IDT_TIMER, 1000, (TIMERPROC)DecreaseSeconds);   // Timer callback   
	if (!ans)
	{
		//drawImagine(hwnd, arrFlag[pos], rectRight);
		SetWindowText(btnAnsLeft, arrFlag[pos]);
		SetWindowText(btnAnsRight, arrFlag[postmp]);
	}
	else
	{
		//drawImagine(hwnd, arrFlag[pos], rectLeft);
		SetWindowText(btnAnsRight, arrFlag[pos]);
		SetWindowText(btnAnsLeft, arrFlag[postmp]);
	}

}

void printResultPlay(HWND hwnd)
{
	wsprintf(buffer, L"%d/1000", valueScore);
	SetWindowText(lblValueScore, buffer);

	ShowWindow(lblNoti, 4);
	ShowWindow(lblValueScore, 4);
	ShowWindow(btnOK, 4);
	ShowWindow(lblSeconds, 0);
	//if (valueScore>arrScore[1][10]
	if (valueScore > wcstol(arrScore[1][9], 0, 10))
	{
		wsprintf(buffer, L"%d", (arrScore[1][10]));
		
		ShowWindow(txtName, 4);
		ShowWindow(lblNotiImport, 4);
		checkUpdate = 1;
	}
	

	drawImagine(hwnd, (WCHAR*)L"NONE", rectRight);
	drawImagine(hwnd, (WCHAR*)L"NONE", rectLeft);
	drawImagine(hwnd, (WCHAR*)L"NONE", rectTop);

	ShowWindow(btnPlayBack, 0);
	ShowWindow(lblQues, 0);

	ShowWindow(btnAnsLeft, 0);
	ShowWindow(btnAnsRight, 0);

	ShowWindow(lblScoreTitle, 0);
	ShowWindow(lblScoreValue, 0);

	for (int i = 0; i < FLAGSIZE; i++)
	{
		appear[i] = 0;
	}

}

void updateChart()
{
	
	WCHAR index[10];
	wsprintf(configPath, L"%s\\config.ini", curPath);
	
	for (int i = 8; i >= 0; i--)
	{
		wsprintf(index, L"%d", i+1);
		if (valueScore > wcstol(arrScore[1][i], 0, 0))
		{ 
			 
			 wsprintf(arrScore[1][i + 1], L"%s", arrScore[1][i]);
			 wsprintf(arrScore[0][i + 1], L"%s", arrScore[0][i]);
			 WritePrivateProfileString(L"score", index, arrScore[1][i+1], configPath);
			 WritePrivateProfileString(L"name", index, arrScore[0][i + 1], configPath);
			 if (0 == i)
			 {
				 wsprintf(index, L"%d", 0);
				 wsprintf(arrScore[1][0], L"%d", valueScore);
				 wsprintf(arrScore[0][0], L"%s", nameScore);
				 WritePrivateProfileString(L"score", index, arrScore[1][0], configPath);
				 WritePrivateProfileString(L"name", index, arrScore[0][0], configPath);
			 }
		}
		else
		{
			wsprintf(arrScore[1][i + 1], L"%d", valueScore);
			wsprintf(arrScore[0][i + 1], L"%s", nameScore);
			WritePrivateProfileString(L"score", index, arrScore[1][i + 1], configPath);
			WritePrivateProfileString(L"name", index, arrScore[0][i + 1], configPath);
			break;
		}
		
	}
}

void DecreaseSeconds(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	seconds--; // Giảm đi một giây

	WCHAR buffer[3];
	wsprintf(buffer, L"%d", seconds);
	SetWindowText(lblSeconds, buffer); // Set text mới

	if (-1 == seconds)
	{
		
		if (count < 10)
		{
			loadGame(hwnd);
		}
		else
		{
			KillTimer(hwnd, IDT_TIMER); // Hủy bỏ timer
			click = 0;
			count = 0;
			printResultPlay(hwnd);
		}
		
	}
}