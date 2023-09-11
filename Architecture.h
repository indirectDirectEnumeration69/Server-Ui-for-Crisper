#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <functional>

#ifdef __linux__
#if __has_include(<sys/types.h>)
#include <sys/types.h>

#else
#define LINUX_HEADER_MISSING
#error "sys/types.h file is missing."
#endif
#if __has_include(<sys/stat.h>)
#include <sys/stat.h>

#else
#define LINUX_HEADER_MISSING
#error "sys/stat.h file is missing."
#endif
#if __has_include(<fcntl.h>)
#include <fcntl.h>

#else
#define LINUX_HEADER_MISSING
#error "fcntl.h file is missing."
#endif
#if __has_include(<sys/ptrace.h>)
#include <sys/ptrace.h>

#else
#define LINUX_HEADER_MISSING
#error "sys/ptrace.h file is missing."
#endif
#if __has_include(<unistd.h>)
#include <unistd.h>

#else
#define LINUX_HEADER_MISSING
#error "unistd.h file is missing."
#endif
#endif


#ifdef _WIN64
constexpr bool isWindows = true;
constexpr auto OS = "Windows_64";
#include <windows.h>

typedef LONG(WINAPI* RegOpenKeyExA_t)(
    HKEY    hKey,
    LPCSTR  lpSubKey,
    DWORD   ulOptions,
    REGSAM  samDesired,
    PHKEY   phkResult);

typedef LONG(WINAPI* RegQueryValueExA_t)(
    HKEY    hKey,
    LPCSTR  lpValueName,
    LPDWORD lpReserved,
    LPDWORD lpType,
    LPBYTE  lpData, 
    LPDWORD lpcbData); 

typedef LONG(WINAPI* RegCloseKey_t)(
    HKEY hKey);

#if !defined(PEB) || !defined(_PEB)
typedef struct _PEB {
    BYTE Reserved1[2];
    BYTE BeingDebugged; //on debug byte value will be a non zero value.
    BYTE Reserved2[21];
    PVOID LoaderData;
} PEB, * PPEB;
#endif

#elif defined(_WIN32)
constexpr bool isWindows = true;
constexpr auto OS = "Windows_32";
#include <windows.h> 
#if !defined(PEB) || !defined(_PEB)
typedef struct _PEB {
    BYTE Reserved1[2];
    BYTE BeingDebugged;
    BYTE Reserved2[9];
    PVOID LoaderData;
} PEB, * PPEB;
#endif

#elif defined(__linux__)
constexpr bool isWindows = false;
constexpr auto OS = "Linux";

#elif defined(__APPLE__)
constexpr bool isWindows = false;
constexpr auto OS = "Mac";

#elif defined(__unix__)
constexpr bool isWindows = false;
constexpr auto OS = "Unix";

#elif defined(__posix__)
constexpr bool isWindows = false;
constexpr auto OS = "Posix";

#else
constexpr bool isWindows = false;
constexpr auto OS = "Unknown";
#define UNKNOWN
#endif

#ifndef UNKNOWN
#if defined(_DEBUG)
constexpr bool isDebug = true;
#elif !defined(NDEBUG)
constexpr bool isDebug = true;
#else
constexpr bool isDebug = false;
#endif

#if defined(__i386__) || defined(_M_IX86) || defined(__x86_64__) || defined(_M_X64)
#include <intrin.h>
#include <fcntl.h>
#endif

inline bool checkDebuggerLinux() {
#ifdef __linux__
    if (ptrace(PTRACE_TRACEME, 0, 1, 0) == -1) {
        return true; 
    }
#endif
    return false;
}

inline bool checkTamperingLinux() {
#ifdef __linux__
    int fd = open("/proc/self/maps", O_RDONLY);
    if (fd == -1) {
        return true;
    }
    close(fd);
#endif
    return false;
}

#include <random>
inline bool checkCPUID() {
#if defined(_WIN64) || defined(_WIN32)
    if (IsDebuggerPresent()) {
        exit(EXIT_FAILURE);
    }
#endif

#if defined(__i386__) || defined(_M_IX86) || defined(__x86_64__) || defined(_M_X64)
    int CPUInfo[4] = { -1 };
    __cpuid(CPUInfo, 1);
    return (CPUInfo[2] & (1 << 5)) != 0;
#else
    return false;
#endif
}

inline std::string Xo(const std::string& input, char key) { 
    std::string output = input;
    for (std::size_t i = 0; i < input.size(); ++i) {
        output[i] = input[i] ^ key;
    }

    return output;
}

inline bool checkDrivers() {
#if defined(_WIN64) || defined(_WIN32)
    if (IsDebuggerPresent()) {
        exit(EXIT_FAILURE);
    }
#endif

    std::ifstream f("/sys/class/dmi/id/product_name"); 
    if (f.is_open()) {
        std::string L;
        std::getline(f, L);

        std::string B = Xo("\x4D\x57\x4B\x42\x58\x4D\x47\x4A", '\x11');
        std::string M = Xo("\x55\x5E\x57\x41\x52\x45", '\x11');

        return (L.find(B) != std::string::npos || L.find(M) != std::string::npos);  
    }
    return false;
}

