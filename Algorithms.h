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


inline bool c_t_c(int a, int b) {
    volatile int result = 0;
    result |= (a ^ b);
    return result == 0;
}
inline SecureBool IsProof() {
    auto dople = [](int x) { return x * 2 - x; };
    auto endStopRunNow = [](bool x) { return !x ? true : false; };

    auto CF1 = [](bool x) { return x && true; };
    auto CF2 = [](bool x) { return !x || false; };
    auto CF3 = [](bool x, bool y) { return x && y; }; 


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
    __asm nop;
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
