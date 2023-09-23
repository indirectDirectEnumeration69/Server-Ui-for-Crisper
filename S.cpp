#include "ServerM.h"
#pragma comment(lib, "Ws2_32.lib")
#include <iostream>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <random>
#include <string> 
#include <climits>
 


//fixing redefinitions now.
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#define WIN_INCLUDE true
#endif

volatile struct J {
    J() {
        std::random_device d;
        std::mt19937 n(d());
        std::uniform_int_distribution<> distr(0, UINT32_MAX);
        uint32_t rum = distr(n);
        uint32_t salt = rum ^ (static_cast<unsigned int>(std::hash<std::string>{}(std::to_string((double)rum))) ^ static_cast<unsigned int>(std::time(0)));
        std::srand(salt);
    }
    virtual ~J() {
    }
    virtual bool performCheck() {
        return true;
    }
};

volatile struct QK : J {
    std::random_device r;
    std::mt19937_64 g;
    std::uniform_int_distribution<> dis;

    QK() : J(), dis(0, UINT32_MAX){
        uint32_t voxol = dis(r);
        g = std::mt19937_64(r() ^ voxol);  
        int r = std::uniform_int_distribution<>(0, 7)(g); 
        switch (r) {
        case 0:
            performCheck();
            checkDebuggerAPI();
            checkTiming();
            checkSystemCalls();
            break;
        case 1:
            checkTiming();
            checkDebuggerAPI();
            performCheck();
            break;
        case 2:
            checkDebuggerAPI();
            performCheck();
            break;
        case 3:
            checkSystemInfo();
            break;
        case 4:
            checkSystemCalls();
            checkMemoryProperties();
            break;
        case 5:
            checkSystemCalls();
            checkDebuggerAPI();
            break;
        case 6:
            checkSystemInfo();
            checkMemoryProperties();
            break;
        default:
            checkDebuggerAPI();
            checkTiming();
            checkSystemInfo();
            checkMemoryProperties();
            break;
        }
    }

    inline bool checkTiming() {
        auto start = std::chrono::high_resolution_clock::now();
        int sum = 0;
        for (int i = 0; i < 1000; ++i) {
            sum += i;
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> diff = end - start;
        if (diff.count() > 1.0) {
            std::exit(1);
        }
        return true;
    }

    inline bool checkDebuggerAPI() {
        if (IsDebuggerPresent()) {
            std::exit(1);
        }
        return true;
    }

    inline bool checkSystemInfo() {
        SYSTEM_INFO sysInfo;
        GetSystemInfo(&sysInfo);

        if (sysInfo.dwNumberOfProcessors < 2) {
            std::exit(1);
        }
        return true;
    }

    inline bool checkSystemCalls() {
        HANDLE hFile = CreateFileA("Log.txt", GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

        if (GetLastError() == ERROR_FILE_NOT_FOUND) {
            return true;
        }
        else {
            std::exit(1);
        }
    }
    inline bool checkMemoryProperties() {
     MEMORYSTATUSEX statex;
     statex.dwLength = sizeof(statex);
     if (GlobalMemoryStatusEx(&statex)) {
         if (statex.ullAvailPhys < (1ULL << 30)) { std::exit(1); }} //Available physical memory check.
     else {std::exit(1);}return true;}
        virtual ~QK() {}}; QK globalInstance;


int main(){Server();}

/*

list of includes going to be used : 

#include <thread>
#include <fstream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <fstream>
#include <windows.h>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <vector>
#include <cstdint>
#include <bitset>
*/

