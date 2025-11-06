#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include <dirent.h>
#include <fstream>
#include <string>
#define PORT 8080

void listFiles(int new_socket) {
    DIR *dir;
    struct dirent *entry;
    dir = opendir("Database");

    if (dir == NULL) {
        const char *error = "Could not open Database directory.";
        send(new_socket, error, strlen(error), 0);
        return;
    }

    std::string fileList = "Files in Database:\n";
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            fileList += entry->d_name;
            fileList += "\n";
        }
    }
    closedir(dir);
    send(new_socket, fileList.c_str(), fileList.size(), 0);
}

void sendFile(int new_socket, const std::string &filename) {
    std::string filepath = "Database/" + filename;
    std::ifstream file(filepath, std::ios::binary);

    if (!file) {
        std::string error = "ERROR: File not found in Database.";
        send(new_socket, error.c_str(), error.size(), 0);
        return;
    }

    std::string ok = "OK";
    send(new_socket, ok.c_str(), ok.size(), 0);
    sleep(1);

    char buffer[1024];
    while (!file.eof()) {
        file.read(buffer, sizeof(buffer));
        send(new_socket, buffer, file.gcount(), 0);
    }
    file.close();
    std::cout << "File " << filename << " sent from Database.\n";
}

void receiveFile(int new_socket, const std::string &filename) {
    std::string filepath = "Database/" + filename;
    std::ofstream file(filepath, std::ios::binary);

    if (!file) {
        std::string error = "ERROR: Cannot save file in Database.";
        send(new_socket, error.c_str(), error.size(), 0);
        return;
    }

    std::string ok = "OK";
    send(new_socket, ok.c_str(), ok.size(), 0);

    char buffer[1024];
    int bytesRead;
    while ((bytesRead = read(new_socket, buffer, sizeof(buffer))) > 0) {
        file.write(buffer, bytesRead);
        if (bytesRead < 1024)
            break;
    }
    file.close();
    std::cout << "File " << filename << " uploaded to Database.\n";
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    char buffer[1024] = {0};

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 3);
    std::cout << "Server ready on port " << PORT << "...\n";

    while (true) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
        memset(buffer, 0, 1024);
        read(new_socket, buffer, 1024);

        std::string command(buffer);

        if (command == "LIST") {
            listFiles(new_socket);
        } 
        else if (command.rfind("GET ", 0) == 0) {
            std::string filename = command.substr(4);
            sendFile(new_socket, filename);
        } 
        else if (command.rfind("PUT ", 0) == 0) {
            std::string filename = command.substr(4);
            receiveFile(new_socket, filename);
        } 
        else {
            const char *msg = "Unknown command.";
            send(new_socket, msg, strlen(msg), 0);
        }

        close(new_socket);
    }

    return 0;
}
