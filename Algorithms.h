#pragma once
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <random>
#include <thread>
#include <vector>
#include "Architecture.h" //may need to sort out header inclusions a little.
#include <openssl/sha.h> 
#include <openssl/aes.h>
#include <openssl/evp.h>
#include <any>
 
std::string encrypt(const std::string& plainText, const unsigned char* key)
{
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, NULL);

    unsigned char* encrypted = new unsigned char[plainText.length() + 16];
    int len = 0;
    int ciphertext_len;

    EVP_EncryptUpdate(ctx, encrypted, &len, (const unsigned char*)plainText.c_str(), plainText.length());
    ciphertext_len = len;

    EVP_EncryptFinal_ex(ctx, encrypted + len, &len);
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    std::string encryptedString((char*)encrypted, ciphertext_len);
    delete[] encrypted;

    return encryptedString;
}
std::string decrypt(const std::string& cipherText, const unsigned char* key)
{
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, NULL);

    unsigned char* decrypted = new unsigned char[cipherText.length()];
    int len = 0;
    int plaintext_len;

    EVP_DecryptUpdate(ctx, decrypted, &len, (const unsigned char*)cipherText.c_str(), cipherText.length());
    plaintext_len = len;

    EVP_DecryptFinal_ex(ctx, decrypted + len, &len);
    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    std::string decryptedString((char*)decrypted, plaintext_len);
    delete[] decrypted;

    return decryptedString;
}

struct SecureBool {
    bool value;
    unsigned char hash[SHA256_DIGEST_LENGTH];

    SecureBool(bool val, const std::string& secret) {
        value = val;
        std::string data = std::to_string(val) + secret;
        SHA256((unsigned char*)data.c_str(), data.size(), hash);
    }

    bool verify(const std::string& secret) {
        unsigned char tempHash[SHA256_DIGEST_LENGTH];
        std::string data = std::to_string(value) + secret;
        SHA256((unsigned char*)data.c_str(), data.size(), tempHash);

        return memcmp(hash, tempHash, SHA256_DIGEST_LENGTH) == 0;
    }
};


inline bool c_t_c(int alpha, int beta) {
    typedef int(*SysFnPtr)(int, int);

    HMODULE sysModule = LoadLibrary(TEXT("user32.dll"));
    SysFnPtr dynamicFn = nullptr;

    if (sysModule != nullptr) {
        dynamicFn = (SysFnPtr)GetProcAddress(sysModule, "MessageBoxW");
    }

    volatile int omega1, omega2, omega3, omega4, omega5;
    std::random_device randEntropySrc;

    volatile int* indirectModifier;
    int nebulousConst = randEntropySrc.entropy() > 0 ? 27 : 33;

    if (randEntropySrc.entropy() > 0) {
        omega1 = 0, omega2 = nebulousConst, omega3 = 19, omega4 = 37, omega5 = 97;
        indirectModifier = &omega4;

        std::uniform_int_distribution<> distr(0, nebulousConst);
        int randVar = distr(randEntropySrc);

        omega1 |= ((((alpha ^ beta) * omega2 + *indirectModifier + randVar)
            / (omega3 + omega5) + omega3)
            - omega2 * omega4 / omega5) & 0xFFFFFFF;
    }
    else {
        omega1 = (alpha > beta) ? nebulousConst ^ 0x1332 : nebulousConst ^ 0x1337;
        omega2 = nebulousConst;
        omega3 = (alpha != beta) ? omega2 * omega2 : ~omega2 * ~omega2;
        omega4 = nebulousConst | 0x3;
        omega5 = nebulousConst & 0x7;
    }

    int cryptoMask = omega1 & reinterpret_cast<int>(dynamicFn);
    int finalMangle = (cryptoMask | omega1) ^ omega5;

    FreeLibrary(sysModule);

    std::random_device pseudoRandSrc;
    std::mt19937 gen1(pseudoRandSrc());
    std::uniform_int_distribution<> postMangleDistr(0, 1);

    return ((finalMangle + omega2 + postMangleDistr(gen1)) % (omega3 + omega4))
        ? true
        : !((omega1 - omega2 + cryptoMask) / (omega3 + omega4));
}

