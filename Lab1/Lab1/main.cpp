﻿#include <Windows.h>
#include <string>
#include <cmath>

#define PI 3.14159265358979323846

#define ID_EDIT     1000

#define ID_BUTTON_0 1001
#define ID_BUTTON_1 1002
#define ID_BUTTON_2 1003
#define ID_BUTTON_3 1004
#define ID_BUTTON_4 1005
#define ID_BUTTON_5 1006
#define ID_BUTTON_6 1007
#define ID_BUTTON_7 1008
#define ID_BUTTON_8 1009
#define ID_BUTTON_9 1010

#define ID_BUTTON_ADD   1011
#define ID_BUTTON_SUBTRACT 1012
#define ID_BUTTON_MULTIPLY 1013
#define ID_BUTTON_DIVIDE 1014
#define ID_BUTTON_EQUALS 1015
#define ID_BUTTON_CLEAR 1016
#define ID_BUTTON_SQUARE_ROOT 1017
#define ID_BUTTON_DECIMAL 1018

#define ID_BUTTON_SIN 1019
#define ID_BUTTON_COS 1020
#define ID_BUTTON_TAN 1021
#define ID_BUTTON_COT 1022
#define ID_BUTTON_DEGREES 1023

#define ID_EDIT_RED 1024
#define ID_EDIT_GREEN 1025
#define ID_EDIT_BLUE 1026
#define ID_BUTTON_CHANGE 1027


LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
double Trigonometric(const std::wstring& functionName, double value);

