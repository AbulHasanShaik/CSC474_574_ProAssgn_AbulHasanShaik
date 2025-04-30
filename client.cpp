#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

#pragma comment(lib, "Ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 4096

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr));

    std::cout << "Connected to server. Type commands (mkdir <dir>, ls <dir>, write <file> <data>, read <file>, move <src> <dest>, exit):\n";

    char buffer[BUFFER_SIZE];

    while (true) {
        std::string input;
        std::getline(std::cin, input);

        if (input == "exit") {
            input += "\n";
            send(sock, input.c_str(), input.size(), 0);
            break;
        }

        input += "\n";
        send(sock, input.c_str(), input.size(), 0);  

        memset(buffer, 0, BUFFER_SIZE);
        int valread = recv(sock, buffer, BUFFER_SIZE, 0);
        if (valread > 0) {
            std::string response(buffer, valread);
            std::cout << "Server: " << response;
        } else {
            std::cout << "Server closed the connection.\n";
            break;
        }
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