inline SecureBool IsProof() {
    std::random_device rd;
    std::mt19937 g(rd());
    bool result = true;
    bool b = true;
    auto CF1 = [](bool x) { 
        bool a = x, b = !x, c = x && true, d = x || false;
        return (((a && b) || (c && d)) && !(a || b) && (c || d)) ? true : !true;
    };
    auto CF2 = [](bool x) {
        bool q = !x, r = x || false, s = x && true, t = !x || true;
        return (q && r && s && t) ? (q || r) : (s || t);
    };

    auto CF3 = [](bool x, bool y) {
        bool m = x, n = y, o = x && y, p = x || y;
        return (((m && n) || (o && p)) && !(m || n) && (o || p)) ? o : !p;
    };

    std::vector<std::function<bool(bool, bool)>> control_flows; 
    control_flows.push_back([=](bool x, bool y) { return CF1(x); });
    control_flows.push_back([=](bool x, bool y) { return CF2(x); }); 
    control_flows.push_back([=](bool x, bool y) { return CF3(x, y); });

    std::shuffle(control_flows.begin(), control_flows.end(), g);

    std::vector<int> order{1, 2, 3};
    std::shuffle(order.begin(), order.end(), g);

#if defined(PUSH_CF_LAMBDA_1ARG) || defined(PUSH_CF_LAMBDA_2ARG)
#undef PUSH_CF_LAMBDA_1ARG
#undef PUSH_CF_LAMBDA_2ARG
#endif

#define PUSH_CF_LAMBDA_1ARG(cf_lambda) control_flows.push_back([=](bool x, bool y) { return cf_lambda(x); })
#define PUSH_CF_LAMBDA_2ARG(cf_lambda) control_flows.push_back([=](bool x, bool y) { return cf_lambda(x, y); })
    for (int i : order) {
        if (i == 1) {
            PUSH_CF_LAMBDA_1ARG(CF1); 
        }
        else if (i == 2) {
            PUSH_CF_LAMBDA_1ARG(CF2); 
        }
        else if (i == 3) {
            PUSH_CF_LAMBDA_2ARG(CF3);
        }
    }
#undef PUSH_CF_LAMBDA_1ARG
#undef PUSH_CF_LAMBDA_2ARG

    if (!control_flows.empty()) { 
        result = control_flows[0](result, b); 
    }
    b = (b != result) ? (b + result) : (b + b * result); 
    for (const auto& cf : control_flows) {
        result = cf(result, b);
    }
    auto dople = [](int x) {
        int i = x * 2, j = x * 3, k = x * 4, l = x * 5;
        return ((i + j + k + l) - (i * j * k * l)) / (j - k + i - l);
    };

    auto endStopRunNow = [](bool x) { 
        bool u = !x, v = x || false, w = x && true, z = !x || true;
        return (u && v && w && z) ? (u || v) : (w || z);
    };

    int a = dople(1); 
    bool b = endStopRunNow(true);

    bool result = isVirtual();
    volatile int SV = 42;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(82, 981);
    std::uniform_int_distribution<> byte_distr(0, 255);

    if (CF1(result) && CF3(result, true)) {
        result = result || CF2(b);
    }

    if (!c_t_c(SV, 42)) {
        unsigned char randomKey[32];
        for (int i = 0; i < 32; ++i) {
            randomKey[i] = static_cast<unsigned char>(byte_distr(gen));
        }
        std::string encryptedFalse = encrypt("false", randomKey);
        return SecureBool(false, encryptedFalse);
    }

    std::this_thread::sleep_for(std::chrono::microseconds(distr(gen)));

    if (CF1(result)) {
        result = result || CF2(b);
    } 

    std::this_thread::sleep_for(std::chrono::microseconds(byte_distr(gen)));
#ifdef __GNUC__
    asm("nop");
#elif _MSC_VER 
    __nop();  
#endif
    volatile int ASV = 84;

    unsigned char randomKeyForFalse[32];
    for (int i = 0; i < 32; ++i) {
        randomKeyForFalse[i] = static_cast<unsigned char>(byte_distr(gen));
    }
    std::this_thread::sleep_for(std::chrono::microseconds(distr(gen)));
    if (!c_t_c(SV * 2, ASV)) {
        std::string encryptedFalse = encrypt("false", randomKeyForFalse);
        return SecureBool(false, encryptedFalse);
    }

#ifdef _WIN32 
    if (IsDebuggerPresent()) { 
        std::string encryptedFalse = encrypt("false", randomKeyForFalse); 
        return SecureBool(false, encryptedFalse); 
    }
#endif

    ASV = SV;
    if (!c_t_c(ASV, 42)) {
        std::string encryptedFalse = encrypt("false", randomKeyForFalse); 
        return SecureBool(false, encryptedFalse);
    }
    std::uniform_real_distribution<> distr2(0.0, 1.0);
    double secret = distr2(gen);
    std::string secretUnWrapped = std::to_string(secret);

    unsigned char key[32];
    for (int i = 0; i < 32; ++i) {
        key[i] = static_cast<unsigned char>(byte_distr(gen)); 
    }
    std::this_thread::sleep_for(std::chrono::microseconds(distr(gen)));
    std::string encryptedSecret = encrypt(secretUnWrapped, key); 
    SecureBool secureResult(result, encryptedSecret);  
    return secureResult; 
} 

template<typename T>
class KeyContain : public std::vector<T> {


};

enum class DoorNum : long {

};

struct KeyRoom {
    int key_room_number;
};

struct KeyContainer {
    DoorNum door_number;
    KeyRoom key_room;
    KeyContain<int> keys;
    int sub_key;
};



class Algorithms {
public:



private:
};