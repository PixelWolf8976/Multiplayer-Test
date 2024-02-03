#pragma once

#include <iostream>
#include <string>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <thread>
#include <vector>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

void handleClient(SOCKET clientSocket) {
    int sharedVariable = 0;

    while (true) {
        char buffer[1024] = { 0 };
        recv(clientSocket, buffer, sizeof(buffer), 0);

        if (strcmp(buffer, "get") == 0) {
            send(clientSocket, reinterpret_cast<char*>(&sharedVariable), sizeof(sharedVariable), 0);
        }
        else if (strncmp(buffer, "set", 3) == 0) {
            int newValue;
            sscanf_s(buffer, "set %d", &newValue);
            sharedVariable = newValue;
        }
    }

    closesocket(clientSocket);
}

class Multiuser {
public:

    void startServer() {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            cerr << "Failed to initialize Winsock." << endl;
            return;
        }

        SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);

        sockaddr_in serverAddress;
        serverAddress.sin_family = AF_INET;
        inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr);
        serverAddress.sin_port = htons(12345);

        bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
        listen(serverSocket, 5);

        cout << "Server waiting for connections..." << endl;

        vector<thread> threads;

        while (true) {
            SOCKET clientSocket = accept(serverSocket, NULL, NULL);
            cout << "Client connected." << endl;

            threads.emplace_back(handleClient, clientSocket);
            threads.back().detach(); // Detach the thread to run it independently
        }

        closesocket(serverSocket);
        WSACleanup();
        return;
    }

    void startClient() {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            cerr << "Failed to initialize Winsock." << endl;
            return;
        }

        SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);

        sockaddr_in serverAddress;
        serverAddress.sin_family = AF_INET;
        inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr); // Use inet_pton() here
        serverAddress.sin_port = htons(12345);

        connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
        cout << "Connected to server." << endl;

        while (true) {
            string command;
            cout << "Enter command (get, set <value>): ";
            getline(cin, command);

            send(clientSocket, command.c_str(), command.size(), 0);

            if (command == "exit") {
                break;
            }

            if (command == "get") {
                int receivedValue;
                recv(clientSocket, reinterpret_cast<char*>(&receivedValue), sizeof(receivedValue), 0);
                cout << "Shared variable value: " << receivedValue << endl;
            }
        }

        closesocket(clientSocket);
        WSACleanup();
        return;
    }
};