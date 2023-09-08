#pragma once
#include <iostream>
#include <variant> //std 17

struct error_list {
    error_list() = default;
};

struct no_error {
    no_error() = default;
};

#ifdef ERROR_NOW
constexpr auto CurrentError = ERROR_NOW;
#error "AN ERROR HAS OCCURRED.."
#define ERRORS true
#else
#define ERRORS false
#define NO_ERROR() do { std::cout << "NO ERRORS, GOOD TO PROCEED"; } while(0)
#endif

std::variant<error_list, no_error> ErrorStruct() {
#if ERRORS
    return error_list{};
#else
    return no_error{};
#endif
}
//need to set up conditions for include and manage #include more logically across each header.