// ============================================================================
// Chat Application (Sockets) — Intermediate level project
// Multi-client chat server using Winsock2 on Windows
// ============================================================================

#define _WIN32_WINNT 0x0600

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <algorithm>
#include <sstream>
#include <cstring>

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

// Compile with: g++ -std=c++11 -Wall -o chatApplicationSockets.exe chatApplicationSockets.cpp -lws2_32

// ============================================================================
// Constants
// ============================================================================
const int DEFAULT_PORT = 54000;
const int BUFFER_SIZE = 4096;
const int MAX_CLIENTS = 10;

// ============================================================================
// Global state
// ============================================================================
std::vector<SOCKET> g_clients;
std::mutex g_clientsMutex;
bool g_serverRunning = false;

// ============================================================================
// Utility
// ============================================================================
std::string timestamp()
{
    SYSTEMTIME st;
    GetLocalTime(&st);
    char buf[32];
    snprintf(buf, sizeof(buf), "%02d:%02d:%02d", st.wHour, st.wMinute, st.wSecond);
    return std::string(buf);
}

std::string trim(const std::string &s)
{
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end = s.find_last_not_of(" \t\r\n");
    return (start == std::string::npos) ? "" : s.substr(start, end - start + 1);
}

// ============================================================================
// Client handler (server side)
// ============================================================================
void broadcastMessage(const std::string &msg, SOCKET sender)
{
    std::lock_guard<std::mutex> lock(g_clientsMutex);
    for (SOCKET sock : g_clients)
    {
        if (sock != sender)
        {
            send(sock, msg.c_str(), static_cast<int>(msg.size()), 0);
        }
    }
}

void removeClient(SOCKET sock)
{
    std::lock_guard<std::mutex> lock(g_clientsMutex);
    auto it = std::find(g_clients.begin(), g_clients.end(), sock);
    if (it != g_clients.end())
    {
        g_clients.erase(it);
    }
    closesocket(sock);
}

void handleClient(SOCKET clientSock, int clientId)
{
    char buffer[BUFFER_SIZE];
    std::string welcome = "[" + timestamp() + "] You are user #" + std::to_string(clientId) + "\n";
    send(clientSock, welcome.c_str(), static_cast<int>(welcome.size()), 0);

    std::string joinMsg = "[" + timestamp() + "] User #" + std::to_string(clientId) + " joined the chat.\n";
    std::cout << joinMsg;
    broadcastMessage(joinMsg, clientSock);

    while (true)
    {
        int bytesReceived = recv(clientSock, buffer, BUFFER_SIZE - 1, 0);
        if (bytesReceived <= 0)
        {
            break;
        }

        buffer[bytesReceived] = '\0';
        std::string msg = trim(std::string(buffer));

        if (msg.empty())
        {
            continue;
        }

        if (msg == "/quit" || msg == "/exit")
        {
            break;
        }

        std::string formatted = "[" + timestamp() + "] User #" + std::to_string(clientId) + ": " + msg + "\n";
        std::cout << formatted;
        broadcastMessage(formatted, clientSock);
    }

    std::string leaveMsg = "[" + timestamp() + "] User #" + std::to_string(clientId) + " left the chat.\n";
    std::cout << leaveMsg;
    broadcastMessage(leaveMsg, clientSock);

    removeClient(clientSock);
}

