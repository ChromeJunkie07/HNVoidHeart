#include <windows.h>
#include "proc.h"

uintptr_t GetHealthFinal(HANDLE hProc, uintptr_t modBaseAddr) {
    player health(0x01A28450, { 0xA0, 0x30, 0x28, 0x18, 0x60, 0x150, 0x190 }, 10);
    health.baseAddr = health.staticAddr + modBaseAddr;
    health.finalAddr = GetDMAAmmy(hProc, health.baseAddr, health.offsets);
    return health.finalAddr;
}

uintptr_t GetGeoFinal(HANDLE hProc, uintptr_t modBaseAddr) {
    player geo(0x019B8900, { 0x0,0x0,0x80,0x60,0x48,0x218,0x1C4 }, 1000);
    geo.baseAddr = geo.staticAddr + modBaseAddr;
    geo.finalAddr = GetDMAAmmy(hProc, geo.baseAddr, geo.offsets);
    return geo.finalAddr;
}