inline bool checkTiming() {
#if defined(_WIN64) || defined(_WIN32)
    if (IsDebuggerPresent()) {
        exit(EXIT_FAILURE); 
    }
#endif


#if defined(__i386__) || defined(_M_IX86) || defined(__x86_64__) || defined(_M_X64)
    auto start = __rdtsc();
    for (volatile int i = 0; i < 10000; ++i) {}
    auto end = __rdtsc();
    return (end - start) < 1000;
#else
    return false;
#endif
}

inline bool SecondaryDebuggerCheck() { //checking with secondary debugger incase debugger is compromised.
#if defined(_WIN64)
    PEB* peb = (PEB*)__readgsqword(0x60);
#elif defined(_WIN32)
    PEB* peb = (PEB*)__readfsdword(0x30);
#endif
    if (peb && peb->BeingDebugged) {
        return true;
    }
    if (checkTiming()) {
        return true;
    }
    return false;
}
std::string obfuscate(const std::string& input, char key) {
    std::string output = input;
    for (std::size_t i = 0; i < input.size(); ++i) {
        char tmp = input[i];
        tmp = (tmp >> 2) | (tmp << (8 - 2));
        tmp -= 3;
        tmp ^= key;
        output[i] = tmp;
    }
    return output;
}

std::string deobfuscate(const std::string& input, char key) {
    std::string output = input;
    for (std::size_t i = 0; i < input.size(); ++i) {
        char tmp = input[i];
        tmp ^= key;
        tmp += 3;
        tmp = (tmp << 2) | (tmp >> (8 - 2));
        output[i] = tmp;
    }
    return output;
}

DWORD CalculateChecksum(const BYTE* data, size_t length) {
    DWORD sum = 0;
    for (size_t i = 0; i < length; ++i) {
        sum += data[i];
    }
    return sum;
}

#if defined(_WIN64) || defined(_WIN32)
bool CRS() {
    std::string obfuscatedPath = obfuscate("HKEY_LOCAL_MACHINE\\HARDWARE\\DESCRIPTION\\System\\BIOS", 0x11);
    std::string obfuscatedValueName = obfuscate("SystemProductName", 0x11);
    std::string path = deobfuscate(obfuscatedPath, 0x11);
    std::string valueName = deobfuscate(obfuscatedValueName, 0x11);

    HMODULE hAdvapi32 = LoadLibrary(TEXT("advapi32.dll"));
    if (hAdvapi32 == NULL) {
        return false;
    }

    auto pRegOpenKeyExA = (RegOpenKeyExA_t)GetProcAddress(hAdvapi32, "RegOpenKeyExA");
    auto pRegQueryValueExA = (RegQueryValueExA_t)GetProcAddress(hAdvapi32, "RegQueryValueExA");
    auto pRegCloseKey = (RegCloseKey_t)GetProcAddress(hAdvapi32, "RegCloseKey");

    if (!pRegOpenKeyExA || !pRegQueryValueExA || !pRegCloseKey) {
        FreeLibrary(hAdvapi32);
        return false;
    }

    __try {
        HKEY hKey;
        if (pRegOpenKeyExA(HKEY_LOCAL_MACHINE, path.c_str(), 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
            DWORD type = REG_SZ;
            DWORD dataSize = 512;
            BYTE data[512];

            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> distr(100, 500);
            Sleep(distr(gen));

            if (pRegQueryValueExA(hKey, valueName.c_str(), 0, &type, data, &dataSize) == ERROR_SUCCESS) {
                DWORD checksum = CalculateChecksum(data, dataSize);
                if (checksum == 56789) {
                    pRegCloseKey(hKey);
                    FreeLibrary(hAdvapi32);
                    return true;
                }
            }
            pRegCloseKey(hKey);
        }
    }
    __except (EXCEPTION_EXECUTE_HANDLER) {
        FreeLibrary(hAdvapi32);
        return false;
    }

    FreeLibrary(hAdvapi32);
    return false;
} 
#endif

inline bool isVirtual() {
    std::vector<std::function<bool()>> checks = { checkCPUID, checkDrivers, checkTiming, SecondaryDebuggerCheck,CRS};
#if defined(__linux__)
    checks.push_back(checkDebuggerLinux);
    checks.push_back(checkTamperingLinux);
#endif
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(checks.begin(), checks.end(), g);

    for (auto& check : checks) {
        if (check()) {
            return true;
        }
    }
    return false;
}
#endif




//                                                         ||||||
// used to determine macro inclusions for operating system vvvvvv
//https://stackoverflow.com/questions/5919996/how-to-detect-reliably-mac-os-x-ios-linux-windows-in-c-preprocessor