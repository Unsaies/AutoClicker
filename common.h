// common.h
#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define UNICODE
#define _UNICODE
#define _WIN32_IE 0x0600
#define _WIN32_WINNT 0x0600

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commctrl.h>
#include <stdlib.h>
#include <time.h>
#include <tchar.h>
#include <stdio.h>

#pragma comment(lib, "comctl32.lib")
#pragma comment(linker, "/SUBSYSTEM:WINDOWS")

// 资源ID定义
#define IDI_MAIN_ICON         101
#define IDD_MAIN_DIALOG       102
#define IDC_CLICK_COUNT       103
#define IDC_FIXED_INTERVAL    104
#define IDC_RANDOM_INTERVAL   105
#define IDC_START_BTN         106
#define IDC_STOP_BTN          107
#define IDC_PAUSE_BTN         108
#define IDC_STATUS            109
#define IDC_HOTKEY_START      110
#define IDC_HOTKEY_STOP       111
#define IDC_HOTKEY_PAUSE      112
#define IDC_HOTKEY_RESUME     113

// 单选按钮ID
#define IDC_RADIO_SINGLE      114
#define IDC_RADIO_DOUBLE      115
#define IDC_RADIO_LEFT        116
#define IDC_RADIO_MIDDLE      117
#define IDC_RADIO_RIGHT       118
#define IDC_RADIO_FIXED       119
#define IDC_RADIO_RANDOM      120

#ifndef _countof
#define _countof(arr) (sizeof(arr)/sizeof(arr[0]))
#endif

typedef enum {
    MOUSE_LEFT,
    MOUSE_MIDDLE,
    MOUSE_RIGHT
} MOUSE_BUTTON;

typedef enum {
    CLICK_SINGLE,
    CLICK_DOUBLE
} CLICK_TYPE;

typedef enum {
    INTERVAL_FIXED,
    INTERVAL_RANDOM
} INTERVAL_TYPE;

// 全局变量声明
extern HINSTANCE hInst;
extern HWND hMainDlg;
extern volatile BOOL g_bRunning;
extern volatile BOOL g_bPaused;
extern DWORD g_dwInterval;
extern DWORD g_dwRandomMax;
extern HANDLE g_hClickerThread;

// 函数声明
void SimulateClick(MOUSE_BUTTON button, CLICK_TYPE clickType);
DWORD WINAPI ClickerThread(LPVOID lpParam);
void HandleHotKey(UINT vkCode);
void RegisterHotKeys(HWND hwnd);
void UnregisterHotKeys(HWND hwnd);
INT_PTR CALLBACK DialogProc(HWND, UINT, WPARAM, LPARAM);
