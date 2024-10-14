#include <windows.h>
#include <stdio.h>

#define UNICODE  // 유니코드 모드 사용

// 창 프로시저 (WindowProcedure)
LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // 알림창 안에 출력하고자 하는 메세지
        const wchar_t* text = L"원하는 메세지";

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

void AnimateWindowUp(HWND hwnd, int startPosX, int startPosY, int endPosY, int speed) {
    // 창을 startPosY에서 endPosY까지 천천히 이동시키는 함수
    for (int posY = startPosY; posY > endPosY; posY -= speed) {
        SetWindowPos(hwnd, NULL, startPosX, posY, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
        Sleep(10);  // 창이 움직이는 속도 조절 (값을 조정해 애니메이션 속도를 변경 가능)
    }
}

int main() {
 
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

    // 창의 시작 위치 (오른쪽 아래 화면 바깥)
    int startPosX = screenWidth - windowWidth;  // 오른쪽 모서리
    int startPosY = screenHeight;  // 화면 바깥 아래쪽
    int endPosY = screenHeight - windowHeight;  // 화면 아래에서 창의 높이만큼 위

    // 윈도우 창 생성
    HWND hwnd = CreateWindow(L"MyWindowClass", L"원하는 알림창의 제목", WS_OVERLAPPEDWINDOW,  //알림창에서의 제목
        startPosX, startPosY, windowWidth, windowHeight, NULL, NULL, hInstance, NULL);

    if (hwnd == NULL) {
        MessageBox(NULL, L"윈도우 창 생성 실패!", L"오류", MB_ICONEXCLAMATION | MB_OK);  // 유니코드 문자열 사용
        return 1;
    }

    // 창 표시 및 업데이트
    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    // 창을 아래에서 위로 천천히 이동시키는 애니메이션 실행
    AnimateWindowUp(hwnd, startPosX, startPosY, endPosY, 5);

    // 메시지 루프
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
} 

//main
