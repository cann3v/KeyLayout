//
// Created by Roman on 30.07.2023.
//

#ifndef KEYLAYOUT_HOOKS_H
#define KEYLAYOUT_HOOKS_H

#include <windows.h>

#include <plog/Log.h>

void SetHook(int key);
LRESULT CALLBACK HandleKey(int iCode, WPARAM wParam, LPARAM lParam);
void PressKeys();

#endif //KEYLAYOUT_HOOKS_H
