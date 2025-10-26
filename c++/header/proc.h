#pragma once

#define UNICODE
#define _UNICODE
#include <windows.h>
#include <vector>

class player {
public:
	uintptr_t staticAddr;
	uintptr_t baseAddr;
	uintptr_t finalAddr;
	std::vector<unsigned int> offsets;
	int newVal;
	player(uintptr_t staticAddr, std::vector<unsigned int> offsets, int newVal) : staticAddr(staticAddr), offsets(offsets), newVal(newVal) {}
};

DWORD GetProcessID(const wchar_t* procName);
uintptr_t GetModuleBaseAddr(DWORD procID, const wchar_t* modbasename);
uintptr_t GetDMAAmmy(HANDLE hProc, uintptr_t basePtr, std::vector<unsigned int> offsets);