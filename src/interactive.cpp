//
// Created by Roman on 30.07.2023.
//
#include <interactive.h>

void interactive_mode(int key) {
    exit(0);
}

void daemon_mode(int key) {
    char szPath[0x100];
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory( &si, sizeof(si) );
    si.cb = sizeof(si);
    ZeroMemory( &pi, sizeof(pi) );
    GetModuleFileName(nullptr, szPath, sizeof(szPath));
    strcat(szPath, " -k ");
    strcat(szPath, std::string(std::to_string(key)).c_str());
    if (!CreateProcess(
            NULL,
            szPath,
            NULL,
            NULL,
            FALSE,
            CREATE_NO_WINDOW,
            NULL,
            NULL,
            &si,
            &pi)
    )
    {
        PLOGF << "CreateProcess failed: " << GetLastError();
        exit(1);
    }
    else {
        PLOGI << "Daemon process created";
        exit(0);
    }
}
