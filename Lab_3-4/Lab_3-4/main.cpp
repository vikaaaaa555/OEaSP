#include "main.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
HWND hwndListView = nullptr;
void UpdateProcessList(HWND);
void EnumThreadsInProcess(DWORD, DWORD&);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSEX wcex = { sizeof(WNDCLASSEX) };
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.lpszClassName = L"Processes Monitor";
    RegisterClassEx(&wcex);

    HWND hWnd = CreateWindow(L"Processes Monitor", L"Processes Monitor", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 400, nullptr, nullptr, hInstance, nullptr);

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

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE: {
        // Создать ListView для отображения списка процессов
        hwndListView = CreateWindow(WC_LISTVIEW, L"", WS_VISIBLE | WS_CHILD | LVS_REPORT,
            10, 10, 480, 300, hWnd, (HMENU)ID_LISTVIEW, nullptr, nullptr);

        
        HWND hwndButton = CreateWindow(L"BUTTON", L"End Task", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            10, 325, 80, 25, hWnd, (HMENU)ID_BUTTON_ENDTASK, GetModuleHandle(NULL), NULL);


        // Инициализировать ListView
        LVCOLUMN lvColumn = { 0 };
        lvColumn.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;

        lvColumn.pszText = const_cast<LPWSTR>(L""); // Приведем к типу LPWSTR
        lvColumn.cx = 20;
        ListView_InsertColumn(hwndListView, 0, &lvColumn);

        lvColumn.pszText = const_cast<LPWSTR>(L"Process"); // Приведем к типу LPWSTR
        lvColumn.cx = 150;
        ListView_InsertColumn(hwndListView, 1, &lvColumn);

        lvColumn.pszText = const_cast<LPWSTR>(L"PID"); // Приведем к типу LPWSTR
        lvColumn.cx = 50;
        ListView_InsertColumn(hwndListView, 2, &lvColumn);

        lvColumn.pszText = const_cast<LPWSTR>(L"Memory (KB)"); // Приведем к типу LPWSTR
        lvColumn.cx = 90;
        ListView_InsertColumn(hwndListView, 3, &lvColumn);

        lvColumn.pszText = const_cast<LPWSTR>(L"Threads"); // Приведем к типу LPWSTR
        lvColumn.cx = 60;
        ListView_InsertColumn(hwndListView, 4, &lvColumn);

        ListView_SetExtendedListViewStyle(hwndListView, LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT);

        // Обновить список процессов
        UpdateProcessList(hwndListView);
        break;
    }

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case ID_BUTTON_ENDTASK:
            // Пройдите по всем элементам ListView и найдите выделенный
            int index = -1;
            for (int i = 0; i < ListView_GetItemCount(hwndListView); ++i) {
                if (ListView_GetCheckState(hwndListView, i)) {
                    index = i;
                    break;
                }
            }

            if (index != -1) {
                // Получите PID выбранного процесса
                wchar_t buffer[256];
                ListView_GetItemText(hwndListView, index, 2, buffer, sizeof(buffer));
                DWORD pid = _wtoi(buffer);

                // Отправьте сигнал завершения процесса
                HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
                if (hProcess != NULL) {
                    TerminateProcess(hProcess, 0);
                    CloseHandle(hProcess);

                    // Удалите элемент из ListView
                    ListView_DeleteItem(hwndListView, index);

                    // Обновить список процессов
                    UpdateProcessList(hwndListView);
                }
            }
            break;
        }
        break;

    case WM_CLOSE:
        DestroyWindow(hWnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void UpdateProcessList(HWND hwndListView) {
    // Очистить текущий список процессов
    ListView_DeleteAllItems(hwndListView);

    // Получить список процессов
    DWORD processIds[1024], cbNeeded, cProcesses;
    if (EnumProcesses(processIds, sizeof(processIds), &cbNeeded)) {
        // Вычислить количество процессов
        cProcesses = cbNeeded / sizeof(DWORD);

        // Для каждого процесса
        for (DWORD i = 0; i < cProcesses; ++i) {
            // Открыть процесс
            HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processIds[i]);

            if (hProcess != nullptr) {
                // Получить модуль процесса
                HMODULE hMod;
                DWORD cbNeeded;

                if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded)) {
                    // Получить базовое имя модуля (это имя исполняемого файла)
                    TCHAR szProcessName[MAX_PATH];
                    GetModuleBaseName(hProcess, hMod, szProcessName, sizeof(szProcessName) / sizeof(TCHAR));

                    // Получить использование памяти
                    PROCESS_MEMORY_COUNTERS pmc;
                    if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {
                        DWORD pid = GetProcessId(hProcess);

                        DWORD threadCount = 0;
                        EnumThreadsInProcess(pid, threadCount);

                        // Получить временные данные процесса
                        FILETIME creationTime, exitTime, kernelTime, userTime;
                        GetProcessTimes(hProcess, &creationTime, &exitTime, &kernelTime, &userTime);

                        // Добавить элемент в список
                        LVITEM lvItem = { 0 };
                        lvItem.mask = LVIF_TEXT;
                        lvItem.iItem = i;
                        lvItem.iSubItem = 0;

                        int itemIndex = ListView_InsertItem(hwndListView, &lvItem);

                        // Установить радиокнопку в 0 колонку
                        ListView_SetCheckState(hwndListView, itemIndex, 0, FALSE);

                        // Установить имя процесса в 1 колонку
                        ListView_SetItemText(hwndListView, itemIndex, 1, szProcessName);

                        // Создать буфер для хранения текста
                        wchar_t buffer[256];
                        _itow_s(pmc.WorkingSetSize / 1024, buffer, 10);

                        // Преобразовать PID в строку
                        std::wstring pidString = std::to_wstring(pid);
                        LPWSTR pidText = const_cast<LPWSTR>(pidString.c_str());

                        ListView_SetItemText(hwndListView, itemIndex, 2, pidText); // PID

                        ListView_SetItemText(hwndListView, itemIndex, 3, buffer); // Установить размер в МБ в 3 колонку

                        std::wstring threadCountString = std::to_wstring(threadCount);
                        LPWSTR threadCountText = const_cast<LPWSTR>(threadCountString.c_str());
                        ListView_SetItemText(hwndListView, itemIndex, 4, threadCountText);
                    }
                }
                // Закрыть дескриптор процесса
                CloseHandle(hProcess);
            }
        }
    }
}

void EnumThreadsInProcess(DWORD processId, DWORD& threadCount) {
    HANDLE hThreadSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (hThreadSnapshot == INVALID_HANDLE_VALUE) {
        return;
    }

    THREADENTRY32 threadEntry;
    threadEntry.dwSize = sizeof(THREADENTRY32);

    if (Thread32First(hThreadSnapshot, &threadEntry)) {
        do {
            if (threadEntry.th32OwnerProcessID == processId) {
                threadCount++;
            }
        } while (Thread32Next(hThreadSnapshot, &threadEntry));
    }

    CloseHandle(hThreadSnapshot);
}