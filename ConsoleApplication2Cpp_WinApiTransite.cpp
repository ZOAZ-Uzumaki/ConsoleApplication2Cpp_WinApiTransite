#include <windows.h>
#include <strsafe.h>

wchar_t surname[100];
wchar_t name[100];
wchar_t patronymic[100];

#define ID_EDIT_SURNAME 101
#define ID_EDIT_NAME    102
#define ID_EDIT_PATRONYMIC 103
#define ID_BUTTON_NEXT   104
#define ID_LISTBOX       105

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

int main() {
    WNDCLASS wc = { 0 };

    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(NULL);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = L"MyWindowClass";

    if (!RegisterClass(&wc)) {
        MessageBox(NULL, L"Registration Failed!", L"Error", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    HWND hwnd = CreateWindow(L"MyWindowClass", L"Form 1", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 300, NULL, NULL, GetModuleHandle(NULL), NULL);

    if (!hwnd) {
        MessageBox(NULL, L"Window Creation Failed!", L"Error", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    ShowWindow(hwnd, SW_SHOWNORMAL);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    static HWND hwndList;

    switch (message) {
    case WM_CREATE: {
        CreateWindow(L"STATIC", L"Прізвище:", WS_VISIBLE | WS_CHILD, 10, 10, 100, 20, hwnd, NULL, NULL, NULL);
        CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 120, 10, 200, 20, hwnd, (HMENU)ID_EDIT_SURNAME, NULL, NULL);

        CreateWindow(L"STATIC", L"Ім'я:", WS_VISIBLE | WS_CHILD, 10, 40, 100, 20, hwnd, NULL, NULL, NULL);
        CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 120, 40, 200, 20, hwnd, (HMENU)ID_EDIT_NAME, NULL, NULL);

        CreateWindow(L"STATIC", L"По батькові:", WS_VISIBLE | WS_CHILD, 10, 70, 100, 20, hwnd, NULL, NULL, NULL);
        CreateWindow(L"EDIT", L"", WS_VISIBLE | WS_CHILD | WS_BORDER, 120, 70, 200, 20, hwnd, (HMENU)ID_EDIT_PATRONYMIC, NULL, NULL);

        CreateWindow(L"BUTTON", L"Далі", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 150, 100, 80, 25, hwnd, (HMENU)ID_BUTTON_NEXT, NULL, NULL);

        hwndList = CreateWindow(L"LISTBOX", L"", WS_VISIBLE | WS_CHILD | WS_BORDER | WS_VSCROLL | ES_AUTOVSCROLL,
            10, 130, 350, 120, hwnd, (HMENU)ID_LISTBOX, NULL, NULL);
        break;
    }

    case WM_COMMAND: {
        if (LOWORD(wParam) == ID_BUTTON_NEXT) {
            GetDlgItemText(hwnd, ID_EDIT_SURNAME, surname, sizeof(surname) / sizeof(wchar_t));
            GetDlgItemText(hwnd, ID_EDIT_NAME, name, sizeof(name) / sizeof(wchar_t));
            GetDlgItemText(hwnd, ID_EDIT_PATRONYMIC, patronymic, sizeof(patronymic) / sizeof(wchar_t));

            wchar_t data[300];
            StringCchPrintf(data, sizeof(data) / sizeof(data[0]), L"Прізвище: %s\nІм'я: %s\nПо батькові: %s", surname, name, patronymic);
            MessageBox(hwnd, data, L"Інформація", MB_OK);

            // Додавання інформації до списку
            SendMessage(hwndList, LB_ADDSTRING, 0, (LPARAM)data);
        }
        break;
    }

    case WM_DESTROY: {
        PostQuitMessage(0);
        break;
    }

    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}
