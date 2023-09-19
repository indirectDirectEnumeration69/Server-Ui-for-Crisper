#pragma once
#include <thread>
#include <windows.h> //will only include if in a windows enviroment, cant souly rely on architecure.h for enviroment checking incase its tampered with.
#include <iostream> //one thread at all times will make sure the enviroment is not changed during runtime.
#include <sddl.h>
#include <fstream> //not going to use this later too obvious.
#include <openssl/rand.h>
//winapi create thread allows for more specific control of thread access rights than std::thread.
//macro for further enviroment checking.
//servers thread control i may keep it as it is or i will integrate it into the ThreadControlSystem(TCS).

class ThreadControlSystem {
public:
    static DWORD WINAPI ISX86(LPVOID lpParam)//static or causes errors as the thread has to have a static type and within winapi call __stdcall is not static.
    {
        SYSTEM_INFO si;
        GetSystemInfo(&si);
        std::string architecture;

        switch (si.wProcessorArchitecture) {
        case PROCESSOR_ARCHITECTURE_INTEL:
            architecture = "x86";
            break;
        case PROCESSOR_ARCHITECTURE_IA64:
            architecture = "Itanium";
            break;
        case PROCESSOR_ARCHITECTURE_AMD64:
            architecture = "x64";
            break;
        case PROCESSOR_ARCHITECTURE_ARM:
            architecture = "ARM";
            break;
        case PROCESSOR_ARCHITECTURE_ARM64:
            architecture = "ARM64";
            break;
        default:
            architecture = "Unknown";
            break;
        }

        std::cout << "Processor Architecture: " << architecture << std::endl;

        unsigned char buffer[256];
        if (RAND_bytes(buffer, sizeof(buffer)) != 1) { 
            std::cerr << "Error generating random bytes." << std::endl;
            return 1;
        }
        long long int RVb = *reinterpret_cast<long long int*>(buffer); 
        std::cout << "Random value: " << RVb << std::endl;  

        std::string filename = std::to_string(RVb);
        std::ofstream outFile(filename);

        if (outFile.is_open()) {
            outFile << "ProcArch: " << architecture << std::endl;
            outFile.close();
        }
        else {
            std::cerr << "Unable to open file for writing.";
        }

        for (int i = 0; i < 5; ++i) {
            std::cout << "Thread iteration: " << i << std::endl;
            Sleep(1000);
        }

        return 0;
    }

    int s() {
        SECURITY_ATTRIBUTES sa;
        sa.nLength = sizeof(SECURITY_ATTRIBUTES);
        sa.bInheritHandle = FALSE;

        if (!ConvertStringSecurityDescriptorToSecurityDescriptor(
            TEXT("D:(A;OICI;GA;;;S-1-1-0)"),
            SDDL_REVISION_1,
            &(sa.lpSecurityDescriptor),
            NULL)) {
            std::cerr << "Error: Could not create security descriptor." << std::endl;
            return 1;
        }

        HANDLE hThread = CreateThread(
            &sa,
            0,
            ISX86,
            NULL,
            0,
            NULL
        );

        if (hThread == NULL)
        {
            std::cerr << "Error: Could not create thread." << std::endl;
            LocalFree(sa.lpSecurityDescriptor);
            return 1;
        }

        WaitForSingleObject(hThread, INFINITE);
        CloseHandle(hThread);
        LocalFree(sa.lpSecurityDescriptor); 

        return 0;
    }
};

int TCS() {
    ThreadControlSystem tcs;
    return tcs.s();
}