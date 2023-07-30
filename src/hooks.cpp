//
// Created by Roman on 30.07.2023.
//

#include "hooks.h"

namespace hooks {
    int key;
}

void SetHook(int key) {
    hooks::key = key;
    HHOOK hhk = SetWindowsHookEx(WH_KEYBOARD_LL, &HandleKey, 0, 0);
    PLOGW << "Hook is set up!";
    MSG msg;
    BOOL bRet;
    while ((bRet = GetMessage(&msg, nullptr, 0, 0)) != 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

LRESULT CALLBACK HandleKey(int iCode, WPARAM wParam, LPARAM lParam) {
    auto p = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

    if (wParam == WM_SYSKEYDOWN) {
        PLOGV << "Type of hook: WM_SYSKEYDOWN";
        if (p->vkCode == hooks::key) {
            PLOGV << "Pressed key (" << p->vkCode << ") == target (" << hooks::key << ")";
            PressKeys();
            return 1;
        } else PLOGV << "Pressed key != target (" << p->vkCode << " != " << hooks::key << ")";
    }
    return CallNextHookEx(nullptr, iCode, wParam, lParam);
}

void PressKeys() {
    PLOGV << "Sending input...";

    INPUT inputs[4] = {};
    ZeroMemory(inputs, sizeof(inputs));

    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = VK_LWIN;

    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.wVk = VK_SPACE;

    inputs[2].type = INPUT_KEYBOARD;
    inputs[2].ki.wVk = VK_SPACE;
    inputs[2].ki.dwFlags = KEYEVENTF_KEYUP;

    inputs[3].type = INPUT_KEYBOARD;
    inputs[3].ki.wVk = VK_LWIN;
    inputs[3].ki.dwFlags = KEYEVENTF_KEYUP;

    UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
    if (uSent != ARRAYSIZE(inputs))
    {
        PLOGE << L"SendInput failed: 0x" << HRESULT_FROM_WIN32(GetLastError());
    }
    else PLOGV << "Input sent";
}
