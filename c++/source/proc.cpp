#define UNICODE
#define _UNICODE
#include <Windows.h>
#include <TlHelp32.h>
#include "../header/proc.h"

DWORD GetProcessID(const wchar_t* procName) {
    DWORD procID = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32 pe;
        pe.dwSize = sizeof(pe);
        if (Process32First(hSnap, &pe)) {
            do
            {
                if (!_wcsicmp(pe.szExeFile, procName)) {
                    procID = pe.th32ProcessID;
                    break;
                }
            } while (Process32Next(hSnap, &pe));

        }
    }
    CloseHandle(hSnap);
    return procID;
}

uintptr_t GetModuleBaseAddr(DWORD procID, const wchar_t* modName) {
    uintptr_t modBaseAddr = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procID);
    if (hSnap != INVALID_HANDLE_VALUE) {
        MODULEENTRY32 modEntry;
        modEntry.dwSize = sizeof(modEntry);
        if (Module32First(hSnap, &modEntry)) {
            do
            {
                if (!_wcsicmp(modEntry.szModule, modName)) {
                    modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
                    break;
                }
            } while (Module32Next(hSnap, &modEntry));
        }
    }
    CloseHandle(hSnap);
    return modBaseAddr;
}

uintptr_t GetDMAAmmy(HANDLE hProc, uintptr_t staticAddr, std::vector<unsigned int> offsets) {
    uintptr_t baseAddr = staticAddr;
    for (int i = 0; i < offsets.size(); ++i) {
        ReadProcessMemory(hProc, (BYTE*)baseAddr, &baseAddr, sizeof(baseAddr), 0);
        baseAddr += offsets[i];
    }
    return baseAddr;
}