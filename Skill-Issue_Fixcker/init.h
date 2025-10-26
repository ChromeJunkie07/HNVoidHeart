#pragma once
#include <windows.h>
#include "proc.h"

uintptr_t GetGeoFinal(HANDLE hProc, uintptr_t modBaseAddr);
uintptr_t GetHealthFinal(HANDLE hProc, uintptr_t modBaseAddr);