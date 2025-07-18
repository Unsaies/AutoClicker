// gui.c
#include "common.h"

HWND hMainDlg;

BOOL CALLBACK SetFontCallback(HWND hWnd, LPARAM lParam) {
    SendMessage(hWnd, WM_SETFONT, (WPARAM)lParam, TRUE);
    return TRUE;
}

INT_PTR CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static HFONT hFont = NULL;
    
    switch(uMsg) {
        case WM_INITDIALOG: {
            hMainDlg = hDlg;
            HICON hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_MAIN_ICON));
            SendMessage(hDlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
            
            // 创建字体
            hFont = CreateFont(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, 
                              DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                              DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Microsoft YaHei"));
            
            if (hFont) {
                SendMessage(hDlg, WM_SETFONT, (WPARAM)hFont, TRUE);
                EnumChildWindows(hDlg, (WNDENUMPROC)SetFontCallback, (LPARAM)hFont);
            }
            
            // 初始化单选按钮
            CheckDlgButton(hDlg, IDC_RADIO_SINGLE, BST_CHECKED);
            CheckDlgButton(hDlg, IDC_RADIO_LEFT, BST_CHECKED);
            CheckDlgButton(hDlg, IDC_RADIO_FIXED, BST_CHECKED);
            
            // 固定间隔选项
            static const TCHAR* fixedIntervals[] = {
                _T("0.1秒"), _T("0.2秒"), _T("0.3秒"), _T("0.4秒"), _T("0.5秒"), 
                _T("1秒"), _T("2秒"), _T("3秒"), _T("4秒"), _T("5秒"), _T("10秒")
            };
            for(int i = 0; i < 11; i++) {
                SendDlgItemMessage(hDlg, IDC_FIXED_INTERVAL, CB_ADDSTRING, 0, (LPARAM)fixedIntervals[i]);
            }
            SendDlgItemMessage(hDlg, IDC_FIXED_INTERVAL, CB_SETCURSEL, 5, 0); // 默认1秒
            
            // 随机间隔选项
            static const TCHAR* randomIntervals[] = {
                _T("0.5秒内随机"), _T("1秒内随机"), _T("3秒内随机"), _T("5秒内随机"), _T("10秒内随机")
            };
            for(int i = 0; i < 5; i++) {
                SendDlgItemMessage(hDlg, IDC_RANDOM_INTERVAL, CB_ADDSTRING, 0, (LPARAM)randomIntervals[i]);
            }
            SendDlgItemMessage(hDlg, IDC_RANDOM_INTERVAL, CB_SETCURSEL, 1, 0); // 默认1秒内随机
            
            // 设置默认点击次数
            SetDlgItemInt(hDlg, IDC_CLICK_COUNT, 100, FALSE);
            
            // 注册热键
            RegisterHotKeys(hDlg);
            
            // 初始化状态
            SetDlgItemText(hDlg, IDC_STATUS, _T("状态: 就绪"));
            EnableWindow(GetDlgItem(hDlg, IDC_STOP_BTN), FALSE);
            EnableWindow(GetDlgItem(hDlg, IDC_PAUSE_BTN), FALSE);
            
            // 设置热键显示
            SetDlgItemText(hDlg, IDC_HOTKEY_START, _T("F6"));
            SetDlgItemText(hDlg, IDC_HOTKEY_STOP, _T("F7"));
            SetDlgItemText(hDlg, IDC_HOTKEY_PAUSE, _T("F8"));
            SetDlgItemText(hDlg, IDC_HOTKEY_RESUME, _T("F9"));
            
            // 默认隐藏随机间隔组合框
            ShowWindow(GetDlgItem(hDlg, IDC_RANDOM_INTERVAL), SW_HIDE);
            return TRUE;
        }
        
        case WM_COMMAND: {
            switch(LOWORD(wParam)) {
                case IDC_START_BTN: 
                    HandleHotKey(VK_F6);
                    break;
                    
                case IDC_STOP_BTN: 
                    HandleHotKey(VK_F7);
                    break;
                    
                case IDC_PAUSE_BTN: 
                    if(g_bPaused) {
                        HandleHotKey(VK_F9);
                    } else {
                        HandleHotKey(VK_F8);
                    }
                    break;
                    
                case IDC_RADIO_FIXED:
                case IDC_RADIO_RANDOM:
                    if (HIWORD(wParam) == BN_CLICKED) {
                        BOOL isFixed = IsDlgButtonChecked(hDlg, IDC_RADIO_FIXED) == BST_CHECKED;
                        ShowWindow(GetDlgItem(hDlg, IDC_FIXED_INTERVAL), isFixed ? SW_SHOW : SW_HIDE);
                        ShowWindow(GetDlgItem(hDlg, IDC_RANDOM_INTERVAL), isFixed ? SW_HIDE : SW_SHOW);
                    }
                    break;
                    
                case IDCANCEL:
                    if(g_bRunning) {
                        g_bRunning = FALSE;
                        // 等待线程结束
                        if (g_hClickerThread) {
                            WaitForSingleObject(g_hClickerThread, 1000);
                            CloseHandle(g_hClickerThread);
                            g_hClickerThread = NULL;
                        }
                    }
                    UnregisterHotKeys(hDlg);
                    EndDialog(hDlg, 0);
                    break;
            }
            return TRUE;
        }
        
        case WM_HOTKEY:
            HandleHotKey(HIWORD(lParam));
            return TRUE;
            
        case WM_CLOSE:
            SendMessage(hDlg, WM_COMMAND, IDCANCEL, 0);
            return TRUE;
            
        case WM_DESTROY:
            if (hFont) {
                DeleteObject(hFont);
                hFont = NULL;
            }
            break;
    }
    return FALSE;
}