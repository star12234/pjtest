#include <windows.h>
#include <tchar.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CREATE:
            // "hello" 문구 출력
            CreateWindow("STATIC", "hello", WS_VISIBLE | WS_CHILD,
                         100, 40, 100, 20, hwnd, NULL, NULL, NULL); // 팝업창의 위치 (100,40) (100,20)
            // 5초 후에 타이머를 종료하도록 설정
            SetTimer(hwnd, 1, 5000, NULL);
            break;
        case WM_TIMER:
            // 타이머가 만료되면 창을 닫음
            PostQuitMessage(0);
            break;
        case WM_DESTROY:
            KillTimer(hwnd, 1);
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    const char CLASS_NAME[] = "PopupWindowClass";

    WNDCLASS wc = {0};

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // 윈도우 생성
    HWND hwnd = CreateWindowEx(
        WS_EX_TOPMOST | WS_EX_TOOLWINDOW, // 창이 항상 최상단에 위치
        CLASS_NAME,
        "Popup", // 창 제목
        WS_POPUP | WS_VISIBLE, // 팝업 창 스타일
        GetSystemMetrics(SM_CXSCREEN) - 320, GetSystemMetrics(SM_CYSCREEN) - 150, // 화면 오른쪽 하단에 위치
        300, 100, // 크기: 300x100
        NULL, NULL, hInstance, NULL);

    if (hwnd == NULL)
    {
        return 0;
    }

    // 메시지 루프
    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

void AddToStartup()
{
    // 레지스트리 키 경로
    HKEY hKey;
    const char* czStartName = "MyPopupApp";  // 시작 프로그램에 등록될 이름
    const char* czExePath = "C:\\path\\to\\your\\program.exe";  // 실행할 프로그램의 경로

    // 레지스트리에서 Run 키 열기
    if (RegOpenKey(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", &hKey) == ERROR_SUCCESS)
    {
        // 프로그램 경로를 레지스트리에 추가
        RegSetValueEx(hKey, czStartName, 0, REG_SZ, (BYTE*)czExePath, strlen(czExePath) + 1);
        RegCloseKey(hKey);
    }
}
