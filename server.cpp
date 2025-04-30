#include <iostream>
#include <thread>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include "file_system.h"

#pragma comment(lib, "Ws2_32.lib")

#define PORT 8080
#define BUFFER_SIZE 4096

void handle_client(SOCKET client_socket) {
    char buffer[BUFFER_SIZE];

    while (true) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received <= 0) break;

        std::string command(buffer);
        // Trim newline and carriage return
        command.erase(std::remove(command.begin(), command.end(), '\n'), command.end());
        command.erase(std::remove(command.begin(), command.end(), '\r'), command.end());
        std::string response;

        if (command.rfind("mkdir ", 0) == 0) {
            std::string dir = command.substr(6);
            response = create_directory(dir) ? "Directory created\n" : "Failed to create directory\n";
        } else if (command.rfind("ls ", 0) == 0) {
            std::string dir = command.substr(3);
            response = list_files(dir);
        } else if (command.rfind("read ", 0) == 0) {
            std::string file = command.substr(5);
            response = read_file(file);
        } else if (command.rfind("write ", 0) == 0) {
            size_t sep = command.find(' ', 6);
            if (sep != std::string::npos) {
                std::string file = command.substr(6, sep - 6);
                std::string content = command.substr(sep + 1);
                response = write_file(file, content) ? "File written\n" : "Failed to write file\n";
            } else {
                response = "Invalid write command\n";
            }
        }
        else if (command.rfind("move ", 0) == 0) {
            size_t sep = command.find(' ', 5);
            if (sep != std::string::npos) {
                std::string source = command.substr(5, sep - 5);
                std::string destination = command.substr(sep + 1);
                response = move_file_or_dir(source, destination) ? "Move successful\n" : "Move failed\n";
            } else {
                response = "Invalid move command\n";
            }
        }
         else if (command == "exit") {
            response = "Goodbye!\n";
            send(client_socket, response.c_str(), response.size(), 0);
            break;
        } else {
            response = "Unknown command\n";
        }

        response += "\n";  // ensure newline
        send(client_socket, response.c_str(), response.size(), 0);
    }

    closesocket(client_socket);
    std::cout << "Client disconnected\n";
}

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET server_fd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (sockaddr*)&address, sizeof(address));
    listen(server_fd, 5);

    std::cout << "Server listening on port " << PORT << "...\n";

    while (true) {
        SOCKET client_socket = accept(server_fd, nullptr, nullptr);
        std::cout << "Client connected\n";
        std::thread(handle_client, client_socket).detach();
    }

    WSACleanup();
    return 0;
}
