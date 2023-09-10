#pragma once
#include <variant> 

#ifndef UNORDERED_SET
#include <unordered_set>
#define UNORDERED_SET true 
#ifndef UNORDED_MAP)
#include <unordered_map>
#define UNORDERED_MAP true 
#endif
#endif

#ifndef ERROR_NOW

struct Deconstruction
{   
     Deconstruction() {};
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
using MacroErrorType = decltype(Call()); 
#else
#ifndef ERRORS
#define ERRORS true
#endif
#endif

struct error_list {
    std::unordered_set<MacroErrorType> Errors{};
    enum class ErrorType { Unknown, RuntimeError, OtherError };
    std::unordered_map<std::exception, ErrorType> Mapping{};
    ErrorType error_Enum_mapper(const std::string& what) {
        if (what == "") return ErrorType::RuntimeError;
        return ErrorType::Unknown;
    }

#ifndef ERROR_NOW
    decltype(Call()) error;
    error_list() {
        try {
            error = Call();
            std::visit([&](auto& MacroArg) {
                if constexpr (requires { MacroArg.MacroErrorSignature; }) {
                    std::cout << MacroArg.MacroErrorSignature << std::endl;
                    std::cout << MacroArg.ErrorMessage << std::endl;
                }else { }
                }, error);
        } //std 20 dependent.
        catch (const std::runtime_error& RE) {
            ErrorType err = error_Enum_mapper(RE.what());
            
            switch (err) {
            case ErrorType::RuntimeError:
                std::cout << "Caught a specific runtime error\n";
                break;

            case ErrorType::OtherError:
                std::cout << "Caught some other error\n";
                break;

            default: break;
            }
        }
        catch (...) {
        }
    };
#else
    error_list() = default;
#endif
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
