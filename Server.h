#pragma once
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <map>
#include <string>
#include <chrono>

void communicate() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed.\n";
        return;
    }

    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed.\n";
        WSACleanup();
        return;
    }

    sockaddr_in service;
    service.sin_family = AF_INET;
    service.sin_addr.s_addr = INADDR_ANY;
    service.sin_port = 0;

    if (bind(listenSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
        std::cerr << "Bind failed.\n";
        closesocket(listenSocket);
        WSACleanup();
        return;
    }

    int len = sizeof(service);
    if (getsockname(listenSocket, (struct sockaddr*)&service, &len) == -1) {
        std::cerr << "Getsockname failed.\n";
        closesocket(listenSocket);
        WSACleanup();
        return;
    }

    std::cout << "Server started on port " << ntohs(service.sin_port) << "\n";

    if (listen(listenSocket, 1) == SOCKET_ERROR) {
        std::cerr << "Listen failed.\n";
        closesocket(listenSocket);
        WSACleanup();
        return;
    }

    SOCKET acceptSocket = accept(listenSocket, NULL, NULL);
    if (acceptSocket == INVALID_SOCKET) {
        std::cerr << "Accept failed.\n";
        closesocket(listenSocket);
        WSACleanup();
        return;
    }

    closesocket(acceptSocket);
    closesocket(listenSocket);
    WSACleanup();
}