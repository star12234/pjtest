
#include <windows.h>
#include <stdio.h>

#define UNICODE  // 유니코드 모드 사용

// 창 프로시저 (WindowProcedure)
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // 화면에 출력할 텍스트
        const wchar_t* text = L"이것은 한글로 된 알림 메시지입니다.";

        // 텍스트 크기 측정
        SIZE textSize;
        GetTextExtentPoint32(hdc, text, wcslen(text), &textSize);

        // 창 크기 가져오기
        RECT clientRect;
        GetClientRect(hwnd, &clientRect);

        // 텍스트를 중앙에 맞추기 위한 좌표 계산
        int x = (clientRect.right - textSize.cx) / 2;  // 가로 중앙
        int y = (clientRect.bottom - textSize.cy) / 2; // 세로 중앙

        // 텍스트 출력
        TextOut(hdc, x, y, text, wcslen(text));

        EndPaint(hwnd, &ps);
        break;
    }
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}

int main() {
    // 콘솔 창 출력 (유니코드로 설정된 콘솔에서 한글이 정상 출력될 수 있습니다)
    wprintf(L"윈도우 창을 생성합니다...\n");

    // 윈도우 클래스 설정
    HINSTANCE hInstance = GetModuleHandle(NULL); // 현재 실행 중인 모듈 핸들 얻기
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WindowProcedure; // 창 프로시저 연결
    wc.hInstance = hInstance;
    wc.lpszClassName = L"MyWindowClass";  // 유니코드 문자열 사용

    // 윈도우 클래스 등록
    if (!RegisterClass(&wc)) {
        MessageBox(NULL, L"윈도우 클래스 등록 실패!", L"오류", MB_ICONEXCLAMATION | MB_OK);  // 유니코드 문자열 사용
        return 1;
    }

    // 화면 해상도 얻기
    int screenWidth = GetSystemMetrics(SM_CXSCREEN); // 화면의 가로 길이
    int screenHeight = GetSystemMetrics(SM_CYSCREEN); // 화면의 세로 길이

    // 창 크기 설정
    int windowWidth = 400;
    int windowHeight = 200;

    // 창을 화면의 가운데에 배치
    int posX = (screenWidth - windowWidth); // 가로 중앙 좌표
    int posY = (screenHeight - windowHeight); // 세로 중앙 좌표

    // 윈도우 창 생성
    HWND hwnd = CreateWindow(L"MyWindowClass", L"한글 알림창", WS_OVERLAPPEDWINDOW,  // 유니코드 문자열 사용
        posX, posY, windowWidth, windowHeight, NULL, NULL, hInstance, NULL);

    if (hwnd == NULL) {
        MessageBox(NULL, L"윈도우 창 생성 실패!", L"오류", MB_ICONEXCLAMATION | MB_OK);  // 유니코드 문자열 사용
        return 1;
    }

    // 창 표시 및 업데이트
    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    // 메시지 루프
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

/*
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
*/

