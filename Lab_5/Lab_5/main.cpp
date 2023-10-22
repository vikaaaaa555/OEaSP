#include "main.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//
void MonitorVolumeChanges();
void OnVolumeChange(float);
void CALLBACK TimerProc(HWND, UINT, UINT_PTR, DWORD);
//

HWND hWnd; // Глобальная переменная для хранения дескриптора окна
COLORREF currentColor = RGB(178, 132, 190);
COLORREF targetColor = RGB(178, 132, 190);
const int timerInterval = 100; // Интервал таймера в миллисекундах

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    //wcex.hbrBackground = CreateSolidBrush(RGB(178, 132, 190));
    wcex.lpszClassName = L"Volume Monitor";
    RegisterClassEx(&wcex);

    hWnd = CreateWindow(L"Volume Monitor", L"Volume Monitor", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 1000, 1000, nullptr, nullptr, hInstance, nullptr);

    if (!hWnd) {
        return false;
    }

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // Создаем отдельный поток для мониторинга громкости
    std::thread monitorThread(MonitorVolumeChanges);

    // Запускаем таймер для плавного изменения цвета фона
    SetTimer(hWnd, 1, timerInterval, nullptr);

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Завершаем поток и закрываем программу
    KillTimer(hWnd, 1); // Останавливаем таймер перед выходом
    monitorThread.detach();
    return static_cast<int>(msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CLOSE:
        DestroyWindow(hWnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_TIMER:
        TimerProc(hWnd, WM_TIMER, 1, GetTickCount64());
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void OnVolumeChange(float newVolume) {
    // Пример: увеличьте ширину и высоту окна на значение громкости
    int newSize = static_cast<int>(600 * newVolume);

    // Установить новый размер окна
    SetWindowPos(hWnd, nullptr, 0, 0, newSize, newSize, SWP_NOMOVE | SWP_NOZORDER);

    // Обновить целевой цвет в зависимости от уровня громкости
    int red = static_cast<int>(255 * newVolume);
    //int blue = static_cast<int>(255 * (1.0f - newVolume));
    targetColor = RGB(red, 132, 190);

    // Запустить таймер для плавного изменения цвета
    SetTimer(hWnd, 1, 100, nullptr);
}

void MonitorVolumeChanges() {
    CoInitialize(nullptr);

    IMMDeviceEnumerator* enumerator = nullptr;
    HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), reinterpret_cast<void**>(&enumerator));

    if (SUCCEEDED(hr)) {
        IMMDevice* defaultDevice = nullptr;
        hr = enumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);

        if (SUCCEEDED(hr)) {
            IAudioEndpointVolume* endpointVolume = nullptr;
            hr = defaultDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, nullptr, reinterpret_cast<void**>(&endpointVolume));

            if (SUCCEEDED(hr)) {
                while (true) {
                    float currentVolume = 0.0f;
                    hr = endpointVolume->GetMasterVolumeLevelScalar(&currentVolume);

                    if (SUCCEEDED(hr)) {
                        OnVolumeChange(currentVolume);
                    }

                    Sleep(1000); // Пауза на 1 секунду
                }
                endpointVolume->Release();
            }
            defaultDevice->Release();
        }
        enumerator->Release();
    }
    CoUninitialize();
}

void CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime) {
    // Изменить текущий цвет в сторону целевого цвета
    int rDiff = (GetRValue(targetColor) - GetRValue(currentColor)) / 10;
    int gDiff = (GetGValue(targetColor) - GetGValue(currentColor)) / 10;
    int bDiff = (GetBValue(targetColor) - GetBValue(currentColor)) / 10;

    currentColor = RGB(GetRValue(currentColor) + rDiff,
        GetGValue(currentColor) + gDiff,
        GetBValue(currentColor) + bDiff);

    // Установить новый цвет фона окна
    HBRUSH hBrush = CreateSolidBrush(currentColor);
    SetClassLongPtr(hwnd, GCLP_HBRBACKGROUND, reinterpret_cast<LONG_PTR>(hBrush));
    InvalidateRect(hwnd, nullptr, TRUE);

    // Если достигнут целевой цвет, остановить таймер
    if (currentColor == targetColor) {
        KillTimer(hwnd, 1);
    }
}