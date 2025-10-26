#define UNICODE
#define _UNICODE
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <string>
#include <cstring>
#include <iostream>
#include "../header/proc.h"
#include "../header/init.h"
#include <limits>
#include <sstream>

#pragma comment(lib, "Ws2_32.lib")

int main() {
    DWORD procID = GetProcessID(L"hollow_knight.exe");
    uintptr_t modBaseAddr = GetModuleBaseAddr(procID, L"UnityPlayer.dll");
    HANDLE hProc = 0;
    hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, procID);
    uintptr_t healthFinal = GetHealthFinal(hProc, modBaseAddr);
    uintptr_t geoFinal = GetGeoFinal(hProc, modBaseAddr);
    WSADATA w;
    if (WSAStartup(MAKEWORD(2, 2), &w) != 0) return 1;

    SOCKET listenSock = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSock == INVALID_SOCKET) { std::cerr << "socket failed: " << WSAGetLastError() << "\n"; WSACleanup(); return 1; }

    sockaddr_in srv{};
    srv.sin_family = AF_INET;
    srv.sin_port = htons(6769);
    srv.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

    if (::bind(listenSock, reinterpret_cast<sockaddr*>(&srv), static_cast<int>(sizeof(srv))) == SOCKET_ERROR) {
        std::cerr << "bind failed: " << WSAGetLastError() << "\n"; closesocket(listenSock); WSACleanup(); return 1;
    }

    if (::listen(listenSock, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "listen failed: " << WSAGetLastError() << "\n"; closesocket(listenSock); WSACleanup(); return 1;
    }

    sockaddr_in clientAddr{};
    int clientAddrLen = static_cast<int>(sizeof(clientAddr));
    SOCKET client = ::accept(listenSock, reinterpret_cast<sockaddr*>(&clientAddr), &clientAddrLen);

    if (client == INVALID_SOCKET) {
        std::cerr << "accept failed: " << WSAGetLastError() << "\n";
        closesocket(listenSock); WSACleanup(); return 1;
    }

    char buffer[500];  // Empfangspuffer
    int bytesReceived;

    while (true) {
        bytesReceived = recv(client, buffer, sizeof(buffer) - 1, 0);

        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
            std::cout << "Received: " << buffer << "\n";
            int cmd = 0;
            int newVal = 0;
            //int newVal = std::stoi(buffer);
            std::stringstream ss(buffer);
            ss >> cmd >> newVal;

            switch (cmd) {
            case 1: { // Health
                if (!WriteProcessMemory(hProc, (BYTE*)healthFinal, &newVal, sizeof(newVal), nullptr)) {
                    std::cerr << "WriteProcessMemory (health) failed: " << GetLastError() << "\n";
                }
                else {
                    std::cout << "Health set to " << newVal << "\n";
                }
                break;
            }
            case 2: { // Geo
                if (!WriteProcessMemory(hProc, (BYTE*)geoFinal, &newVal, sizeof(newVal), nullptr)) {
                    std::cerr << "WriteProcessMemory (geo) failed: " << GetLastError() << "\n";
                }
                else {
                    std::cout << "Geo set to " << newVal << "\n";
                }
                break;
            }
            default:
                std::cerr << "Unknown command: " << cmd << "\n";
                break;
            }
        }
        else if (bytesReceived == 0) {
            std::cout << "Client disconnected.\n";
            break;
        }
        else {
            std::cerr << "recv failed: " << WSAGetLastError() << "\n";
            break;
        }
    }
    CloseHandle(hProc);
    return 0;
}