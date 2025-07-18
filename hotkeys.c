// hotkeys.c
#include "common.h"
extern HANDLE g_hClickerThread;

void HandleHotKey(UINT vkCode) {
    if(!IsWindowVisible(hMainDlg)) {
        ShowWindow(hMainDlg, SW_SHOW);
        SetForegroundWindow(hMainDlg);
        return;
    }
    
    if(vkCode == VK_F6 && !g_bRunning) {
        g_bRunning = TRUE;
        g_hClickerThread = CreateThread(NULL, 0, ClickerThread, NULL, 0, NULL);
        SetDlgItemText(hMainDlg, IDC_STATUS, _T("状态: 启动中..."));
        EnableWindow(GetDlgItem(hMainDlg, IDC_START_BTN), FALSE);
        EnableWindow(GetDlgItem(hMainDlg, IDC_STOP_BTN), TRUE);
        EnableWindow(GetDlgItem(hMainDlg, IDC_PAUSE_BTN), TRUE);
    }
    else if(vkCode == VK_F7 && g_bRunning) {
        g_bRunning = FALSE;
        SetDlgItemText(hMainDlg, IDC_STATUS, _T("状态: 正在停止..."));
    }
    else if(vkCode == VK_F8 && g_bRunning && !g_bPaused) {
        g_bPaused = TRUE;
        SetDlgItemText(hMainDlg, IDC_STATUS, _T("状态: 已暂停"));
        SetDlgItemText(hMainDlg, IDC_PAUSE_BTN, _T("继续(&R)"));
    }
    else if(vkCode == VK_F9 && g_bRunning && g_bPaused) {
        g_bPaused = FALSE;
        SetDlgItemText(hMainDlg, IDC_STATUS, _T("状态: 运行中"));
        SetDlgItemText(hMainDlg, IDC_PAUSE_BTN, _T("暂停(&P)"));
    }
}

void RegisterHotKeys(HWND hwnd) {
    RegisterHotKey(hwnd, 1, 0, VK_F6);
    RegisterHotKey(hwnd, 2, 0, VK_F7);
    RegisterHotKey(hwnd, 3, 0, VK_F8);
    RegisterHotKey(hwnd, 4, 0, VK_F9);
}

void UnregisterHotKeys(HWND hwnd) {
    UnregisterHotKey(hwnd, 1);
    UnregisterHotKey(hwnd, 2);
    UnregisterHotKey(hwnd, 3);
    UnregisterHotKey(hwnd, 4);
}
