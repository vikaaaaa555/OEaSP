#pragma once

#include <Windows.h>
#include <tchar.h>
#include <iostream>
#include <vector>
#include <string>

#include <psapi.h>
#pragma comment(lib, "psapi.lib")

#include <CommCtrl.h>
#pragma comment(lib, "Comctl32.lib")

#include <TlHelp32.h>


#define ID_LISTVIEW 1000
#define ID_BUTTON_ENDTASK 1001
#define ID_TIMER_UPDATE 1002