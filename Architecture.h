#pragma once

#ifdef _WIN64
constexpr bool isWindows = true;
constexpr auto OS = "Windows_64";

#elif defined(_WIN32)
constexpr bool isWindows = true;
constexpr auto OS = "Windows_32";

#elif defined(__linux__)
constexpr bool isWindows = false;
constexpr auto OS = "Linux";

#elif defined(__APPLE__)
constexpr bool isWindows = false;
constexpr auto OS = "Mac";

#elif defined(__unix__)
constexpr bool isWindows = false;
constexpr auto OS = "Unix";

#elif defined(__posix)
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
#endif


//https://stackoverflow.com/questions/5919996/how-to-detect-reliably-mac-os-x-ios-linux-windows-in-c-preprocessor