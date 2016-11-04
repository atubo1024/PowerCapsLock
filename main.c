
#define WINVER          0x0500
#include <Windows.h>
#include <stdio.h>
#include <stdint.h>

#ifdef DEBUG 
	#define trace printf
#else
	#define trace(...) (void)0
#endif

#define APPNAME "PowerCapsLock"

static HHOOK mHook = NULL;
static WPARAM mCapKeyStatus = WM_KEYUP;

static uint8_t mPassthroughOnce = 0;
static uint8_t mIgnoreCapitalKey = 0;

static void SendKey(WPARAM wParam, WORD wVk)
{
    INPUT input = {0};
	if (wParam == WM_KEYUP) input.ki.dwFlags = KEYEVENTF_KEYUP;
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = wVk;
    input.ki.wScan = MapVirtualKeyExW(wVk, 0, 0);
    SendInput(1, &input, sizeof(INPUT));
}
/**
 * @param wParam  WM_KEYDOWN, WM_KEYUP, WM_SYSKEYDOWN, or WM_SYSKEYUP.
 * @param lParam  A pointer to a KBDLLHOOKSTRUCT structure.
 */
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) 
{
	KBDLLHOOKSTRUCT *p;
	DWORD vkCode;
	
	if (nCode >= 0) {
		p = (KBDLLHOOKSTRUCT *) lParam;
		trace("nCode=%d, wParam=%d, lParam=0x%ld, vk=0x%02x, sc=0x%02x\n", 
				nCode, wParam, lParam, p->vkCode, p->scanCode);
		vkCode = p->vkCode;
		if (mPassthroughOnce) {
			trace("passthrough any key\n");
			mPassthroughOnce = 0;
		} else if (vkCode == VK_CAPITAL) {
			mCapKeyStatus = wParam;
			if (wParam == WM_KEYUP) {
				if (mIgnoreCapitalKey) {
					mIgnoreCapitalKey = 0;
				} else {
					trace("send virtual capital key down\n");
					/** keydown会触发大写键切换 */
					mPassthroughOnce = 1;
					SendKey(WM_KEYDOWN, vkCode);
					/* sleep is not needed. */
					/* Sleep(100); */
					mPassthroughOnce = 1;
					SendKey(WM_KEYUP, vkCode);
				}
			}
			return 1;
		} else if (mCapKeyStatus == WM_KEYDOWN) {
			mIgnoreCapitalKey = 1;
			/** replace vkCode and scanCode not work, intercepting it and resend an INPUT event. */
			switch (vkCode)
			{
			case 'H':
				trace("H\n");
				mPassthroughOnce = 1;
				SendKey(wParam, VK_LEFT);
				break;
			case 'J':
				trace("J\n");
				mPassthroughOnce = 1;
				SendKey(wParam, VK_DOWN);
				break;
			case 'K':
				trace("K\n");
				mPassthroughOnce = 1;
				SendKey(wParam, VK_UP);
				break;
			case 'L':
				trace("L\n");
				mPassthroughOnce = 1;
				SendKey(wParam, VK_RIGHT);
				break;
			case VK_SPACE:
				if (wParam == WM_KEYUP) {
					trace("cap+space\n");
					mPassthroughOnce = 1;
					SendKey(WM_KEYDOWN, VK_LWIN);
					mPassthroughOnce = 1;
					SendKey(WM_KEYDOWN, VK_SPACE);
					mPassthroughOnce = 1;
					SendKey(WM_KEYUP, VK_SPACE);
					mPassthroughOnce = 1;
					SendKey(WM_KEYUP, VK_LWIN);
				}
				break;
			default:
				if (wParam == WM_KEYUP) {
					trace("convert capital to ctrl\n");
					mPassthroughOnce = 1;
					SendKey(WM_KEYDOWN, VK_LCONTROL);
					mPassthroughOnce = 1;
					SendKey(WM_KEYDOWN, vkCode);
					mPassthroughOnce = 1;
					SendKey(WM_KEYUP, vkCode);
					mPassthroughOnce = 1;
					SendKey(WM_KEYUP, VK_LCONTROL);
				}
				break;
			}
			return 1;
		}
	}
	return CallNextHookEx(mHook, nCode, wParam, lParam);
}

LRESULT CALLBACK MainLoop(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch(message)
    {
        case WM_CREATE:
            return 0;
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProc( hwnd, message, wParam, lParam );
}
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
    static TCHAR szAppName[] = TEXT(APPNAME);
	HWND hwnd;
    MSG msg;
    WNDCLASS wndclass;
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = MainLoop;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon( NULL, IDI_APPLICATION );
    wndclass.hCursor = LoadCursor( NULL, IDC_ARROW );
    wndclass.hbrBackground = (HBRUSH)GetStockObject( WHITE_BRUSH );
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szAppName;
    RegisterClass( &wndclass );
    hwnd = CreateWindow(szAppName,
						 szAppName,
                         WS_OVERLAPPEDWINDOW,
                         CW_USEDEFAULT,
                         CW_USEDEFAULT,
                         CW_USEDEFAULT,
                         CW_USEDEFAULT,
                         NULL,
                         NULL,
                         hInstance,
                         NULL );
    SetProcessWorkingSetSize(GetCurrentProcess(), -1, -1);

	mHook = SetWindowsHookEx( 
        WH_KEYBOARD_LL, 
        KeyboardProc,
        hInstance,
		0
    ); 

    while( GetMessage( &msg, NULL, 0, 0 ) )
    {
        TranslateMessage( &msg );
        DispatchMessage( &msg );
    }
    return msg.wParam;
}