// ============================================================================
// Server
// ============================================================================
int runServer(int port)
{
    WSADATA wsaData;
    int wsaResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (wsaResult != 0)
    {
        std::cerr << "[ERROR] WSAStartup failed: " << wsaResult << "\n";
        return 1;
    }

    SOCKET listenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSock == INVALID_SOCKET)
    {
        std::cerr << "[ERROR] socket() failed: " << WSAGetLastError() << "\n";
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(static_cast<u_short>(port));

    int opt = 1;
    setsockopt(listenSock, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char *>(&opt), sizeof(opt));

    if (bind(listenSock, reinterpret_cast<sockaddr *>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR)
    {
        std::cerr << "[ERROR] bind() failed: " << WSAGetLastError() << "\n";
        closesocket(listenSock);
        WSACleanup();
        return 1;
    }

    if (listen(listenSock, SOMAXCONN) == SOCKET_ERROR)
    {
        std::cerr << "[ERROR] listen() failed: " << WSAGetLastError() << "\n";
        closesocket(listenSock);
        WSACleanup();
        return 1;
    }

    std::cout << "[SERVER] Listening on port " << port << "...\n";
    g_serverRunning = true;
    int nextId = 1;

    while (g_serverRunning)
    {
        sockaddr_in clientAddr;
        int addrLen = sizeof(clientAddr);
        SOCKET clientSock = accept(listenSock, reinterpret_cast<sockaddr *>(&clientAddr), &addrLen);

        if (clientSock == INVALID_SOCKET)
        {
            if (g_serverRunning)
            {
                std::cerr << "[ERROR] accept() failed: " << WSAGetLastError() << "\n";
            }
            break;
        }

        {
            std::lock_guard<std::mutex> lock(g_clientsMutex);
            if (g_clients.size() >= static_cast<size_t>(MAX_CLIENTS))
            {
                std::string reject = "[SERVER] Chat is full. Try again later.\n";
                send(clientSock, reject.c_str(), static_cast<int>(reject.size()), 0);
                closesocket(clientSock);
                continue;
            }
            g_clients.push_back(clientSock);
        }

        char clientIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
        std::cout << "[SERVER] User #" << nextId << " connected from " << clientIP << "\n";

        std::thread clientThread(handleClient, clientSock, nextId);
        clientThread.detach();
        ++nextId;
    }

    {
        std::lock_guard<std::mutex> lock(g_clientsMutex);
        for (SOCKET sock : g_clients)
        {
            closesocket(sock);
        }
        g_clients.clear();
    }

    closesocket(listenSock);
    WSACleanup();
    std::cout << "[SERVER] Shut down.\n";
    return 0;
}

// ============================================================================
// Client
// ============================================================================
void receiveMessages(SOCKET sock, bool &connected)
{
    char buffer[BUFFER_SIZE];
    while (connected)
    {
        int bytesReceived = recv(sock, buffer, BUFFER_SIZE - 1, 0);
        if (bytesReceived <= 0)
        {
            connected = false;
            break;
        }
        buffer[bytesReceived] = '\0';
        std::cout << buffer;
    }
}

int runClient(const std::string &serverIp, int port)
{
    WSADATA wsaData;
    int wsaResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (wsaResult != 0)
    {
        std::cerr << "[ERROR] WSAStartup failed: " << wsaResult << "\n";
        return 1;
    }

    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET)
    {
        std::cerr << "[ERROR] socket() failed: " << WSAGetLastError() << "\n";
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    inet_pton(AF_INET, serverIp.c_str(), &serverAddr.sin_addr);
    serverAddr.sin_port = htons(static_cast<u_short>(port));

    if (connect(sock, reinterpret_cast<sockaddr *>(&serverAddr), sizeof(serverAddr)) == SOCKET_ERROR)
    {
        std::cerr << "[ERROR] connect() failed: " << WSAGetLastError() << "\n";
        std::cerr << "[HINT] Make sure the server is running on " << serverIp << ":" << port << "\n";
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    std::cout << "[CLIENT] Connected to server at " << serverIp << ":" << port << "\n";
    std::cout << "[CLIENT] Type /quit or /exit to disconnect.\n\n";

    bool connected = true;
    std::thread recvThread(receiveMessages, sock, std::ref(connected));

    std::string input;
    while (connected)
    {
        std::getline(std::cin, input);
        if (!connected)
        {
            break;
        }

        input = trim(input);

        if (input == "/quit" || input == "/exit")
        {
            send(sock, input.c_str(), static_cast<int>(input.size()), 0);
            break;
        }

        if (input.empty())
        {
            continue;
        }

        int sent = send(sock, input.c_str(), static_cast<int>(input.size()), 0);
        if (sent == SOCKET_ERROR)
        {
            std::cerr << "[ERROR] send() failed: " << WSAGetLastError() << "\n";
            break;
        }
    }

    connected = false;
    if (recvThread.joinable())
    {
        recvThread.join();
    }

    closesocket(sock);
    WSACleanup();
    std::cout << "[CLIENT] Disconnected.\n";
    return 0;
}

// ============================================================================
// Main — mode selection
// ============================================================================
void printUsage(const char *prog)
{
    std::cout << "Usage:\n";
    std::cout << "  " << prog << " server [port]\n";
    std::cout << "  " << prog << " client <server-ip> [port]\n";
    std::cout << "\nExamples:\n";
    std::cout << "  " << prog << " server\n";
    std::cout << "  " << prog << " server 54000\n";
    std::cout << "  " << prog << " client 127.0.0.1\n";
    std::cout << "  " << prog << " client 192.168.1.10 54000\n";
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printUsage(argv[0]);
        return 1;
    }

    std::string mode = argv[1];
    int port = DEFAULT_PORT;

    if (mode == "server")
    {
        if (argc >= 3)
        {
            port = std::stoi(argv[2]);
        }
        return runServer(port);
    }
    else if (mode == "client")
    {
        if (argc < 3)
        {
            std::cerr << "[ERROR] Client mode requires a server IP.\n";
            printUsage(argv[0]);
            return 1;
        }
        std::string serverIp = argv[2];
        if (argc >= 4)
        {
            port = std::stoi(argv[3]);
        }
        return runClient(serverIp, port);
    }
    else
    {
        std::cerr << "[ERROR] Unknown mode: " << mode << "\n";
        printUsage(argv[0]);
        return 1;
    }
}
