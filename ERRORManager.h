#pragma once
#include <variant> 

#ifndef ERROR_NOW 
struct Deconstruction
{
    ~Deconstruction() {};
};
#define error_acknowledge() [Deconstruct = Deconstruction()] {\
    struct MacroError { int MacroErrorSignature = -4; const char* ErrorMessage = "Macro error found";\
                        const char* MacroName = "ERROR_NOW"; bool IsDefined = true;};\
    return std::variant<MacroError>{MacroError{}};\
}
#endif

#if defined(error_acknowledge)
bool Error_acknowledged = true;

auto Call() {return error_acknowledge()();}
#else
#ifndef ERRORS
#define ERRORS true
#endif
#endif

struct error_list {
   
        
    error_list(){Call();};
    
    ~error_list() {
        
    }
   
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
#if defined(_WIN32) || defined(_WIN64)

#endif
    return error_list{};
#else
    return no_error{};
#endif
}
//need to set up conditions for include and manage #include more logically across each header.
