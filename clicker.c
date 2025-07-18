// clicker.c
#include "common.h"

volatile BOOL g_bRunning = FALSE;
volatile BOOL g_bPaused = FALSE;
DWORD g_dwInterval = 100;
DWORD g_dwRandomMax = 0;
HANDLE g_hClickerThread = NULL;

// 模拟鼠标点击
void SimulateClick(MOUSE_BUTTON button, CLICK_TYPE clickType) {
    DWORD downEvent = 0, upEvent = 0;
    
    switch(button) {
        case MOUSE_LEFT:
            downEvent = MOUSEEVENTF_LEFTDOWN;
            upEvent = MOUSEEVENTF_LEFTUP;
            break;
        case MOUSE_MIDDLE:
            downEvent = MOUSEEVENTF_MIDDLEDOWN;
            upEvent = MOUSEEVENTF_MIDDLEUP;
            break;
        case MOUSE_RIGHT:
            downEvent = MOUSEEVENTF_RIGHTDOWN;
            upEvent = MOUSEEVENTF_RIGHTUP;
            break;
    }
    
    INPUT inputs[4] = {0};
    int inputCount = (clickType == CLICK_DOUBLE) ? 4 : 2;
    
    for(int i = 0; i < inputCount; i += 2) {
        inputs[i].type = INPUT_MOUSE;
        inputs[i].mi.dwFlags = downEvent;
        
        inputs[i+1].type = INPUT_MOUSE;
        inputs[i+1].mi.dwFlags = upEvent;
    }
    
    SendInput(inputCount, inputs, sizeof(INPUT));
}

// 连点器线程
DWORD WINAPI ClickerThread(LPVOID lpParam) {
    // 获取鼠标按键选择
    MOUSE_BUTTON button = MOUSE_LEFT;
    if (IsDlgButtonChecked(hMainDlg, IDC_RADIO_MIDDLE) == BST_CHECKED) {
        button = MOUSE_MIDDLE;
    } else if (IsDlgButtonChecked(hMainDlg, IDC_RADIO_RIGHT) == BST_CHECKED) {
        button = MOUSE_RIGHT;
    }
    
    // 获取点击类型
    CLICK_TYPE clickType = CLICK_SINGLE;
    if (IsDlgButtonChecked(hMainDlg, IDC_RADIO_DOUBLE) == BST_CHECKED) {
        clickType = CLICK_DOUBLE;
    }
    
    // 获取间隔类型
    INTERVAL_TYPE intervalType = INTERVAL_FIXED;
    if (IsDlgButtonChecked(hMainDlg, IDC_RADIO_RANDOM) == BST_CHECKED) {
        intervalType = INTERVAL_RANDOM;
    }
    
    // 获取点击次数
    DWORD g_dwClickCount = 0;
    BOOL bSuccess = FALSE;
    g_dwClickCount = GetDlgItemInt(hMainDlg, IDC_CLICK_COUNT, &bSuccess, FALSE);
    
    if (!bSuccess || g_dwClickCount == 0) {
        MessageBox(hMainDlg, _T("请输入有效的点击次数！"), _T("错误"), MB_ICONERROR);
        g_bRunning = FALSE;
        SetDlgItemText(hMainDlg, IDC_STATUS, _T("状态: 输入无效"));
        EnableWindow(GetDlgItem(hMainDlg, IDC_START_BTN), TRUE);
        EnableWindow(GetDlgItem(hMainDlg, IDC_STOP_BTN), FALSE);
        EnableWindow(GetDlgItem(hMainDlg, IDC_PAUSE_BTN), FALSE);
        return 0;
    }
    
    DWORD g_dwCurrentCount = 0;
    
    // 获取间隔设置
    int fixedSel = SendDlgItemMessage(hMainDlg, IDC_FIXED_INTERVAL, CB_GETCURSEL, 0, 0);
    int randomSel = SendDlgItemMessage(hMainDlg, IDC_RANDOM_INTERVAL, CB_GETCURSEL, 0, 0);
    
    DWORD fixedIntervals[] = {100, 200, 300, 400, 500, 1000, 2000, 3000, 4000, 5000, 10000};
    DWORD randomMax[] = {500, 1000, 3000, 5000, 10000};
    
    if (intervalType == INTERVAL_FIXED && fixedSel >= 0 && fixedSel < 11) {
        g_dwInterval = fixedIntervals[fixedSel];
    } else if (intervalType == INTERVAL_RANDOM && randomSel >= 0 && randomSel < 5) {
        g_dwRandomMax = randomMax[randomSel];
    }
    
    while(g_bRunning && g_dwCurrentCount < g_dwClickCount) {
        if(g_bPaused) {
            Sleep(100);
            continue;
        }
        
        SimulateClick(button, clickType);
        g_dwCurrentCount++;
        
        TCHAR status[128];
        _sntprintf(status, _countof(status), _T("状态: 运行中 (%lu/%lu)"), g_dwCurrentCount, g_dwClickCount);
        SetDlgItemText(hMainDlg, IDC_STATUS, status);
        
        if(g_dwCurrentCount >= g_dwClickCount) {
            g_bRunning = FALSE;
            break;
        }
        
        DWORD sleepTime = g_dwInterval;
        if(intervalType == INTERVAL_RANDOM && g_dwRandomMax > 0) {
            sleepTime = rand() % g_dwRandomMax;
        }
        
        Sleep(sleepTime);
    }
    
    SetDlgItemText(hMainDlg, IDC_STATUS, _T("状态: 已完成"));
    EnableWindow(GetDlgItem(hMainDlg, IDC_START_BTN), TRUE);
    EnableWindow(GetDlgItem(hMainDlg, IDC_STOP_BTN), FALSE);
    EnableWindow(GetDlgItem(hMainDlg, IDC_PAUSE_BTN), FALSE);
    
    g_bPaused = FALSE;
    g_hClickerThread = NULL;
    return 0;
}
