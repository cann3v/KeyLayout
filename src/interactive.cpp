//
// Created by Roman on 30.07.2023.
//
#include <interactive.h>

void interactive_mode(int key) {
    std::cout << "[1] Add to startup" << std::endl;
    std::cout << "[2] Delete from startup" << std::endl;

    int choice;
    std::cin >> choice;
    switch (choice) {
        case 1:
            autorunOn(key);
            break;
        case 2:
            autorunOff();
            break;
        default:
            PLOGF << "Unknown option";
            exit(1);
    }
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
    }
}

void autorunOn(int key) {
    char szPath[0x100];
    GetModuleFileName(nullptr, szPath, sizeof(szPath));
    strcat(szPath, " -k ");
    strcat(szPath, std::string(std::to_string(key)).c_str());
    strcat(szPath, " -d");
    PLOGV << "Command to execute: " << szPath;

    LONG lresult = 0;
    char value[0x100];
    DWORD buffersize = sizeof(value);

    lresult = RegGetValueA(
            HKEY_LOCAL_MACHINE,
            R"(Software\Microsoft\Windows\CurrentVersion\Run)",
            "KeyLayout",
            RRF_RT_REG_SZ,
            nullptr,
            (PVOID)&value,
            &buffersize
            );

    if (lresult == ERROR_FILE_NOT_FOUND) {
        PLOGV << "Registry value not found.";

        HKEY hKey;
        lresult = RegCreateKeyExA(
                HKEY_LOCAL_MACHINE,
                R"(Software\Microsoft\Windows\CurrentVersion\Run)",
                0,
                "",
                REG_OPTION_NON_VOLATILE,
                KEY_SET_VALUE,
                nullptr,
                &hKey,
                nullptr
                );

        if (hKey && (lresult == ERROR_SUCCESS)) {
            PLOGV << "Key created";

            lresult = RegSetValueEx(
                    hKey,
                    "KeyLayout",
                    0,
                    REG_SZ,
                    LPBYTE(szPath),
                    strlen(szPath)
                    );
            RegCloseKey(hKey);

            if (lresult == ERROR_SUCCESS) {
                std::cout << "Autorun enabled" << std::endl;
            }
            else {
                PLOGF << "lresult = " << lresult << " (see https://learn.microsoft.com/en-us/windows/win32/debug/system-error-codes)";
                std::cout << "Failure while setting value." << std::endl;
                exit(1);
            }
        }
        else {
            PLOGF << "lresult = " << lresult << " (see https://learn.microsoft.com/en-us/windows/win32/debug/system-error-codes)";
            std::cout << "Failure while creating key." << std::endl;
            exit(1);
        }
    }
    else {
        PLOGV << "lresult =" << lresult << " (see https://learn.microsoft.com/en-us/windows/win32/debug/system-error-codes)";
        std::cout << "Autorun already enabled." << std::endl;
    }
}

void autorunOff() {
    LONG lresult = 0;
    char value[0x100];
    DWORD buffersize = sizeof(value);

    lresult = RegGetValueA(
            HKEY_LOCAL_MACHINE,
            R"(Software\Microsoft\Windows\CurrentVersion\Run)",
            "KeyLayout",
            RRF_RT_REG_SZ,
            nullptr,
            (PVOID)&value,
            &buffersize
    );

    if (lresult == ERROR_FILE_NOT_FOUND) {
        PLOGF << "lresult = " << lresult << " (see https://learn.microsoft.com/en-us/windows/win32/debug/system-error-codes)";
        std::cout << "Registry value not found. Autorun already disabled." << std::endl;
        exit(1);
    }
    else if (lresult == ERROR_SUCCESS) {
        PLOGV << "Registry value found";

        lresult = RegDeleteKeyValueA(
                HKEY_LOCAL_MACHINE,
                R"(Software\Microsoft\Windows\CurrentVersion\Run)",
                "KeyLayout"
                );

        if (lresult == ERROR_SUCCESS) {
            std::cout << "Registry value deleted." << std::endl;
        }
        else {
            PLOGF << "lresult = " << lresult << " (see https://learn.microsoft.com/en-us/windows/win32/debug/system-error-codes)";
            std::cout << "Error while deleting key value." << std::endl;
            exit(1);
        }
    }
    else {
        PLOGF << "lresult = " << lresult << " (see https://learn.microsoft.com/en-us/windows/win32/debug/system-error-codes)";
        std::cout << "Unhandled result, sorry." << std::endl;
        exit(1);
    }
}
