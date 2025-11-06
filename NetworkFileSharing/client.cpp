#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <fstream>
#define PORT 8080

void uploadFile(const std::string &filename) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    std::string command = "PUT " + filename;
    send(sock, command.c_str(), command.size(), 0);

    read(sock, buffer, 1024);
    if (strncmp(buffer, "OK", 2) != 0) {
        std::cout << "Server error: " << buffer << std::endl;
        close(sock);
        return;
    }

    std::ifstream file("Local/" + filename, std::ios::binary);
    if (!file) {
        std::cout << "File not found in Local folder.\n";
        close(sock);
        return;
    }

    std::cout << "Uploading " << filename << " to Database on server...\n";
    char data[1024];
    while (!file.eof()) {
        file.read(data, sizeof(data));
        send(sock, data, file.gcount(), 0);
    }

    file.close();
    close(sock);
    std::cout << "Upload complete!\n";
}

void downloadFile(const std::string &filename) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    std::string command = "GET " + filename;
    send(sock, command.c_str(), command.size(), 0);

   read(sock, buffer, 1024);
    std::string serverResponse = buffer;
    memset(buffer, 0, 1024);

if (serverResponse == "NO_FILE") {
    std::cout << "Error: File not found in Database.\n";
    close(sock);
    return;
}

if (serverResponse == "OK") {
    std::ofstream outFile("Downloads/" + filename, std::ios::binary);
    std::cout << "Downloading " << filename << " from Database...\n";

    int bytesRead;
    while ((bytesRead = read(sock, buffer, 1024)) > 0) {
        if (std::string(buffer, bytesRead) == "END") break;
        outFile.write(buffer, bytesRead);
        memset(buffer, 0, 1024);
    }
    outFile.close();
    std::cout << "Download complete! File saved to Downloads/ folder.\n";
}


    close(sock);
}

void listFiles() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;
    char buffer[4096] = {0};

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);

    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    send(sock, "LIST", strlen("LIST"), 0);
    read(sock, buffer, 4096);
    std::cout << buffer << std::endl;

    close(sock);
}

int main() {
    int choice;
    std::string filename;

    while (true) {
        std::cout << "\n==== File Sharing Menu ====\n";
        std::cout << "1. List files in Database\n";
        std::cout << "2. Download file (to Downloads)\n";
        std::cout << "3. Upload file (from Local)\n";
        std::cout << "4. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                listFiles();
                break;
            case 2:
                std::cout << "Enter filename to download: ";
                std::cin >> filename;
                downloadFile(filename);
                break;
            case 3:
                std::cout << "Enter filename to upload: ";
                std::cin >> filename;
                uploadFile(filename);
                break;
            case 4:
                std::cout << "Exiting...\n";
                return 0;
            default:
                std::cout << "Invalid choice.\n";
        }
    }
}
