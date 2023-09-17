#pragma once
#include <variant> 
#include <chrono>
#include <deque> 
#ifndef UNORDERED_SET
#include <unordered_set>
#define UNORDERED_SET true 
#ifndef UNORDED_MAP
#include <unordered_map>
#define UNORDERED_MAP true 
#endif
#endif

#ifdef ERROR_NOW 

struct Deconstruction
{

    //need conditions to make sure that Virtual is ran if a macro error occurs before compile
    // just incase vm is affecting program execution.
    bool Andrew;

     Deconstruction() {
         Andrew = isVirtual(); //dont do if arch isnt included, future compiles arch may not be included?
     };
    ~Deconstruction() {
    
       // Andrew = IsProof();
       //need to change types
    };
};

#define error_acknowledge() [Deconstruct = Deconstruction()] {\
    struct MacroError { int MacroErrorSignature = -4; const char* ErrorMessage = "Macro error found";\
                        const char* MacroName = "ERROR_NOW"; bool IsDefined = true;};\
    return std::variant<MacroError>{MacroError{}};\
}
#endif

#if defined(error_acknowledge) 
#include "Architecture.h" //if macro error occurs then include this on compile time and do checks.
bool Error_acknowledged = true;

auto Call() {return error_acknowledge()();}
using MacroErrorType = decltype(Call()); 
#else
#ifndef ERRORS
#define ERRORS true
#endif
#endif
 
struct error_list {
#ifdef ERROR_NOW
    std::unordered_set<MacroErrorType> Errors{};
#endif
    enum class ErrorType { Unknown, RuntimeError, OtherError };
    std::unordered_map<std::exception, ErrorType> Mapping{};
    ErrorType error_Enum_mapper(const std::string& what) {
        if (what == "") return ErrorType::RuntimeError;
        return ErrorType::Unknown;
    }

#ifdef ERROR_NOW
    decltype(Call()) error;
    error_list() {
        try {
            error = Call();
            std::visit([&](auto& MacroArg) {
                if constexpr (requires { MacroArg.MacroErrorSignature; }) {
                    std::cout << MacroArg.MacroErrorSignature << std::endl;
                    std::cout << MacroArg.ErrorMessage << std::endl;
                }
                else {}
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
        struct ELCheck_ {
#if defined(error_acknowledge)  
            std::vector<MacroErrorType> MacroErrors{};
#endif
            std::chrono::system_clock::time_point timeStamp; 
            int errorCount = 0;  
            enum class Severity { Low, Medium, High };
            std::vector<Severity> severities;
            std::deque<std::string> errorHistory;
            ELCheck_() : timeStamp(std::chrono::system_clock::now()), errorCount(1) {}
#if defined(error_acknowledge) 
            void HandleError(const MacroErrorType& error) {
                timeStamp = std::chrono::system_clock::now();
                std::visit([&](auto& MacroArg) {
                    if constexpr (requires { MacroArg.MacroErrorSignature; }) {
                        errorHistory.push_back(MacroArg.ErrorMessage); 
                    }
                    }, error);
            }
#endif
        };
        typedef  ELCheck_* ELStruct_ptr;
        auto ELCheck = [this]() -> ELCheck_* {
            ELStruct_ptr ELcheckNew = nullptr;
            if (Mapping.empty()) {
                ELcheckNew = new ELCheck_{
                    
                };
                if (ELcheckNew) {
                    /*ELcheckNew->severities., */ //functiion for this,emplacing threads with a priority based on the error. (clean up checks ..)
                    ELcheckNew->errorCount++; //could expand to be for sub errors within a general error , counted as error 1 on construction.

                }
            } 
            return ELcheckNew;
        };

        ELCheck_* EL = ELCheck();
         
        if (EL) {
            delete EL;
        }
    }
};

struct no_error {
    no_error() = default;
};

#ifdef ERROR_NOW
constexpr auto CurrentError = ERROR_NOW;
#error "AN ERROR HAS OCCURRED.."
#if defined(ERRORS)
#undef ERRORS
#endif
#define ERRORS true
#else
#undef ERRORS
#define ERRORS false
#undef NO_ERROR
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