HWND g_hEdit, hActiveWindow;
HHOOK hKeyboardHook;
std::wstring g_inputText;
double g_currentValue = 0.0;
bool g_isNewInput = true;
bool g_decimalEntered = false;
std::wstring g_operator; // Хранение текущего оператора.
COLORREF colour = RGB(178, 132, 190);

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0) {
        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
            KBDLLHOOKSTRUCT* pKbStruct = (KBDLLHOOKSTRUCT*)lParam;

            if (pKbStruct->scanCode == 0x1C) {
                hActiveWindow = GetForegroundWindow();

                HWND hButton = GetDlgItem(hActiveWindow, ID_BUTTON_EQUALS);

                if (hButton != NULL) {
                    SendMessage(hButton, BM_CLICK, 0, 0);
                }
            }

            else if (pKbStruct->scanCode == 0x0C) {
                hActiveWindow = GetForegroundWindow();

                HWND hButton = GetDlgItem(hActiveWindow, ID_BUTTON_SUBTRACT);

                if (hButton != NULL) {
                    SendMessage(hButton, BM_CLICK, 0, 0);
                }
            }

            else if (pKbStruct->scanCode == 0x35) {
                hActiveWindow = GetForegroundWindow();

                HWND hButton = GetDlgItem(hActiveWindow, ID_BUTTON_DIVIDE);

                if (hButton != NULL) {
                    SendMessage(hButton, BM_CLICK, 0, 0);
                }
            }

            else if (pKbStruct->scanCode == 0x34) {
                hActiveWindow = GetForegroundWindow();

                HWND hButton = GetDlgItem(hActiveWindow, ID_BUTTON_DECIMAL);

                if (hButton != NULL) {
                    SendMessage(hButton, BM_CLICK, 0, 0);
                }
            }

            else if (GetAsyncKeyState(0x10) && GetAsyncKeyState(0xBB) && GetAsyncKeyState(0x10) && GetAsyncKeyState(0xBB)) {
                hActiveWindow = GetForegroundWindow();

                HWND hButton = GetDlgItem(hActiveWindow, ID_BUTTON_ADD);

                if (hButton != NULL) {
                    SendMessage(hButton, BM_CLICK, 0, 0);
                }
            }
        }
    }

    return CallNextHookEx(hKeyboardHook, nCode, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    WNDCLASSEX wcex;
    memset(&wcex, 0, sizeof(WNDCLASSEX));
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = CreateSolidBrush(RGB(255, 192, 203));
    wcex.lpszClassName = L"Calculator";
    RegisterClassEx(&wcex);

    HWND hWnd = CreateWindow(L"Calculator", L"Calculator", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 525, nullptr, nullptr, hInstance, nullptr);

    HHOOK hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, hInstance, 0);

    if (!hWnd)
    {
        return false;
    }

    g_hEdit = CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_RIGHT | ES_READONLY,
        10, 10, 360, 50, hWnd, (HMENU)ID_EDIT, hInstance, nullptr);

    CreateWindow(L"EDIT", L"178", WS_CHILD | WS_VISIBLE , 10, 460, 115, 15, hWnd, (HMENU)ID_EDIT_RED, hInstance, nullptr);
    CreateWindow(L"EDIT", L"132", WS_CHILD | WS_VISIBLE , 130, 460, 115, 15, hWnd, (HMENU)ID_EDIT_GREEN, hInstance, nullptr);
    CreateWindow(L"EDIT", L"190", WS_CHILD | WS_VISIBLE , 250, 460, 115, 15, hWnd, (HMENU)ID_EDIT_BLUE, hInstance, nullptr);
    //CreateWindow(L"BUTTON", L"change", WS_CHILD | WS_VISIBLE, 280, 460, 80, 15, hWnd, (HMENU)ID_BUTTON_CHANGE, hInstance, nullptr);

    CreateWindow(L"BUTTON", L"0", WS_CHILD | WS_VISIBLE, 100, 400, 80, 50, hWnd, (HMENU)ID_BUTTON_0, hInstance, nullptr);
    CreateWindow(L"BUTTON", L"1", WS_CHILD | WS_VISIBLE, 10, 340, 80, 50, hWnd, (HMENU)ID_BUTTON_1, hInstance, nullptr);
    CreateWindow(L"BUTTON", L"2", WS_CHILD | WS_VISIBLE, 100, 340, 80, 50, hWnd, (HMENU)ID_BUTTON_2, hInstance, nullptr);
    CreateWindow(L"BUTTON", L"3", WS_CHILD | WS_VISIBLE, 190, 340, 80, 50, hWnd, (HMENU)ID_BUTTON_3, hInstance, nullptr);
    CreateWindow(L"BUTTON", L"4", WS_CHILD | WS_VISIBLE, 10, 280, 80, 50, hWnd, (HMENU)ID_BUTTON_4, hInstance, nullptr);
    CreateWindow(L"BUTTON", L"5", WS_CHILD | WS_VISIBLE, 100, 280, 80, 50, hWnd, (HMENU)ID_BUTTON_5, hInstance, nullptr);
    CreateWindow(L"BUTTON", L"6", WS_CHILD | WS_VISIBLE, 190, 280, 80, 50, hWnd, (HMENU)ID_BUTTON_6, hInstance, nullptr);
    CreateWindow(L"BUTTON", L"7", WS_CHILD | WS_VISIBLE, 10, 220, 80, 50, hWnd, (HMENU)ID_BUTTON_7, hInstance, nullptr);
    CreateWindow(L"BUTTON", L"8", WS_CHILD | WS_VISIBLE, 100, 220, 80, 50, hWnd, (HMENU)ID_BUTTON_8, hInstance, nullptr);
    CreateWindow(L"BUTTON", L"9", WS_CHILD | WS_VISIBLE, 190, 220, 80, 50, hWnd, (HMENU)ID_BUTTON_9, hInstance, nullptr);
    CreateWindow(L"BUTTON", L".", WS_CHILD | WS_VISIBLE, 10, 400, 80, 50, hWnd, (HMENU)ID_BUTTON_DECIMAL, hInstance, nullptr);

    CreateWindow(L"BUTTON", L"+", WS_CHILD | WS_VISIBLE, 280, 400, 80, 50, hWnd, (HMENU)ID_BUTTON_ADD, hInstance, nullptr);
    CreateWindow(L"BUTTON", L"-", WS_CHILD | WS_VISIBLE, 280, 340, 80, 50, hWnd, (HMENU)ID_BUTTON_SUBTRACT, hInstance, nullptr);
    CreateWindow(L"BUTTON", L"*", WS_CHILD | WS_VISIBLE, 280, 280, 80, 50, hWnd, (HMENU)ID_BUTTON_MULTIPLY, hInstance, nullptr);
    CreateWindow(L"BUTTON", L"/", WS_CHILD | WS_VISIBLE, 280, 220, 80, 50, hWnd, (HMENU)ID_BUTTON_DIVIDE, hInstance, nullptr);

    CreateWindow(L"BUTTON", L"=", WS_CHILD | WS_VISIBLE, 190, 400, 80, 50, hWnd, (HMENU)ID_BUTTON_EQUALS, hInstance, nullptr);
    CreateWindow(L"BUTTON", L"C", WS_CHILD | WS_VISIBLE, 280, 100, 80, 50, hWnd, (HMENU)ID_BUTTON_CLEAR, hInstance, nullptr);
    CreateWindow(L"BUTTON", L"√", WS_CHILD | WS_VISIBLE, 190, 100, 80, 50, hWnd, (HMENU)ID_BUTTON_SQUARE_ROOT, hInstance, nullptr);
    CreateWindow(L"BUTTON", L"%", WS_CHILD | WS_VISIBLE, 100, 100, 80, 50, hWnd, (HMENU)ID_BUTTON_DEGREES, hInstance, nullptr);

    CreateWindow(L"BUTTON", L"sin", WS_CHILD | WS_VISIBLE, 10, 160, 80, 50, hWnd, (HMENU)ID_BUTTON_SIN, hInstance, nullptr);
    CreateWindow(L"BUTTON", L"cos", WS_CHILD | WS_VISIBLE, 100, 160, 80, 50, hWnd, (HMENU)ID_BUTTON_COS, hInstance, nullptr);
    CreateWindow(L"BUTTON", L"tg", WS_CHILD | WS_VISIBLE, 190, 160, 80, 50, hWnd, (HMENU)ID_BUTTON_TAN, hInstance, nullptr);
    CreateWindow(L"BUTTON", L"ctg", WS_CHILD | WS_VISIBLE, 280, 160, 80, 50, hWnd, (HMENU)ID_BUTTON_COT, hInstance, nullptr);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case ID_BUTTON_0:
        case ID_BUTTON_1:
        case ID_BUTTON_2:
        case ID_BUTTON_3:
        case ID_BUTTON_4:
        case ID_BUTTON_5:
        case ID_BUTTON_6:
        case ID_BUTTON_7:
        case ID_BUTTON_8:
        case ID_BUTTON_9:
            if (g_isNewInput)
            {
                g_inputText.clear();
                g_isNewInput = false;
            }
            g_inputText += wchar_t('0' + LOWORD(wParam) - ID_BUTTON_0);
            SetWindowText(g_hEdit, g_inputText.c_str());
            break;

        case ID_BUTTON_ADD:
        case ID_BUTTON_SUBTRACT:
        case ID_BUTTON_MULTIPLY:
        case ID_BUTTON_DIVIDE:
            if (!g_isNewInput)
            {
                // Если предыдущий ввод был числом, сбросьте флаг десятичной точки.
                if (g_inputText.find(L".") != std::wstring::npos)
                {
                    g_decimalEntered = false;
                }

                if (!g_inputText.empty()) // Если ввод начинается с оператора, добавляем 0.
                {
                    g_currentValue = std::wcstod(g_inputText.c_str(), nullptr);
                    g_inputText.clear();
                    g_isNewInput = true;
                }
            }
            switch (LOWORD(wParam))
            {
            case ID_BUTTON_ADD:
                g_operator = L"+";
                break;
            case ID_BUTTON_SUBTRACT:
                g_operator = L"-";
                break;
            case ID_BUTTON_MULTIPLY:
                g_operator = L"*";
                break;
            case ID_BUTTON_DIVIDE:
                g_operator = L"/";
                break;
            }
            break;

        case ID_BUTTON_EQUALS:
            if (!g_isNewInput)
            {
                if (!g_inputText.empty()) // Учитываем второе число.
                {
                    double newValue = std::wcstod(g_inputText.c_str(), nullptr);
                    if (g_operator == L"+")
                        g_currentValue += newValue;
                    else if (g_operator == L"-")
                        g_currentValue -= newValue;
                    else if (g_operator == L"*")
                        g_currentValue *= newValue;
                    else if (g_operator == L"/")
                    {
                        if (newValue != 0.0)
                            g_currentValue /= newValue;
                        else
                        {
                            MessageBox(hWnd, L"Cannot divide by zero.", L"Error", MB_OK | MB_ICONERROR);
                            g_currentValue = 0.0;
                        }
                    }

                    // Преобразование в строку.
                    g_inputText = std::to_wstring(g_currentValue);

                    // Цикл удаления нулей до числа или точки.
                    size_t pos = g_inputText.length() - 1;
                    bool pnt = false;
                    while (pos--)
                    {
                        if (g_inputText[pos] == L'.') pnt = true;
                    }
                    pos = g_inputText.length();
                    if (pnt) {
                        while (pos > 0 && (g_inputText[pos - 1] == L'0' || g_inputText[pos - 1] == L'.'))
                        {
                            pos--;
                            if (g_inputText[pos] == L'.')break;
                        }
                        g_inputText.resize(pos);
                    }
                    
                    g_isNewInput = true;
                    SetWindowText(g_hEdit, g_inputText.c_str());
                }
            }
            break;

        case ID_BUTTON_SIN:
        case ID_BUTTON_COS:
        case ID_BUTTON_TAN:
        case ID_BUTTON_COT:
        {
            double newValue = std::wcstod(g_inputText.c_str(), nullptr);
            switch (LOWORD(wParam))
            {
            case ID_BUTTON_SIN:
                g_operator = L"sin";
                break;
            case ID_BUTTON_COS:
                g_operator = L"cos";
                break;
            case ID_BUTTON_TAN:
                g_operator = L"tg";
                break;
            case ID_BUTTON_COT:
                g_operator = L"ctg";
                break;
            }

            double result = Trigonometric(g_operator, newValue);
            g_currentValue = result;
            g_inputText = std::to_wstring(result);
            g_isNewInput = true;
            SetWindowText(g_hEdit, g_inputText.c_str());
        }
            break;

        case ID_BUTTON_DEGREES:
            if (!g_isNewInput)
            {
                double newValue = std::wcstod(g_inputText.c_str(), nullptr);
                    g_currentValue = newValue / 100;
                    g_inputText = std::to_wstring(g_currentValue);
                    g_isNewInput = true;
                    SetWindowText(g_hEdit, g_inputText.c_str());
            }
            break;

        case ID_BUTTON_DECIMAL:
            // Проверяем, что десятичная точка ещё не была введена.
            if (!g_decimalEntered)
            {
                g_inputText += L"."; // Добавляем десятичную точку к введённому тексту.
                g_decimalEntered = true; // Устанавливаем флаг, что десятичная точка введена.
                SetWindowText(g_hEdit, g_inputText.c_str());
            }
            break;

        case ID_BUTTON_CLEAR:
            g_inputText.clear();
            g_currentValue = 0.0;
            g_isNewInput = true;
            g_decimalEntered = false;
            g_operator.clear(); // Очистка текущего оператора.
            SetWindowText(g_hEdit, L"");
            break;

        case ID_BUTTON_SQUARE_ROOT:
            double newValue = std::wcstod(g_inputText.c_str(), nullptr);
            if (newValue >= 0.0)
            {
                g_currentValue = sqrt(newValue);
                g_inputText = std::to_wstring(g_currentValue);
                g_isNewInput = true;
                SetWindowText(g_hEdit, g_inputText.c_str());
            }
            else
            {
                MessageBox(hWnd, L"Invalid input for square root.", L"Error", MB_OK | MB_ICONERROR);
                g_currentValue = 0.0;
                g_inputText.clear();
                SetWindowText(g_hEdit, L"");
            }
            break;
        }
        case ID_BUTTON_CHANGE:
        {
            colour = RGB(GetDlgItemInt(hWnd, ID_EDIT_RED, nullptr, false),
                GetDlgItemInt(hWnd, ID_EDIT_GREEN, nullptr, false),
                GetDlgItemInt(hWnd, ID_EDIT_BLUE, nullptr, false));
            RedrawWindow(hWnd, nullptr, nullptr, RDW_UPDATENOW | RDW_INVALIDATE);
        }
            break;

        break;

    case WM_CTLCOLOREDIT:
    {
        HDC hdc = (HDC)wParam;
        SetBkColor(hdc, colour);
        SetTextColor(hdc, RGB(171, 39, 79));
    }
        break;

    case WM_CTLCOLORSTATIC:
    {
        HDC hdc = (HDC)wParam;
        SetBkColor(hdc, colour);
        //SetBkColor(hdc, RGB(178, 132, 190));
        SetTextColor(hdc, RGB(171, 39, 79));
        return (INT_PTR)CreateSolidBrush(colour);
    }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

double Trigonometric(const std::wstring& functionName, double value)
{
    if (functionName == L"sin")
    {
        return sin(value * PI / 180);
    }
    else if (functionName == L"cos")
    {
        return cos(value * PI / 180);
    }
    else if (functionName == L"tg")
    {
        return tan(value * PI / 180);
    }
    else if (functionName == L"ctg")
    {
        return 1.0 / tan(value * PI / 180);
    }
    else return 0.0;
}


