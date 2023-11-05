#include <windows.h>
#include <tchar.h>
#include <thread>
#include <mutex>

std::mutex mutex; // Мьютекс для синхронизации доступа к общей переменной
int counter = 0; // Общая переменная, которую будут изменять потоки
HWND hCounterLabel, hIncrementButton, hDecrementButton;

#define WM_UPDATE_COUNTER (WM_USER + 1) // Сообщение для обновления счетчика

void incrementCounter() {
    while (true) {
        {
            std::lock_guard<std::mutex> lock(mutex); // Захват мьютекса для безопасного доступа к counter
            counter++;
        }

        // Обновление метки счетчика
        PostMessage(hCounterLabel, WM_UPDATE_COUNTER, 0, 0);

        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Задержка для имитации работы
    }
}

void decrementCounter() {
    while (true) {
        {
            std::lock_guard<std::mutex> lock(mutex);
            counter--;
        }

        PostMessage(hCounterLabel, WM_UPDATE_COUNTER, 0, 0);

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void updateCounterLabel() {
    TCHAR buffer[256];
    _stprintf_s(buffer, _T("Counter: %d"), counter);
    SetWindowText(hCounterLabel, buffer);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE:
        hCounterLabel = CreateWindow(_T("STATIC"), _T(""), WS_VISIBLE | WS_CHILD, 10, 10, 200, 20, hWnd, nullptr, nullptr, nullptr);

        hIncrementButton = CreateWindow(_T("BUTTON"), _T("Increment"), WS_VISIBLE | WS_CHILD, 10, 40, 80, 20, hWnd, nullptr, nullptr, nullptr);
        hDecrementButton = CreateWindow(_T("BUTTON"), _T("Decrement"), WS_VISIBLE | WS_CHILD, 100, 40, 80, 20, hWnd, nullptr, nullptr, nullptr);

        // Обновление метки счетчика при создании окна
        updateCounterLabel();

        // Запуск потоков в отдельном потоке
        std::thread(incrementCounter).detach();
        std::thread(decrementCounter).detach();
        break;
    case WM_CLOSE:
        DestroyWindow(hWnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_UPDATE_COUNTER:
        // Обновление счетчика
        updateCounterLabel();
        break;
    case WM_COMMAND:
        if (lParam == (LPARAM)hIncrementButton) {
            // Инкремент
            std::lock_guard<std::mutex> lock(mutex);
            counter++;
            updateCounterLabel();
        }
        else if (lParam == (LPARAM)hDecrementButton) {
            // Декремент
            std::lock_guard<std::mutex> lock(mutex);
            counter--;
            updateCounterLabel();
        }
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.lpszClassName = _T("SyncApp");
    RegisterClassEx(&wcex);

    HWND hWnd = CreateWindow(_T("SyncApp"), _T("Synchronization Example"), WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 150, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd) {
        return false;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return static_cast<int>(msg.wParam);
}
